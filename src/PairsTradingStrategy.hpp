// src/PairsTradingStrategy.hpp (Updated)
#pragma once
#include "DataHandler.hpp" // For Bar struct
#include <string>
#include <vector>
#include <numeric>
#include <cmath> // For std::log and std::sqrt
#include <stdexcept>

class PairsTradingStrategy {
public:
    PairsTradingStrategy(int lookback_window, double z_score_threshold)
        : lookback_window(lookback_window), z_score_threshold(z_score_threshold) {}

    std::string generate_signal(double price_A, double price_B) {
        
        // --- THE CRITICAL CHANGE IS HERE ---
        // Calculate the spread using the log of the prices, not the raw difference.
        double current_spread = std::log(price_A) - std::log(price_B);
        spreads.push_back(current_spread);

        if (spreads.size() < lookback_window) return "HOLD";

        // The rest of the logic remains the same, as it operates on the spread series
        auto first = spreads.end() - lookback_window;
        auto last = spreads.end();
        
        double spread_mean = std::accumulate(first, last, 0.0) / lookback_window;
        double sq_sum = 0.0;
        for(auto it = first; it != last; ++it) {
            sq_sum += (*it - spread_mean) * (*it - spread_mean);
        }
        double spread_std_dev = std::sqrt(sq_sum / lookback_window);

        if (spread_std_dev == 0.0) return "HOLD";

        double z_score = (current_spread - spread_mean) / spread_std_dev;

        if (z_score > z_score_threshold && position == 0) {
            position = -1;
            return "SHORT_SPREAD";
        } 
        else if (z_score < -z_score_threshold && position == 0) {
            position = 1;
            return "LONG_SPREAD";
        }
        else if (std::abs(z_score) < 0.5 && position != 0) {
            position = 0;
            return "EXIT_SPREAD";
        }

        return "HOLD";
    }

    std::string generate_signal(const Bar& new_bar) override {
        throw std::logic_error("PairsTradingStrategy requires data for two assets.");
        return "ERROR";
    }

private:
    int lookback_window;
    double z_score_threshold;
    std::vector<double> spreads;
    int position = 0; // 0=flat, 1=long spread, -1=short spread
};