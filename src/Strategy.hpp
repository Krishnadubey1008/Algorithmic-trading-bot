// src/Strategy.hpp
#pragma once
#include "DataHandler.hpp"
#include <string>
#include <vector>
#include <numeric>

// Abstract base class for all strategies
class Strategy {
public:
    virtual ~Strategy() = default;
    virtual std::string generate_signal(const Bar& new_bar) = 0;
};

// Concrete implementation of a Moving Average Crossover strategy
class MovingAverageCrossover : public Strategy {
public:
    MovingAverageCrossover(int short_window, int long_window)
        : short_window(short_window), long_window(long_window) {}

    std::string generate_signal(const Bar& new_bar) override {
        prices.push_back(new_bar.close);

        if (prices.size() < long_window) {
            return "HOLD"; // Not enough data
        }

        double short_sma = calculate_sma(short_window);
        double long_sma = calculate_sma(long_window);

        if (short_sma > long_sma && last_signal != "BUY") {
            last_signal = "BUY";
            return "BUY";
        } else if (short_sma < long_sma && last_signal != "SELL") {
            last_signal = "SELL";
            return "SELL";
        }
        return "HOLD";
    }

private:
    double calculate_sma(int window) const {
        if (prices.size() < window) return 0.0;
        double sum = std::accumulate(prices.end() - window, prices.end(), 0.0);
        return sum / window;
    }

    int short_window;
    int long_window;
    std::vector<double> prices;
    std::string last_signal = "HOLD";
};