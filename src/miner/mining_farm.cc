
#include "miner/mine_farm.h"

MiningFarm::MiningFarm() :
  m_enabled(true){

  std::thread(collect_money);
  std::thread(start_minning);
}

MiningFarm::~MiningFarm() {
  disable();
}

float MiningFarm::take_away_money() {
  std::unique_lock<std::mutex>(_mining_money_lock);
  auto cash_away = _mining_money;
  _mining_money = 0;
  return cash_away;
}

void MiningFarm::summ(float money) {
  std::unique_lock<std::mutex>(_mining_money_lock);
  _mining_money += money;
}

void MiningFarm::disable() {
  m_enabled = false;
  m_execute_cv.notify_all();
}

void MiningFarm::add_future(Future& itm) {
  std::unique_lock<std::mutex>(_farm_lock);
  m_farm.emplace_back(std::move(itm));
}

MiningFarm::Future MiningFarm::future() {
  std::unique_lock<std::mutex>(_farm_lock);
  Future itm = std::move(m_farm.front());
  m_farm.pop_front();
  itm.wait();
  return itm;
}

void MiningFarm::start_one() {
  Future ret = std::async(std::launch::async, [&]()->float {
    namespace sc = std::chrono;
    auto before = sc::system_clock::now();
    float summ = this->get_btc();
    auto after = sc::system_clock::now();

    m_last_execute_time = sc::duration_cast<sc::milliseconds>(after - before).count();
    return summ;
  });
  add_future(ret);
}

void MiningFarm::collect_money() {
  while (enabled()) {
    auto ret_type = future();
    summ(ret_type.get());
    m_execute_cv.notify_one();
  }
}

void MiningFarm::start_minning() {
  while (enabled()) {
    std::unique_lock<std::mutex>(m_execute_lock);
    while (enabled() && m_last_execute_time >  kExecuteLimit) {
      m_execute_cv.wait(m_execute_lock);
    }
    start_one();
  }
}

