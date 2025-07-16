// src/PairsBacktester.hpp
#pragma once
#include "PairsDataHandler.hpp"
#include "PairsTradingStrategy.hpp"
#include "PairsExecutionHandler.hpp"
#include <memory>

class PairsBacktester {
public:
    PairsBacktester(
        std::unique_ptr<PairsDataHandler> data_handler,
        std::unique_ptr<PairsTradingStrategy> strategy,
        std::unique_ptr<PairsExecutionHandler> execution_handler,
        const std::string& symbol_a,
        const std::string& symbol_b,
        double initial_capital
    );
    void run();

private:
    std::unique_ptr<PairsDataHandler> data_handler;
    std::unique_ptr<PairsTradingStrategy> strategy;
    std::unique_ptr<PairsExecutionHandler> execution_handler;
    std::string symbol_a;
    std::string symbol_b;
    double capital;
    // Simple portfolio: 1 = long A/short B, -1 = short A/long B, 0 = flat
    int portfolio_position = 0;
};