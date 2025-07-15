// src/HybridStrategy.hpp
#pragma once
#include "Strategy.hpp"
#include <numeric>

class HybridStrategy : public Strategy {
public:
    HybridStrategy(int short_window, int long_window, int rsi_period, int volume_window)
        : short_window(short_window), long_window(long_window), 
          rsi_period(rsi_period), volume_window(volume_window) {}

    std::string generate_signal(const Bar& new_bar) override {
        // Store prices and volumes for SMA calculations
        prices.push_back(new_bar.close);
        volumes.push_back(new_bar.volume);

        // Don't do anything until we have enough data
        if (prices.size() < long_window || prices.size() < volume_window) {
            return "HOLD";
        }

        // --- 1. Trend Indicators ---
        double short_sma = calculate_sma(prices, short_window);
        double long_sma = calculate_sma(prices, long_window);

        // --- 2. Momentum Indicator ---
        double rsi = calculate_rsi(new_bar.close);

        // --- 3. Volume Indicator ---
        double volume_sma = calculate_sma(volumes, volume_window);

        // Determine trend state based on crossover
        bool is_golden_cross = (short_sma > long_sma);
        bool was_death_cross = (last_short_sma <= last_long_sma && last_short_sma != 0.0);

        // --- Signal Logic ---
        // BUY Condition: A new golden cross occurred, RSI is not overbought, and volume is strong
        if (is_golden_cross && was_death_cross && rsi < 70.0 && new_bar.volume > volume_sma) {
             last_signal = "BUY";
             return "BUY";
        } 
        // SELL Condition: A death cross occurred OR RSI is overbought (profit-taking)
        else if ((!is_golden_cross || rsi > 70.0) && last_signal == "BUY") {
            last_signal = "SELL";
            return "SELL";
        }

        // Update last known SMA values for crossover detection on the next bar
        last_short_sma = short_sma;
        last_long_sma = long_sma;

        return "HOLD";
    }

private:
    // Generic SMA calculator for price or volume
    template<typename T>
    double calculate_sma(const std::vector<T>& data, int window) const {
        if (data.size() < window) return 0.0;
        double sum = std::accumulate(data.end() - window, data.end(), 0.0);
        return sum / window;
    }

    // RSI calculation
    double calculate_rsi(double current_price) {
        if (prices.size() < rsi_period + 1) return 50.0; // Return neutral RSI if not enough data

        double price_change = current_price - prices[prices.size() - 2];
        
        if (price_change > 0) {
            avg_gain = ((avg_gain * (rsi_period - 1)) + price_change) / rsi_period;
            avg_loss = (avg_loss * (rsi_period - 1)) / rsi_period;
        } else {
            avg_loss = ((avg_loss * (rsi_period - 1)) - price_change) / rsi_period;
            avg_gain = (avg_gain * (rsi_period - 1)) / rsi_period;
        }

        if (avg_loss == 0) return 100.0; // Avoid division by zero

        double rs = avg_gain / avg_loss;
        return 100.0 - (100.0 / (1.0 + rs));
    }

    // Strategy parameters
    int short_window;
    int long_window;
    int rsi_period;
    int volume_window;

    // State variables
    std::vector<double> prices;
    std::vector<double> volumes;
    std::string last_signal = "HOLD";
    double last_short_sma = 0.0;
    double last_long_sma = 0.0;
    double avg_gain = 0.0;
    double avg_loss = 0.0;
};