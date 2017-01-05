#pragma once

#include <mutex>
#include <atomic>
#include <future>

class MiningFarm : public IBTCMiner throw {
public:
  MiningFarm();
  ~MiningFarm() override;

  float take_away_money() throw;

private:
  using Future = std::future<ReturnFarm>;
  void disable() throw;
  void StartMinning() throw;
  void StartOne() throw;
  void CollectMoney() throw;

  inline bool enabled() const throw { return _enabled; };

  void add_future(Future itm) throw;
  Future future() throw;

  void summ(float money) throw;

  std::mutex _execute_lock;
  std::condition_variable _execute_cv;
  
  std::mutex _farm_lock;
  std::deque<Future> _farm;
  
  std::atomic<bool> _enabled;
  std::atomic<uint64_t> _last_execute_time;
  static constexpr uint64_t kExecuteLimit = 10000u; // 10 sec

  float _mining_money;
  std::mutex _mining_money_lock;

  struct ReturnFarm{
    float summ;
    uint64_t durration;
    ReturnFarm() :
      summ(0),
      durration(0) {}
  };
};