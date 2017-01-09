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

  inline bool is_enable() const { return m_enabled; };
  inline void disable();
  inline uint64_t last_execute_time() const { return m_last_execute_time; };

private:
  using Future = std::future<float>;
  void start_minning();
  void start_one();
  void collect_money();
  
  void add_future(Future& itm);
  float get_money_block();

  void summ(float money);

  std::mutex m_execute_lock;
  std::condition_variable m_execute_cv;
  
  std::mutex m_farm_lock;
  std::condition_variable m_farm_cv;
  std::deque<Future> m_farm;
  
  std::atomic<bool> m_enabled;
  std::atomic<uint64_t> m_last_execute_time;
  static const uint32_t kMinThreadCount = 10u;
  static constexpr uint64_t kExecuteLimit = 20000u; // 20 sec

  float m_mining_money;
  std::mutex m_mining_money_lock;

  std::unique_ptr<std::thread> m_collect_money_thread;
  std::unique_ptr<std::thread> m_mining_thread;
};