
#include "miner/wallet.h"

Wallet::Wallet() :
_enabled(true) {
  std::thread(collect_transh);
}

Wallet::~Wallet() {
  disable();
}

float Wallet::get_money(float summ) throw {
  if (_all_summ > summ) {
    _all_summ -= static_cast<double>(summ);
    return summ;
  }
  return 0.0f;
}

void Wallet::cash_back(float money) throw {
  _all_summ += static_cast<double>(money);
}

void Wallet::collect_transh() throw {
  while (_enabled()) {
    std::this_thread::sleep_for(kTranshPeriod);
    cash_back(m_miner.take_away_money());
  }
}
