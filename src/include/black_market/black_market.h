#pragma once

#include "lib_proxy/lib_proxy.h"
#include <mutex>
#include <deque>
#include <memory>
#include <condition_variable>

class BlackMarket {
public:
  BlackMarket();
  ~BlackMarket();

  bool see_new_item(evil::Item& item) throw;
  bool buy_item(const evil::Item& item) throw;

private:
  void asynch_get_message(const byte* message, size_t size) throw;
  void add_new_item(const evil::Item& item) const throw;

  std::unique_ptr<LibProxy> _lib_helper;

  std::mutex _asynch_func_lock;
  std::mutex _market_collection_lock;
  std::condition_variable _market_collection_cv;
  std::deque<evil::Item> _market_collection;
  uint32_t _last_viewed;

  evil::api *_rare_items;
};