#pragma once

#include <mutex>
#include <atomic>
#include <future>
#include "miner/btc_miner.h"
#include <deque>

class MiningFarm : public IBTCMiner {
public:
  MiningFarm();
  virtual ~MiningFarm();

  float take_away_money();

private:
  using Future = std::future<float>;
  void disable();
  void start_minning();
  void start_one();
  void collect_money();

  inline bool enabled() const { return m_enabled; };

  void add_future(Future& itm);
  Future future();

  void summ(float money);

  std::mutex m_execute_lock;
  std::condition_variable m_execute_cv;
  
  std::mutex _farm_lock;
  std::deque<Future> m_farm;
  
  std::atomic<bool> m_enabled;
  std::atomic<uint64_t> m_last_execute_time;
  static constexpr uint64_t kExecuteLimit = 10000u; // 10 sec

  float _mining_money;
  std::mutex _mining_money_lock;
};