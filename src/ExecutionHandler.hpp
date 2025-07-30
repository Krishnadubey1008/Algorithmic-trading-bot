// src/PairsExecutionHandler.hpp
#pragma once
#include <string>
#include <iostream>

class PairsExecutionHandler {
public:
    virtual ~PairsExecutionHandler() = default;
    virtual void execute_pair_order(const std::string& signal, const std::string& symbol_A, const std::string& symbol_B) = 0;
};

class SimulatedPairsExecutionHandler : public PairsExecutionHandler {
public:
    void execute_pair_order(const std::string& signal, const std::string& symbol_A, const std::string& symbol_B) override {
        if (signal == "SHORT_SPREAD") {
            std::cout << "SIMULATED EXECUTION -> SHORT " << symbol_A << " AND LONG " << symbol_B << std::endl;
        } else if (signal == "LONG_SPREAD") {
            std::cout << "SIMULATED EXECUTION -> LONG " << symbol_A << " AND SHORT " << symbol_B << std::endl;
        } else if (signal == "EXIT_SPREAD") {
            std::cout << "SIMULATED EXECUTION -> EXIT SPREAD position in " << symbol_A << " and " << symbol_B << std::endl;
        }
    }
};