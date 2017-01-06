
#include "trade/trade.h"
#include "black_market/black_market.h"
#include "miner/wallet.h"

Trade::Trade() :
  _enabled(true) {};

Trade::~Trade(){};

void Trade::disable() {
  _enabled = false;
  m_wallet.disable();
}

size_t Trade::total_purchased() {
  std::unique_lock<std::mutex>(_collection_lock);
  return _buyed_collection.size();
}

double Trade::common_summ() {
  std::unique_lock<std::mutex>(_collection_lock);
  double summ = 0.0f;

  for (auto& itm:_buyed_collection) {
    summ += itm.thing.coin.cost;
  }

  return summ;
}

void Trade::save_collection(const evil::Item& item) {
  std::unique_lock<std::mutex>(_collection_lock);
  _buyed_collection.push_back(std::move(item));
}

void Trade::start_trading() {  
  std::thread([&] {
    while (_enabled) {
      // Get next item or if list empty wait until not empty
      const evil::Item new_item = m_market.see_new_item();
      const float reserved_money = m_wallet.get_money(new_item.thing.coin.cost);

      if (reserved_money) {
        if (m_market.buy_item(new_item)) {
          save_collection(new_item);
        } else {
          m_wallet.cash_back(reserved_money);
        }
      }
    }
  });
}