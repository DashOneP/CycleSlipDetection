#include "main_app_logic.h"

#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "program_base.h"
#include "read_rnx_file.h"

/**
 * 目前程序为单线程计算，计划改为多线程运算
 * 一个线程进行文件读取+数据解析
 * 另一个线程进行计算
 */

// 定义命令行参数
ABSL_FLAG(std::string, input_file_path, "./data/input.25o",
          "Path to the input observation file.");

namespace cycle_slip_detection {

absl::Status RunMainLogic(int argc, char* argv[]) {
  // 解析命令行参数
  std::vector<char*> args =
      absl::ParseCommandLine(argc, argv);  // 接受不带有--或者-的参数
  // 解析已定义的参数命令，即--后面，在ABSL_FLAG中定义的参数
  std::string input_file_path = absl::GetFlag(FLAGS_input_file_path);
  // 读取文件，成功则返回 RnxFileReader 对象，失败则返回错误状态
  ASSIGN_OR_RETURN(auto rnx_reader, RnxFileReader::Create(input_file_path));
  rnx_reader->ReadRnxFile();  // 读取RINEX文件，解析数据并存储到全局变量中

  return absl::OkStatus();
}

}  // namespace cycle_slip_detection