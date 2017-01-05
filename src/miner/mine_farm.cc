
#include "miner/mine_farm.h"

MiningFarm::MiningFarm() :
  _enabled(true){

  std::thread(CollectMoney);
  std::thread(StartMinning);
}

MiningFarm::~MiningFarm() {
  disable();
}

float MiningFarm::take_away_money() throw {
  std::unique_lock<std::mutex>(_mining_money_lock);
  auto cash_away = _mining_money;
  _mining_money = 0;
  return cash_away;
}

void MiningFarm::summ(float money) throw {
  std::unique_lock<std::mutex>(_mining_money_lock);
  _mining_money += money;
}

void MiningFarm::disable() {
  _enabled = false;
  _execute_cv.notify_all();
}

void MiningFarm::add_future(Future itm) throw {
  std::unique_lock<std::mutex>(_farm_lock);
  _farm.emplace_back(std::move(itm));
}

MiningFarm::Future MiningFarm::future() throw {
  std::unique_lock<std::mutex>(_farm_lock);
  Future itm = _farm.pop_front();
  itm.wait();
  return itm;
}

void MiningFarm::StartOne() throw {
  auto ret = std::async(std::launch::async, []() {
    namespace sc = std::chrono;
    Future itm;
    auto before = sc::system_clock::now();
    itm.get.summ = get_btc();
    auto after = sc::system_clock::now();

    itm.get().durration = sc::duration_cast<sc::milliseconds>(after - before).count();
    return itm;
  });
  _last_execute_time = itm.get().durration();
  add_future(itm);
}

void MiningFarm::CollectMoney() throw {
  while (enabled()) {
    auto ret_type = future();
    summ(ret_type.get().summ);
  }
}

void MiningFarm::StartMinning() throw {

  while (enabled() {
    std::unique_lock<std::mutex>)(_execute_lock);
    while (enabled() && _last_execute_time >  kExecuteLimit) {
      _execute_cv.wait(_execute_lock);
    }
    StartOne();
  }
}

