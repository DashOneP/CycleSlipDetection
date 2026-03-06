#include "main_app_logic.h"

#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/status/status.h"
#include "program_base.h"
#include "read_rnx_file.h"

// 定义命令行参数
ABSL_FLAG(std::string, input_file_path, "./data/input.25o",
          "Path to the input observation file.");

namespace cycle_slip_detection {

absl::Status RunMainLogic(int argc, char* argv[]) {
  // 解析命令行参数
  std::vector<char*> args =
      absl::ParseCommandLine(argc, argv);  // 接受不带有--或者-的参数
  // 解析已定义的参数命令
  std::string input_file_path = absl::GetFlag(FLAGS_input_file_path);
  // 读取文件
  ASSIGN_OR_RETURN(std::unique_ptr<RnxFileReader> rnx_reader,
                   RnxFileReader::Create(input_file_path));

  return absl::OkStatus();
}

}  // namespace cycle_slip_detection