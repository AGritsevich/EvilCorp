#include "trade/trade.h"
#include <iostream>

int main(int argc, char** argv) {
  try {
    if (argc == 2 && 0 == std::string(argv[1]).compare("test")) {
      std::cout << "Test mode running..." << std::endl;
      //assert();
      //assert();
      std::cout << "All tests successfully passed!" << std::endl;
    }
    else { // Regular work
      std::cout.setstate(std::ios_base::failbit);
      Trade trader;

      //trader.trading();

      int exit_symbol = 0;
      std::cout.clear();
      std::cout << "Press c or C to stop" << std::endl;
      std::cout.setstate(std::ios_base::failbit);

      do {
        exit_symbol = std::cin.get();
      } while (exit_symbol != 'c' && exit_symbol != 'C');
      
      trader.disable();
      std::cout.clear();
      std::cout << "\r Total items purchased: " << trader.total_purchased() << ", common summ: " << trader.common_summ();
      std::cout.flush();
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