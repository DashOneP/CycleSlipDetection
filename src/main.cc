#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "main_app_logic.h"
/**
 * @brief Programme Entrance
 */
int main(int argc, char* argv[]) {
  absl::SetProgramUsageMessage(
      "这是一个高精度的 GNSS 周跳探测程序 (Cycle Slip Detection)。\n"
      "用法示例: ./CycleSlipDetection --input_file_path=./data/test.25o");
  // 程序主逻辑入口，调用 main_app_logic.cc 中的函数
  auto program_status = cycle_slip_detection::RunMainLogic(argc, argv);
  return program_status.ok() ? EXIT_SUCCESS : EXIT_FAILURE;
}