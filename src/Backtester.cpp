// src/Backtester.cpp
#include "Backtester.hpp"
#include "config.h"
#include <iostream>
#include <iomanip>

Backtester::Backtester(
    std::unique_ptr<DataHandler> dh,
    std::unique_ptr<Strategy> s,
    std::unique_ptr<ExecutionHandler> eh,
    const std::string& sym,
    double initial_capital)
    : data_handler(std::move(dh)),
      strategy(std::move(s)),
      execution_handler(std::move(eh)),
      symbol(sym),
      capital(initial_capital),
      position(0) {}

void Backtester::run() {
    std::cout << "\n--- Starting Backtest for " << symbol << " ---" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Initial Capital: $" << capital << std::endl;

    Bar current_bar;
    double last_price = 0.0;
    while (data_handler->get_next_bar(current_bar)) {
        last_price = current_bar.close;
        std::string signal = strategy->generate_signal(current_bar);

        // Simple position sizing: 1 share at a time
        if (signal == "BUY" && position == 0) {
            execution_handler->execute_order(symbol, "BUY", 1);
            position += 1;
            capital -= current_bar.close; // Subtract cost of share
        } else if (signal == "SELL" && position > 0) {
            execution_handler->execute_order(symbol, "SELL", 1);
            position -= 1;
            capital += current_bar.close; // Add proceeds from sale
        }
    }

    double final_value = capital + (position * last_price);
    double profit_loss = final_value - config::INITIAL_CAPITAL;

    std::cout << "\n--- Backtest Finished ---" << std::endl;
    std::cout << "Final Capital: $" << capital << std::endl;
    std::cout << "Holding: " << position << " share(s) of " << symbol << " at $" << last_price << std::endl;
    std::cout << "Final Portfolio Value: $" << final_value << std::endl;
    std::cout << "Total Profit/Loss: $" << profit_loss << std::endl;
}