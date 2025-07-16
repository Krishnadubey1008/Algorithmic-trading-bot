// src/PairsBacktester.cpp
#include "PairsBacktester.hpp"
#include "config.h"
#include <iostream>
#include <iomanip>

PairsBacktester::PairsBacktester(
    std::unique_ptr<PairsDataHandler> dh,
    std::unique_ptr<PairsTradingStrategy> s,
    std::unique_ptr<PairsExecutionHandler> eh,
    const std::string& sym_a,
    const std::string& sym_b,
    double initial_capital)
    : data_handler(std::move(dh)),
      strategy(std::move(s)),
      execution_handler(std::move(eh)),
      symbol_a(sym_a),
      symbol_b(sym_b),
      capital(initial_capital) {}

void PairsBacktester::run() {
    std::cout << "\n--- Starting Pairs Trading Backtest for " 
              << symbol_a << " / " << symbol_b << " ---" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    std::pair<Bar, Bar> current_pair;
    double pnl = 0.0;
    double entry_price_a = 0.0;
    double entry_price_b = 0.0;

    while (data_handler->get_next_bar_pair(current_pair)) {
        std::string signal = strategy->generate_signal(current_pair.first.close, current_pair.second.close);

        if (signal == "LONG_SPREAD" && portfolio_position == 0) {
            execution_handler->execute_pair_order(signal, symbol_a, symbol_b);
            portfolio_position = 1;
            entry_price_a = current_pair.first.close;
            entry_price_b = current_pair.second.close;
        } 
        else if (signal == "SHORT_SPREAD" && portfolio_position == 0) {
            execution_handler->execute_pair_order(signal, symbol_a, symbol_b);
            portfolio_position = -1;
            entry_price_a = current_pair.first.close;
            entry_price_b = current_pair.second.close;
        }
        else if (signal == "EXIT_SPREAD" && portfolio_position != 0) {
            execution_handler->execute_pair_order(signal, symbol_a, symbol_b);
            if(portfolio_position == 1) { // Was Long A, Short B
                pnl += (current_pair.first.close - entry_price_a) - (current_pair.second.close - entry_price_b);
            } else { // Was Short A, Long B
                pnl += (entry_price_a - current_pair.first.close) + (current_pair.second.close - entry_price_b);
            }
            portfolio_position = 0;
        }
    }
    std::cout << "\n--- Pairs Backtest Finished ---" << std::endl;
    std::cout << "Final Realized PnL: $" << pnl << std::endl;
    std::cout << "Final Portfolio Value: $" << capital + pnl << std::endl;
}