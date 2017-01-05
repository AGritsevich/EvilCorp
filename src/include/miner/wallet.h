#pragma once

class Wallet {
public:
  Wallet();
  ~Wallet();

  float get_money() throw;
  void cash_back(float money) throw;
  inline void disable() throw { _enabled = false; };
  void collect_transh() throw;

private:
  inline bool disabled() const throw {return _enabled; };

  std::atomic<bool> _enabled;
  static constexpr std::chrono::milliseconds kTranshPeriod(2000u);
  std::condition_variable _waiting_transh;
  // This isn't financial app, so will be enough just one double
  std::atomic<double> _all_summ;

  MiningFarm m_miner;
};