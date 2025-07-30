// src/DataHandler.hpp
#pragma once
#include <vector>
#include <string>

// Represents a single bar of OHLCV data
struct Bar {
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    long volume;
};