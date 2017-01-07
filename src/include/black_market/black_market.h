#pragma once

#include <mutex>
#include <deque>
#include <memory>
#include <condition_variable>
#include <atomic>
#include "lib_proxy/lib_proxy.h"
#include "api/api.h"

namespace service_fun {
  void call_back(const byte* message, const size_t size);
} // service_fun

class BlackMarket {
  friend void service_fun::call_back(const byte* message, const size_t size);
public:
  BlackMarket();
  ~BlackMarket();

  const evil::Item & see_new_item();
  bool buy_item(const evil::Item& item);
  inline void disable();
  inline bool is_enabled() const noexcept { return m_enabled; };

private:
  inline void update_callback() { _rare_items->get_raw_rare(service_fun::call_back); };
  void add_new_item(const evil::Item& item);

  std::atomic<bool> m_enabled;
  std::unique_ptr<LibProxy> m_lib_helper;

  std::mutex m_asynch_func_lock;
  std::mutex m_market_collection_lock;
  std::condition_variable m_market_collection_cv;
  std::deque<evil::Item> m_market_collection;
  uint32_t m_last_viewed;

  evil::api *_rare_items;

  std::unique_ptr<std::thread> m_call_back_thread;
};