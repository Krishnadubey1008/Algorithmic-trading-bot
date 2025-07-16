#pragma once
#include "DataHandler.hpp"
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>

class PairsTradingStrategy {
public:
    PairsTradingStrategy(int lookback_window, double z_score_threshold)
        : lookback_window(lookback_window), z_score_threshold(z_score_threshold) {}

    std::string generate_signal(double price_A, double price_B) {
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

        // --- REVERSED MOMENTUM LOGIC ---
        if (z_score > z_score_threshold && position == 0) {
            position = 1; // Long the spread (BUY the breakout)
            return "LONG_SPREAD";
        } 
        else if (z_score < -z_score_threshold && position == 0) {
            position = -1; // Short the spread (SELL the breakdown)
            return "SHORT_SPREAD";
        }
        // Exit when the momentum fades (Z-score returns towards the mean)
        else if (std::abs(z_score) < 0.5 && position != 0) {
            position = 0; // Exit trade
            return "EXIT_SPREAD";
        }

        return "HOLD";
    }

private:
    int lookback_window;
    double z_score_threshold;
    std::vector<double> spreads;
    int position = 0;
};