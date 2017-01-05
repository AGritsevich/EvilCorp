
#include "trade.h"
#include "black_market/black_market.h"
#include "miner/wallet.h"

Trade::Trade() :
  _enabled(true) {};

Trade::~Trade{};

void Trade::SwithcOff() {
  _enabled = false;
}

void Trade::save_collection(const evil::Item& item) {
  std::unique_lock<std::mutex>(_collection_lock);
  _buyed_collection.push_back(std::move(new_item));
}

Trade::StartTrading() {
  
  while (_enabled) {
    evil::Item new_item;
    bool result = _market.see_new_item(new_item); // Get next item

    if (false == result ) continue;

    const double reserved_money = _wallet.get_money(new_item.coin.cost);

    if (reserved_money) {
      if (_market.buy_item(new_item)) {
        save_collection(new_item)
      }
      else {
        _wallet.cash_back(reserved_money);
      }
    }
  }
}