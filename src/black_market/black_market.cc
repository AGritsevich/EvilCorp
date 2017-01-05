
#include "black_market.h"
#include <algorithm>

BlackMarket::BlackMarket() :
#ifdef __linux__
  _lib_helper(LinuxLib), 
#else // WIN32
  _lib_helper(WinLib),
#endif /* WIN32 */
  _last_viewed(0),
  _rare_items(nullptr) {
  if (!_lib_helper->Open(API_LIB)) {
    throw std::exception::runtime_error::ios_base::failure("Can't open library");
  }

  if (nullptr == (_rare_items = dynamic_cast<evil::api*>(_lib_helper->LibraryFunction("api")))) {
    throw std::exception::runtime_error::ios_base::failure("Can't load class");
  }

  _rare_items->get_raw_rare(&BlackMarket::asynch_get_message);
}

BlackMarket::~BlackMarket() {

}

bool BlackMarket::see_new_item(evil::Item& item) throw {
  std::unique_lock<std::mutex> lock(_market_collection_lock);
 
  while (_market_collection.size() == 0) { _market_collection_cv.wait(lock); };

  if (_last_viewed > _market_collection.size()) {
    _last_viewed = 0;
  }

  std::deque<evil::Item>::const_iterator it(_market_collection.begin());

  std::advance(it, _last_viewed++);
  item = *it;
}

bool BlackMarket::buy_item(const evil::Item& item) throw {
  try {
    _market_collection.erase(item);

    return true;
  }
  catch (...) {

    return false;
  }
}

void BlackMarket::add_new_item(const evil::Item& item) const throw {
  std::unique_lock<std::mutex> lock(_market_collection_lock);
  _market_collection.emplace_back(std::move(item));
  lock.unlock();
  _market_collection_cv.notify_one();
}

void BlackMarket::asynch_get_message(const byte* message, size_t size) {
  std::unique_lock<std::mutex>(_asynch_func_lock);
  api::Item *new_item = dynamic_cast<evil::Item*>(message);

  add_new_item(*new_item);
}

