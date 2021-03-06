
#include "miner/mine_farm.h"

MiningFarm::MiningFarm() :
  m_enabled(true),
  m_mining_money(0),
  m_last_execute_time(0) {

  m_collect_money_thread.reset(new std::thread(&MiningFarm::collect_money, this));
  m_mining_thread.reset(new std::thread(&MiningFarm::start_minning, this));
}

MiningFarm::~MiningFarm() {
  disable();
}

float MiningFarm::take_away_money() {
  std::unique_lock<std::mutex> lock(m_mining_money_lock);
  auto cash_away = m_mining_money;
  m_mining_money = 0;
  return cash_away;
}

void MiningFarm::summ(float money) {
  std::unique_lock<std::mutex> lock(m_mining_money_lock);
  m_mining_money += money;
}

inline void MiningFarm::disable() {
  m_enabled = false;
  m_execute_cv.notify_all();
  m_farm_cv.notify_all();
  if (m_collect_money_thread->joinable()) {
    m_collect_money_thread->join();
  }
  if (m_mining_thread->joinable()) {
    m_mining_thread->join();
  }
}

void MiningFarm::add_future(Future& itm) {
  std::unique_lock<std::mutex> lock(m_farm_lock);
  m_farm.emplace_back(std::move(itm));
  m_farm_cv.notify_one();
}

float MiningFarm::get_money_block() {
  std::unique_lock<std::mutex> lock(m_farm_lock);
  while (is_enable() && m_farm.size() < kMinThreadCount) {
    m_farm_cv.wait(lock);
  }
  float ret_money = m_farm.front().get();
  m_farm.pop_front();
  return ret_money;
}

void MiningFarm::start_one() {
  if (!is_enable()) return;
  
  Future ret = std::async(std::launch::async, [this]()->float {
    namespace sc = std::chrono;
    auto before = sc::system_clock::now();
    float summ = get_btc();
    auto after = sc::system_clock::now();

    m_last_execute_time = sc::duration_cast<sc::milliseconds>(after - before).count();
    return summ;
  });
  add_future(ret);
}

void MiningFarm::collect_money() {
  while (is_enable()) {
    summ(get_money_block());
    m_execute_cv.notify_one();
  }
}

void MiningFarm::start_minning() {
  while (is_enable()) {
    std::unique_lock<std::mutex> lock(m_execute_lock);
    for (auto i = 0; i < kMinThreadCount; i++) {
      start_one();
    }
    while (is_enable() && m_last_execute_time >  kExecuteLimit) {
      m_execute_cv.wait(lock);
    }
    start_one();
  }
}

