// src/PairsDataHandler.hpp
#pragma once
#include "DataHandler.hpp"
#include <vector>
#include <string>
#include <utility> // For std::pair

// This class will fetch and synchronize data for two symbols
class PairsDataHandler {
public:
    PairsDataHandler(const std::string& symbol_a, const std::string& symbol_b);

    // Gets the next synchronized pair of bars
    bool get_next_bar_pair(std::pair<Bar, Bar>& bar_pair);

private:
    std::vector<Bar> fetch_data_for_symbol(const std::string& symbol);
    std::vector<std::pair<Bar, Bar>> synchronized_bars;
    size_t current_index;
};