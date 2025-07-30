// src/PairsDataHandler.hpp
#pragma once
#include "DataHandler.hpp"
#include <vector>
#include <string>
#include <utility> // For std::pair

class PairsDataHandler {
public:
    PairsDataHandler(const std::string& symbol_a, const std::string& symbol_b);
    bool get_next_bar_pair(std::pair<Bar, Bar>& bar_pair);

private:
    std::vector<Bar> fetch_data_for_symbol(const std::string& symbol);
    std::vector<std::pair<Bar, Bar>> synchronized_bars;
    size_t current_index;
};