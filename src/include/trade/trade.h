#pragma once

#include "api/api.h"
#include "miner/wallet.h"
#include "black_market/black_market.h"
#include <atomic>
#include <mutex>
#include <deque>

class Trade {
public:
  Trade();
  ~Trade();

  void start_trading();
  void disable();
  size_t total_purchased();
  double common_summ();
  
private:
  void save_collection(const evil::Item& item);

  std::atomic<bool> _enabled;
  std::mutex _collection_lock;
  std::deque<evil::Item> _buyed_collection;

  BlackMarket m_market;
  Wallet m_wallet;
};