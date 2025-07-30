// src/PairsBacktester.cpp
#include "PairsBacktester.hpp"
#include "config.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <cmath>

// Forward-declare the helper functions so `run()` can see them
std::vector<double> calculate_daily_returns(const std::vector<double>& values);
double calculate_sharpe_ratio(const std::vector<double>& portfolio_values, double risk_free_rate = 0.0);
double calculate_max_drawdown(const std::vector<double>& values);


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

    std::vector<double> portfolio_values;
    portfolio_values.push_back(capital);

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
            entry_price_a = 0.0; // Reset entry prices
            entry_price_b = 0.0;
        }
        
        // Calculate and store daily portfolio value
        double current_market_value = 0.0;
        if (portfolio_position == 1) { // Long A, Short B
            current_market_value = (current_pair.first.close - entry_price_a) - (current_pair.second.close - entry_price_b);
        } else if (portfolio_position == -1) { // Short A, Long B
            current_market_value = (entry_price_a - current_pair.first.close) + (current_pair.second.close - entry_price_b);
        }
        portfolio_values.push_back(capital + pnl + current_market_value);
    }

    std::cout << "\n--- Pairs Backtest Finished ---" << std::endl;
    std::cout << "Final Realized PnL: $" << pnl << std::endl;
    std::cout << "Final Portfolio Value: $" << portfolio_values.back() << std::endl;

    if (portfolio_values.size() > 1) {
        double sharpe = calculate_sharpe_ratio(portfolio_values);
        double mdd = calculate_max_drawdown(portfolio_values);
        std::cout << "\n--- Performance Metrics ---" << std::endl;
        std::cout << "Sharpe Ratio (annualized): " << sharpe << std::endl;
        std::cout << "Max Drawdown: " << (mdd * 100.0) << "%" << std::endl;
    }
}


// --- Performance Metric Helper Functions ---

std::vector<double> calculate_daily_returns(const std::vector<double>& values) {
    std::vector<double> returns;
    for (size_t i = 1; i < values.size(); ++i) {
        if (values[i-1] != 0) {
            returns.push_back((values[i] / values[i-1]) - 1.0);
        } else {
            returns.push_back(0.0);
        }
    }
    return returns;
}

double calculate_sharpe_ratio(const std::vector<double>& portfolio_values, double risk_free_rate) {
    auto returns = calculate_daily_returns(portfolio_values);
    if (returns.size() < 2) return 0.0;

    double sum = std::accumulate(returns.begin(), returns.end(), 0.0);
    double mean = sum / returns.size();

    double sq_sum = 0.0;
    for (const auto& r : returns) {
        sq_sum += (r - mean) * (r - mean);
    }
    double std_dev = std::sqrt(sq_sum / (returns.size() - 1));

    if (std_dev == 0.0) return 0.0;
    
    return (mean - risk_free_rate) / std_dev * std::sqrt(252);
}

double calculate_max_drawdown(const std::vector<double>& values) {
    if (values.empty()) return 0.0;
    double peak = values[0];
    double max_drawdown = 0.0;

    for (const auto& value : values) {
        if (value > peak) {
            peak = value;
        }
        double drawdown = (peak - value) / peak;
        if (drawdown > max_drawdown) {
            max_drawdown = drawdown;
        }
    }
    return max_drawdown;
}