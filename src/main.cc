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

  // 2. 如果运行出错了，打印出你 Create 函数里写的报错信息！
  if (!program_status.ok()) {
    std::cerr << "程序运行失败: " << program_status.ToString() << std::endl;
    return EXIT_FAILURE;  // 告诉 Linux 系统程序异常退出
  }

  return EXIT_SUCCESS;  // 完美结束
}