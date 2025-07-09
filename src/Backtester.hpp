// src/Backtester.hpp
#pragma once
#include "DataHandler.hpp"
#include "Strategy.hpp"
#include "ExecutionHandler.hpp"
#include <memory>
#include <string>

class Backtester {
public:
    Backtester(
        std::unique_ptr<DataHandler> data_handler,
        std::unique_ptr<Strategy> strategy,
        std::unique_ptr<ExecutionHandler> execution_handler,
        const std::string& symbol,
        double initial_capital
    );
    void run();

private:
    std::unique_ptr<DataHandler> data_handler;
    std::unique_ptr<Strategy> strategy;
    std::unique_ptr<ExecutionHandler> execution_handler;
    std::string symbol;
    double capital;
    int position;
};