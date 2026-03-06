#ifndef CYCLESLIPDETECTION_PROGRAMBASE_H_
#define CYCLESLIPDETECTION_PROGRAMBASE_H_

namespace cycle_slip_detection {

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