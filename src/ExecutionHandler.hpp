// src/ExecutionHandler.hpp
#pragma once
#include <string>
#include <iostream>

// Abstract base class for handling order execution
class ExecutionHandler {
public:
    virtual ~ExecutionHandler() = default;
    virtual void execute_order(const std::string& symbol, const std::string& signal, int quantity) = 0;
};

// A simulated handler for backtesting. It prints orders instead of executing them.
class SimulatedExecutionHandler : public ExecutionHandler {
public:
    void execute_order(const std::string& symbol, const std::string& signal, int quantity) override {
        std::cout << "SIMULATED EXECUTION -> "
                  << signal << " " << quantity << " share(s) of " << symbol
                  << std::endl;
    }
};