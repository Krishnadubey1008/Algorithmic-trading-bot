// src/APIDataHandler.hpp
#pragma once
#include "DataHandler.hpp" // It inherits from DataHandler

// This is the declaration of our class
class APIDataHandler : public DataHandler {
public:
    // The constructor
    APIDataHandler(const std::string& symbol);

    // The main function
    bool get_next_bar(Bar& bar) override;

private:
    std::vector<Bar> bars;
    size_t current_index;
};