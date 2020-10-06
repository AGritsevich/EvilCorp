#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <deque>
#include "api/api.h"
#include "black_market/black_market.h"
#include "miner/wallet.h"

class Trade {
public:
  Trade();
  ~Trade();

  void trading();
  void disable();
  inline bool is_enabled() const { return m_enabled; };
  size_t total_purchased();
  double common_summ();
  inline bool is_enable() const { return m_enabled; };
  
private:
  void save_collection(const evil::Item& item);

  std::atomic<bool> m_enabled;
  std::mutex m_collection_lock;
  std::deque<evil::Item> m_buyed_collection;

  BlackMarket m_market;
  Wallet m_wallet;
  std::unique_ptr<std::thread> m_trading_thread;
};