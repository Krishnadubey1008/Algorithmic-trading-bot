// src/main_pairs.cpp
#include "PairsBacktester.hpp"
#include "PairsDataHandler.hpp"
#include "PairsTradingStrategy.hpp"
#include "PairsExecutionHandler.hpp"
#include "config.h"
#include <iostream>
#include <memory>

int main() {
    try {
        auto data_handler = std::make_unique<PairsDataHandler>(config::SYMBOL_A, config::SYMBOL_B);
        auto strategy = std::make_unique<PairsTradingStrategy>(config::LOOKBACK_WINDOW, config::Z_SCORE_THRESHOLD);
        auto execution_handler = std::make_unique<SimulatedPairsExecutionHandler>();
        
        PairsBacktester backtester(
            std::move(data_handler),
            std::move(strategy),
            std::move(execution_handler),
            config::SYMBOL_A,
            config::SYMBOL_B,
            config::INITIAL_CAPITAL
        );
        
        backtester.run();

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}