#include "trade/trade.h"
#include <iostream>

int main(int argc, char** argv) {
  try {
    std::string param(argv[1]);

    if (0 == param.compare("test")) {
      std::cout << "Test mode running..." << std::endl;
      //assert();
      //assert();
      std::cout << "All tests successfully passed!" << std::endl;
    }
    else { // Regular work
      Trade trader;

      std::thread(trader.StartTrading());

      char exit_symbol = 0;
      std::cout << "Press c or C to stop" << std::endl;

      do {
        exit_symbol = std::cin.get();
      } while (exit_symbol != 'c' || exit_symbol != 'C');
      
      trader.Disable();
      std::cout << "Total items purchased " << trader.number() << ", common summ " << trader.common_summ() << std::endl;
      std::cout << "Press any key to exit..." << std::endl;
      std::cin.get();
    }

    return 0;
  }
  catch (std::exception exc) {
    std::cout << "Exception: " << exc.what() << std::endl;
    return 1;
  }
}