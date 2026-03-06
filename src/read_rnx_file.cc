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

}  // namespace cycle_slip_detection