
#include "miner/wallet.h"

Wallet::Wallet() :
  _enabled(true),
  kTranshPeriod(2000) {
  std::thread(collect_transh);
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
  m_all_summ = m_all_summ + static_cast<double>(money);
}

void Wallet::collect_transh() {
  while (is_enabled()) {
    std::this_thread::sleep_for(kTranshPeriod);
    cash_back(m_miner.take_away_money());
  }
}
