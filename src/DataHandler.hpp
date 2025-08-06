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
// Abstract base class for all data handlers
class DataHandler {
public:
    virtual ~DataHandler() = default;
    virtual bool get_next_bar(Bar& bar) = 0;
};
