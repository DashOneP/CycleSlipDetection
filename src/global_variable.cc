#include "global_variable.h"

namespace cycle_slip_detection {

ObsTypeManager& ObsTypeManager::GetInstance() {
  static absl::NoDestructor<ObsTypeManager> instance;
  return *instance;
}

void ObsTypeManager::AddObsType(const ObsType& obs_type) {
  absl::MutexLock lock(&mu_);
  obs_types_.push_back(obs_type);
}

std::vector<ObsType> ObsTypeManager::GetObsTypes(char sys_code) const {
  absl::MutexLock lock(&mu_);
  std::vector<ObsType> result;
  for (const auto& obs_type : obs_types_) {
    if (obs_type.sys_code == sys_code) {
      result.push_back(obs_type);
    }
  }
  return result;  // 如果没有找到，返回一个空的 vector
}

ObsManager& ObsManager::GetInstance() {
  static absl::NoDestructor<ObsManager> instance;
  return *instance;
}

void ObsManager::AddObsData(const ObsData& obs_data) {
  absl::MutexLock lock(&mu_);
  obs_data_ = obs_data;  // 存储观测数据，实际实现中可以考虑存储多个 ObsData
  // 这里可以添加线程安全机制，例如使用 mutex 来保护数据访问
  // 但由于目前只有一个 ObsData，我们暂时不需要复杂的同步机制
  // 如果未来需要支持多个 ObsData，可以考虑使用 std::vector<ObsData> 和 mutex
}

ObsData ObsManager::GetObsData() const {
  absl::MutexLock lock(&mu_);
  // 这里可以添加线程安全机制，例如使用 mutex 来保护数据访问
  // 但由于目前只有一个 ObsData，我们暂时不需要复杂的同步机制
  // 如果未来需要支持多个 ObsData，可以考虑使用 std::vector<ObsData> 和 mutex
  return obs_data_;  // 返回存储的 ObsData，实际实现中应该返回存储的
                     // ObsData
}

EpochDataManager& EpochDataManager::GetInstance() {
  static absl::NoDestructor<EpochDataManager> instance;
  return *instance;
}

void EpochDataManager::AddEpoch(EpochData epoch) {
  absl::MutexLock lock(&mu_);
  epochs_.push_back(std::move(epoch));
}

void EpochDataManager::ProcessAllEpochs(
    const std::function<void(const std::vector<EpochData>&)>& processor) const {
  if (!processor) {
    return;  // 如果没有提供处理函数，直接返回
  }
  absl::MutexLock lock(&mu_);
  processor(epochs_);
}

size_t EpochDataManager::GetSize() const {
  absl::MutexLock lock(&mu_);
  return epochs_.size();
}

void EpochDataManager::ClearEpochs() {
  absl::MutexLock lock(&mu_);
  epochs_.clear();
}

}  // namespace cycle_slip_detection
