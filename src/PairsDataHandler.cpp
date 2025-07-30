#include "PairsDataHandler.hpp"
#include "config.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <map>

using json = nlohmann::json;

std::vector<Bar> PairsDataHandler::fetch_data_for_symbol(const std::string& symbol) {
    std::cout << "Fetching data for " << symbol << "..." << std::endl;
    std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol="
                    + symbol + "&outputsize=full&apikey=" + config::ALPHA_VANTAGE_API_KEY;
    cpr::Response r = cpr::Get(cpr::Url{url});
    if (r.status_code != 200) throw std::runtime_error("Failed to fetch data for " + symbol);
    if (r.text.empty()) throw std::runtime_error("Empty response for " + symbol);

    json data = json::parse(r.text);
    if (data.contains("Error Message") || data.contains("Note")) {
        throw std::runtime_error("API Error for " + symbol + ": " + data.dump(2));
    }
    if (!data.contains("Time Series (Daily)")) {
        throw std::runtime_error("Invalid API response for " + symbol);
    }

    std::vector<Bar> bars;
    for (auto& item : data["Time Series (Daily)"].items()) {
        Bar bar;
        bar.timestamp = item.key();
        bar.close = std::stod(item.value()["4. close"].get<std::string>());
        bars.push_back(bar);
    }
    std::reverse(bars.begin(), bars.end());
    return bars;
}

PairsDataHandler::PairsDataHandler(const std::string& symbol_a, const std::string& symbol_b) : current_index(0) {
    auto bars_a = fetch_data_for_symbol(symbol_a);
    auto bars_b = fetch_data_for_symbol(symbol_b);

    std::map<std::string, Bar> map_b;
    for (const auto& bar : bars_b) {
        map_b[bar.timestamp] = bar;
    }

    for (const auto& bar_a : bars_a) {
        if (map_b.count(bar_a.timestamp)) {
            synchronized_bars.push_back({bar_a, map_b[bar_a.timestamp]});
        }
    }
    std::cout << "Successfully synchronized " << synchronized_bars.size() << " bars of data." << std::endl;
}

bool PairsDataHandler::get_next_bar_pair(std::pair<Bar, Bar>& bar_pair) {
    if (current_index < synchronized_bars.size()) {
        bar_pair = synchronized_bars[current_index];
        current_index++;
        return true;
    }
    return false;
}