#include "read_rnx_file.h"

#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/ascii.h"
#include "absl/strings/string_view.h"
#include "global_variable.h"
#include "program_base.h"

namespace cycle_slip_detection {

absl::StatusOr<std::unique_ptr<RnxFileReader>> RnxFileReader::Create(
    const std::string& input_file) {
  if (input_file.empty()) {
    return absl::InvalidArgumentError("输入文件名不能为空");
  }
  auto reader = std::unique_ptr<RnxFileReader>(new RnxFileReader(input_file));
  reader->file_stream_.open(input_file);
  if (!reader->file_stream_.is_open()) {
    return absl::NotFoundError("Failed to open RINEX file: " + input_file);
  }
  return reader;
}

void RnxFileReader::ReadRnxFile() {
  ReadRnxFileHeader();
  ReadRnxFileBody();
}

// 单文件观测数据ObsData，只在此处使用，作为存入全局变量的临时变量
ObsData obs_data_tmp;

// 读取RINEX文件头部数据
void RnxFileReader::ReadRnxFileHeader() {
  std::string line;
  std::string line_comment;
  double rnx_version = 2.1;
  GnssTime start_time = {0, 0, 0, 0, 0, 0.0, ""};
  GnssTime end_time = {0, 0, 0, 0, 0, 0.0, ""};
  ObsType obs_type = {'G', 0, {""}};

  // 逐行读取
  while (std::getline(file_stream_, line)) {
    if (line.empty()) {
      continue;
    }
    line_comment = line.substr(60);  // RINEX文件中，前60列是数据，后面是注释
    // 获取文件版本，文件类型
    if (line_comment.find("RINEX VERSION / TYPE") != std::string::npos) {
      rnx_version = std::stod(line.substr(0, 9));
    }
    // 获取卫星系统，观测值类型，观测值数量
    if (line_comment.find("SYS / # / OBS TYPES") != std::string::npos) {
      char sys_code = line[0];
      int frequency_num = std::stoi(line.substr(3, 6));
      std::vector<std::string> type_list;
      for (int i = 0, col = 7; i < frequency_num; ++i, col += 4) {
        if (col <= 55) {
          type_list.push_back(line.substr(col, 4));
        } else {
          std::getline(file_stream_, line);  // 超过60列，停止读取
          col = 3;
          i--;  // 下次循环会让i增加1，所以这里需要减1来抵消
        }
      }
      obs_type.sys_code = sys_code;
      obs_type.num_types = frequency_num;
      obs_type.type_list = type_list;
      ObsManager::GetInstance().SetObsType(obs_type);
      // ObsManager::GetInstance().ProcessObsData([&](const ObsData& data) {
      //   std::cout << data.obs_type[0].sys_code << std::endl;
      // });
    }

    if (line_comment.find("TIME OF FIRST OBS") != std::string::npos) {
      start_time.year = std::stoi(line.substr(0, 6));
      start_time.month = std::stoi(line.substr(6, 6));
      start_time.day = std::stoi(line.substr(12, 6));
      start_time.hour = std::stoi(line.substr(18, 6));
      start_time.minute = std::stoi(line.substr(24, 6));
      start_time.second = std::stod(line.substr(30, 13));
      start_time.time_sys =
          absl::StripLeadingAsciiWhitespace(line.substr(43, 8));
    }

    if (line_comment.find("TIME OF LAST OBS") != std::string::npos) {
      end_time.year = std::stoi(line.substr(0, 6));
      end_time.month = std::stoi(line.substr(6, 6));
      end_time.day = std::stoi(line.substr(12, 6));
      end_time.hour = std::stoi(line.substr(18, 6));
      end_time.minute = std::stoi(line.substr(24, 6));
      end_time.second = std::stod(line.substr(30, 13));
      end_time.time_sys = absl::StripLeadingAsciiWhitespace(line.substr(43, 8));
      ObsManager::GetInstance().SetObsBaseData(
          {rnx_version, start_time, end_time});  // 存入全局变量
    }

    if (line_comment.find("END OF HEADER") != std::string::npos) {
      return;  // 读取到文件头结束，停止读取
    }
  }
}
// 读取RINEX文件主体数据
void RnxFileReader::ReadRnxFileBody() {}

}  // namespace cycle_slip_detection