#pragma once

#include "api/api.h"
#include <atomic>
#include <mutex>
#include <deque>

class Trade {
public:
  Trade();
  ~Trade();

  void StartTrading();
  void SwithcOff();
  
private:
  void save_collection(const evil::Item& item);

  std::atomic<bool> _enabled;
  std::mutex _collection_lock;
  std::deque<evil::Item> _buyed_collection;

  BlackMarket _market;
  Wallter _wallet;
};