#ifndef CYCLESLIPDETECTION_PROGRAMBASE_H_
#define CYCLESLIPDETECTION_PROGRAMBASE_H_

#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"

namespace cycle_slip_detection {

struct GnssTime {
  int year, month, day, hour, minute;
  double second;
  std::string time_sys;  // 时间系统，例如 GPS, BDS, GAL 等
};

// 卫星系统类型
struct ObsType {
  char sys_code;
  int num_types;
  std::vector<std::string> type_list;
};

// 每个历元的观测值
struct EpochData {
  GnssTime epoch_time;
  int num_sats;
  absl::flat_hash_map<std::string, std::vector<double>>
      sat_obs;  // 卫星编号和对应的观测值列表
};

// 单观测文件数据
struct ObsData {
  double rnx_version;
  GnssTime start_time;
  GnssTime end_time;
  std::vector<ObsType> obs_type;
  std::vector<EpochData> epoch_data;
};

#define ASSIGN_OR_RETURN_IMPL(tmp_var, lhs, rexpr) \
  auto tmp_var = (rexpr);                          \
  if (!tmp_var.ok()) {                             \
    return tmp_var.status();                       \
  }                                                \
  lhs = std::move(tmp_var).value()

// 组合宏，利用 __LINE__ 防止变量名冲突
#define ASSIGN_OR_RETURN(lhs, rexpr) \
  ASSIGN_OR_RETURN_IMPL(status_or_##__LINE__, lhs, rexpr)

}  // namespace cycle_slip_detection

#endif  // CYCLESLIPDETECTION_PROGRAMBASE_H_