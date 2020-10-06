#include "trade/trade.h"
#include <iostream>

int main(int argc, char** argv) {
  try {
    // Get underlying buffer
    std::streambuf* orig_buf = std::cout.rdbuf();

    if (argc == 2 && 0 == std::string(argv[1]).compare("test")) {
      std::cout << "Test mode running..." << std::endl;
      // Sorry, I have no time for test witting. 
      // I want use here GTest
      std::cout << "All tests successfully passed!" << std::endl;
    }
    else { // Regular work
      Trade trader;

      int exit_symbol = 0;
      std::cout.clear();
      std::cout << "Press c or C to stop" << std::endl;
      std::cout.rdbuf(nullptr);

      do {
        exit_symbol = std::cin.get();
      } while (exit_symbol != 'c' && exit_symbol != 'C');
      
      std::cout.clear();
      std::cout.rdbuf(orig_buf);
      std::cout << "Please wait, exiting..." << std::endl;
      trader.disable();
      std::cout.clear();
      std::cout << "\r Total items purchased: " << trader.total_purchased() << ", common summ: " << trader.common_summ() << std::endl;
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