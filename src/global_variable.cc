#include "global_variable.h"

namespace cycle_slip_detection {

// ObsTypeManager& ObsTypeManager::GetInstance() {
//   static absl::NoDestructor<ObsTypeManager> instance;
//   return *instance;
// }

// void ObsTypeManager::AddObsType(const ObsType& obs_type) {
//   absl::MutexLock lock(&mu_);
//   obs_types_.push_back(obs_type);
// }

// std::vector<ObsType> ObsTypeManager::GetObsTypes(char sys_code) const {
//   absl::MutexLock lock(&mu_);
//   std::vector<ObsType> result;
//   for (const auto& obs_type : obs_types_) {
//     if (obs_type.sys_code == sys_code) {
//       result.push_back(obs_type);
//     }
//   }
//   return result;  // 如果没有找到，返回一个空的 vector
// }

ObsManager& ObsManager::GetInstance() {
  static absl::NoDestructor<ObsManager> instance;
  return *instance;
}

void ObsManager::SetObsBaseData(const ObsData& obs_data) {
  absl::MutexLock lock(&mu_);
  obs_data_.rnx_version = obs_data.rnx_version;
  obs_data_.start_time = obs_data.start_time;
  obs_data_.end_time = obs_data.end_time;
}

void ObsManager::SetObsType(const ObsType& obs_type) {
  absl::MutexLock lock(&mu_);
  obs_data_.obs_type.push_back(obs_type);
}

void ObsManager::AddObsData(const EpochData& epoch_data) {
  absl::MutexLock lock(&mu_);
  obs_data_.epoch_data.push_back(epoch_data);  // 添加历元数据
}

void ObsManager::ProcessObsData(
    const std::function<void(const ObsData&)>& processor) const {
  absl::MutexLock lock(&mu_);

  if (!processor) {
    return;
  }
  // absl::MutexLock lock(&mu_);
  processor(obs_data_);
}

// EpochDataManager& EpochDataManager::GetInstance() {
//   static absl::NoDestructor<EpochDataManager> instance;
//   return *instance;
// }

// void EpochDataManager::AddEpoch(EpochData epoch) {
//   absl::MutexLock lock(&mu_);
//   epochs_.push_back(std::move(epoch));
// }

// void EpochDataManager::ProcessAllEpochs(
//     const std::function<void(const std::vector<EpochData>&)>& processor)
//     const {
//   if (!processor) {
//     return;  // 如果没有提供处理函数，直接返回
//   }
//   absl::MutexLock lock(&mu_);
//   processor(epochs_);
// }

// size_t EpochDataManager::GetSize() const {
//   absl::MutexLock lock(&mu_);
//   return epochs_.size();
// }

// void EpochDataManager::ClearEpochs() {
//   absl::MutexLock lock(&mu_);
//   epochs_.clear();
// }

}  // namespace cycle_slip_detection
