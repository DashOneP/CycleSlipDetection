#include "read_rnx_file.h"

#include <string>

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
  void ReadRnxFileHeader();
  void ReadRnxFileBody();
}
void RnxFileReader::ReadRnxFileHeader() {
  std::string line;
  std::string line_comment;
  double rnx_version = 2.1;
  //逐行读取
  while (std::getline(file_stream_, line)) {
    if (line.empty()) {
      continue;
    }
    line_comment = line.substr(60);  // RINEX文件中，前60列是数据，后面是注释
    // 获取文件版本，文件类型
    if (line_comment.find("RINEX VERSION / TYPE")) {
      rnx_version = std::stod(line.substr(0, 9));
    }
    // 获取卫星系统，观测值类型，观测值数量
    if (line_comment.find("SYS / # / OBS TYPES")) {
      
    }
  }
}
void RnxFileReader::ReadRnxFileBody() {
  // 读取RINEX文件主体数据
}

}  // namespace cycle_slip_detection