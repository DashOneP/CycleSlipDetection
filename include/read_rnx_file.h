#ifndef CYCLESLIPDETECTION_READRNXFILE_H_
#define CYCLESLIPDETECTION_READRNXFILE_H_

#include <fstream>
#include <string>

#include "absl/status/statusor.h"

namespace cycle_slip_detection {

class RnxFileReader {
 public:
  static absl::StatusOr<std::unique_ptr<RnxFileReader>> Create(
      const std::string& input_file);
  void ReadRnxFile();
  void ReadRnxFileHeader();
  void ReadRnxFileBody();

 private:
  std::string input_file_path_;
  std::ifstream file_stream_;
  explicit RnxFileReader(const std::string& input_file_path)
      : input_file_path_(input_file_path) {}
};

}  // namespace cycle_slip_detection

#endif  // CYCLESLIPDETECTION_READRNXFILE_H_