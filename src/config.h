// src/config.h
#pragma once
#include <string>

// WARNING: Do not commit API keys to a public repository.
namespace config {
    // --- API Keys ---
    const std::string ALPHA_VANTAGE_API_KEY = "YOUR_ALPHA_VANTAGE_API_KEY";
    const std::string ALPACA_API_KEY        = "YOUR_ALPACA_API_KEY";
    const std::string ALPACA_SECRET_KEY     = "YOUR_ALPACA_SECRET_KEY";

    // --- Single-Asset Strategy Parameters (for HybridStrategy) ---
    const std::string SYMBOL = "AAPL";
    const int SHORT_WINDOW   = 10;
    const int LONG_WINDOW    = 30;
    const double INITIAL_CAPITAL = 10000.0;
    
    // --- Pairs Trading Strategy Parameters (NEW) ---
    const std::string SYMBOL_A = "GOOGL"; // e.g., Google
    const std::string SYMBOL_B = "MSFT";  // e.g., Microsoft
    const int LOOKBACK_WINDOW  = 30;     // Lookback period for spread stats
    const double Z_SCORE_THRESHOLD = 2.0; // Z-score to trigger a trade
}
