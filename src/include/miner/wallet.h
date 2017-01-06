#pragma once
#include "miner/mine_farm.h"

class Wallet {
public:
  Wallet();
  ~Wallet();

  float get_money(float summ);
  void cash_back(float money);
  inline void disable() { _enabled = false; };
  void collect_transh();

private:
  inline bool is_enabled() const {return _enabled; };

  std::atomic<bool> _enabled;
  const std::chrono::milliseconds kTranshPeriod;
  std::condition_variable _waiting_transh;
  // This isn't financial app, so will be enough just one double
  std::atomic<double> m_all_summ;

  MiningFarm m_miner;
};