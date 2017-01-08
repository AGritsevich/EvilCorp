
#include "miner/wallet.h"

Wallet::Wallet() :
  m_enabled(true),
  m_all_summ(0.0f),
  kTranshPeriod(5000) {
  m_collect_transh_thread.reset(new std::thread(&Wallet::collect_transh, this));
  //m_collect_transh_thread->detach();
}

Wallet::~Wallet() {
  disable();
}

float Wallet::get_money(float summ) {
  if (m_all_summ > summ) {
    m_all_summ = m_all_summ - static_cast<double>(summ);
    return summ;
  }
  return 0.0f;
}

void Wallet::cash_back(float money) {
  if (!money) return;
  m_all_summ = m_all_summ + static_cast<double>(money);
}

void Wallet::disable() {
  m_enabled = false;
  m_miner.disable();
  m_waiting_transh_cv.notify_all();
  if (m_collect_transh_thread->joinable()) {
    m_collect_transh_thread->join();
  }
}

void Wallet::collect_transh() {
  while (is_enabled()) {
    std::unique_lock<std::mutex> lck(m_transh_lock);
    const std::chrono::milliseconds timeout(m_miner.last_execute_time() ? m_miner.last_execute_time() : kTranshPeriod);
    do {} 
    while (is_enabled() &&
      m_waiting_transh_cv.wait_for(lck, timeout) != std::cv_status::timeout);
    cash_back(m_miner.take_away_money());
  }
}
