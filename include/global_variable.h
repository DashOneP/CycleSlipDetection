#ifndef CYCLESLIPDETECTION_GLOBALVARIABLE_H_
#define CYCLESLIPDETECTION_GLOBALVARIABLE_H_

#include "absl/base/no_destructor.h"
#include "absl/base/thread_annotations.h"
#include "absl/synchronization/mutex.h"
#include "program_base.h"

namespace cycle_slip_detection {

// 管理单观测文件数据的单例类
class ObsManager {
 public:
  static ObsManager& GetInstance();
  void AddObsData(const ObsData& obs_data);  // 添加观测数据
  ObsData GetObsData() const;                // 获取观测数据

 private:
  friend class absl::NoDestructor<ObsManager>;
  ObsManager() = default;                  // 私有构造函数，禁止外部实例化
  ObsManager(const ObsManager&) = delete;  // 禁止复制
  ObsManager& operator=(const ObsManager&) = delete;  // 禁止赋值

  // mu_线程锁，保护 obs_data_ 的访问，在之后所有需要访问
  // obs_data_这个数据之前，都必须对其进行上锁保护在进行操作
  mutable absl::Mutex mu_;
  ObsData obs_data_ ABSL_GUARDED_BY(mu_);
};

class ObsTypeManager {
 public:
  static ObsTypeManager& GetInstance();      // 静态函数，获取单例实例
  void AddObsType(const ObsType& obs_type);  // 添加观测类型
  std::vector<ObsType> GetObsTypes(
      char sys_code) const;  // 根据卫星系统代码获取观测类型

 private:
  // 友元类，允许 NoDestructor访问私有成员
  friend class absl::NoDestructor<ObsTypeManager>;
  ObsTypeManager() = default;  // 私有构造函数，禁止外部实例化
  ObsTypeManager(const ObsTypeManager&) = delete;
  ObsTypeManager& operator=(const ObsTypeManager&) = delete;

  mutable absl::Mutex mu_;
  std::vector<ObsType> obs_types_ ABSL_GUARDED_BY(mu_);
};

class EpochDataManager {
 public:
  static EpochDataManager& GetInstance();  // 静态函数，获取单例实例
  void AddEpoch(EpochData epoch);          // 添加历元数据
  // 【核心修改点】：不再返回 vector 副本，而是接收一个处理函数
  // processor 是一个可调用对象（如 Lambda），它接收内部数据的常量引用
  void ProcessAllEpochs(
      const std::function<void(const std::vector<EpochData>&)>& processor)
      const;
  /**
   * ProcessAllEpochs使用示例
   * manager = EpochDataManager::GetInstance();
   * manager.ProcessAllEpochs(
   *  // 这里是处理函数，也是ProcessAllEpochs函数的参数，下面是Lambda表达式
   *  // Lambda表达式特点[](...){...}
   *  // []表示这是一个Lambda表达式，()里是参数列表，{}里是函数体
   *  [](const std::vector<EpochData>& epochs {
   *
   *   }
   * )
   */
  size_t GetSize() const;  // 获取历元数据数量
  void ClearEpochs();      // 清空所有历元数据

 private:
  friend class absl::NoDestructor<EpochDataManager>;
  EpochDataManager() = default;
  EpochDataManager(const EpochDataManager&) = delete;             // 禁止复制
  EpochDataManager& operator=(const EpochDataManager&) = delete;  // 禁止赋值

  mutable absl::Mutex mu_;
  std::vector<EpochData> epochs_ ABSL_GUARDED_BY(mu_);
};

}  // namespace cycle_slip_detection

#endif  // CYCLESLIPDETECTION_GLOBALVARIABLE_H_