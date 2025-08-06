// src/config.h
#pragma once
#include <string>

namespace config {
    // --- API Keys ---
    const std::string ALPHA_VANTAGE_API_KEY = "U99C1ZPRB2E673SR";
    const std::string ALPACA_API_KEY        = "PKTF90X4J9FKS6IOZ4NB";
    const std::string ALPACA_SECRET_KEY     = "AbGsKRGrgdMMaNyTF4yox6XZUat8Q6l1l6C4g1Bb";

    // --- Single-Asset Strategy Parameters ---
    const std::string SYMBOL = "AAPL";
    const int SHORT_WINDOW   = 10;
    const int LONG_WINDOW    = 30;
    const double INITIAL_CAPITAL = 10000.0;
    
    // --- Pairs Trading Strategy Parameters ---
    const std::string SYMBOL_A = "PYPL";   
    const std::string SYMBOL_B = "TFC";  
    const int LOOKBACK_WINDOW  = 30;     // Lookback period for spread stats
    const double Z_SCORE_THRESHOLD = 2.0; // Z-score to trigger a trade
}
