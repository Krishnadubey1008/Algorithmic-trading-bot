// src/config.h
#pragma once
#include <string>

// WARNING: Do not commit API keys to a public repository.
// Use environment variables or a secure vault in production.
namespace config {
    // --- API Keys ---
    const std::string ALPHA_VANTAGE_API_KEY = "YOUR_ALPHA_VANTAGE_API_KEY";
    const std::string ALPACA_API_KEY        = "YOUR_ALPACA_API_KEY";
    const std::string ALPACA_SECRET_KEY     = "YOUR_ALPACA_SECRET_KEY";

    // --- Trading Parameters ---
    const std::string SYMBOL = "AAPL";
    const int SHORT_WINDOW   = 10;
    const int LONG_WINDOW    = 30;
    const double INITIAL_CAPITAL = 10000.0;
}