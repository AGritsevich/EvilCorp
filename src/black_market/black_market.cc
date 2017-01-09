
#include "black_market/black_market.h"
#include <algorithm>
#ifdef __linux__
#include "lib_proxy/linux_lib.h"
const char kApiLib[] = "api.so";
#else // WIN32
#include "lib_proxy/win_lib.h"
const char kApiLib[] = "api.dll";
#endif /* WIN32 */
#include <future>

namespace service_fun {
  static BlackMarket *g_ptr_market = nullptr;

  void call_back(const byte * message, const size_t size) {
    std::unique_lock<std::mutex> lock(g_ptr_market->m_asynch_func_lock);
    const size_t kMinSize = (sizeof(evil::RareType) + sizeof(evil::Coin));
    if (g_ptr_market && message && size > kMinSize) {
      if (!g_ptr_market->is_enabled()) return;
 
      evil::Item new_item;
      memcpy_s(&new_item, sizeof(new_item), message, size);

      g_ptr_market->add_new_item(new_item);
    }
    lock.unlock();
    g_ptr_market->update_callback();
  }
} // namespace service_fun

BlackMarket::BlackMarket() :
  m_enabled(true),
#ifdef __linux__
  m_lib_helper(new LinuxLib), 
#else // WIN32
  m_lib_helper(new WinLib),
#endif /* WIN32 */
  m_last_viewed(0),
  _rare_items(nullptr) {

  service_fun::g_ptr_market = this;

  if (!m_lib_helper->Open(kApiLib)) {
    throw std::ios_base::failure("Can't open library");
  }

  if (nullptr == 
    (_rare_items = reinterpret_cast<evil::api*>
                                (m_lib_helper->LibraryFunction("api")))) {
    throw std::ios_base::failure("Can't load class");
  }

  m_call_back_thread.reset(
                  new std::thread(service_fun::call_back, nullptr, 0));
}

BlackMarket::~BlackMarket() {
  disable();
}

const evil::Item& BlackMarket::see_new_item() {
  std::unique_lock<std::mutex> lock(m_market_collection_lock);
 
  while (is_enabled() && m_market_collection.size() == 0) { 
    m_market_collection_cv.wait(lock); 
  };

  if (m_last_viewed >= m_market_collection.size()) {
    m_last_viewed = 0;
  }

  std::deque<evil::Item>::const_iterator 
                            it(m_market_collection.begin());

  std::advance(it, m_last_viewed++);
  return *it;
}

bool BlackMarket::buy_item(const evil::Item& bought) {
  std::unique_lock<std::mutex> lock(m_market_collection_lock);
  try {

    std::deque<evil::Item>::iterator it = std::find_if(
      m_market_collection.begin(), m_market_collection.end(), 
      [&](evil::Item &itm) { return itm == bought;}
    );

    m_market_collection.erase(it);

    return true;
  }
  catch (...) {
    // Probably already bought
    return false;
  }
}

void BlackMarket::disable() {
  m_enabled = false;
  m_market_collection_cv.notify_all();
  if (m_call_back_thread->joinable()) {
    m_call_back_thread->join();
  }
}

void BlackMarket::add_new_item(const evil::Item& item) {
  std::unique_lock<std::mutex> lock(m_market_collection_lock);
  m_market_collection.emplace_back(std::move(item));
  lock.unlock();
  m_market_collection_cv.notify_one();
}
