// src/config.h
#pragma once
#include <string>

// WARNING: Do not commit API keys to a public repository.
// Use environment variables or a secure vault in production.
namespace config {
    // --- API Keys ---
    const std::string ALPHA_VANTAGE_API_KEY = "U99C1ZPRB2E673SR";
    const std::string ALPACA_API_KEY        = "PKTF90X4J9FKS6IOZ4NB";
    const std::string ALPACA_SECRET_KEY     = "AbGsKRGrgdMMaNyTF4yox6XZUat8Q6l1l6C4g1Bb";
    

    // --- Trading Parameters ---
    const std::string SYMBOL = "AAPL";
    const int SHORT_WINDOW   = 10;
    const int LONG_WINDOW    = 30;
    const double INITIAL_CAPITAL = 10000.0;
}
