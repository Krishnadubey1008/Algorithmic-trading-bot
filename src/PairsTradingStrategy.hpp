// src/PairsTradingStrategy.hpp
#pragma once
#include "DataHandler.hpp" // For Bar struct
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>

class PairsTradingStrategy { // No inheritance
public:
    PairsTradingStrategy(int lookback_window, double z_score_threshold)
        : lookback_window(lookback_window), z_score_threshold(z_score_threshold) {}

    std::string generate_signal(double price_A, double price_B) {
        // ... all the correct logic from the previous answer ...
        double current_spread = std::log(price_A) - std::log(price_B);
        spreads.push_back(current_spread);

        if (spreads.size() < lookback_window) return "HOLD";
        
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
    // The erroneous function with 'override' has been removed.

private:
    int lookback_window;
    double z_score_threshold;
    std::vector<double> spreads;
    int position = 0;
};