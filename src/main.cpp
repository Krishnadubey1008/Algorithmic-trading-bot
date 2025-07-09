// src/main.cpp
#include "Backtester.hpp"
#include "APIDataHandler.cpp" // Included for simple, single-file compilation
#include "Strategy.hpp"
#include "ExecutionHandler.hpp"
#include "config.h"
#include <iostream>
#include <memory>

int main() {
    try {
        // 1. Create the components using smart pointers
        auto data_handler = std::make_unique<APIDataHandler>(config::SYMBOL);
        auto strategy = std::make_unique<MovingAverageCrossover>(config::SHORT_WINDOW, config::LONG_WINDOW);
        auto execution_handler = std::make_unique<SimulatedExecutionHandler>();
        
        // 2. Create and run the backtester
        Backtester backtester(
            std::move(data_handler),
            std::move(strategy),
            std::move(execution_handler),
            config::SYMBOL,
            config::INITIAL_CAPITAL
        );
        
        backtester.run();

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}