// src/APIDataHandler.cpp
#include "DataHandler.hpp"
#include "config.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using json = nlohmann::json;

class APIDataHandler : public DataHandler {
public:
    APIDataHandler(const std::string& symbol);
    bool get_next_bar(Bar& bar) override;

private:
    std::vector<Bar> bars;
    size_t current_index;
};

APIDataHandler::APIDataHandler(const std::string& symbol) : current_index(0) {
    std::cout << "Fetching data for " << symbol << "..." << std::endl;
    std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol="
                    + symbol + "&outputsize=full&apikey=" + config::ALPHA_VANTAGE_API_KEY;

    cpr::Response r = cpr::Get(cpr::Url{url});

    if (r.status_code != 200) {
        throw std::runtime_error("Failed to fetch data. Status: " + std::to_string(r.status_code));
    }

    try {
        json data = json::parse(r.text);
        if (!data.contains("Time Series (Daily)")) {
            throw std::runtime_error("Invalid API response: 'Time Series (Daily)' not found.");
        }

        for (auto& item : data["Time Series (Daily)"].items()) {
            Bar bar;
            bar.timestamp = item.key();
            bar.open      = std::stod(item.value()["1. open"].get<std::string>());
            bar.high      = std::stod(item.value()["2. high"].get<std::string>());
            bar.low       = std::stod(item.value()["3. low"].get<std::string>());
            bar.close     = std::stod(item.value()["4. close"].get<std::string>());
            bar.volume    = std::stol(item.value()["5. volume"].get<std::string>());
            bars.push_back(bar);
        }

        std::reverse(bars.begin(), bars.end());
        std::cout << "Successfully loaded " << bars.size() << " bars of data." << std::endl;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }
}

bool APIDataHandler::get_next_bar(Bar& bar) {
    if (current_index < bars.size()) {
        bar = bars[current_index];
        current_index++;
        return true;
    }
    return false;
}