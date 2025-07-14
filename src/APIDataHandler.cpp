// src/APIDataHandler.cpp
#include "APIDataHandler.hpp"
#include "config.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <algorithm>
#include <stdexcept>

// A using-declaration for convenience with the nlohmann/json library
using json = nlohmann::json;

// This is the implementation of the constructor.
// It fetches and processes data from the API when an APIDataHandler object is created.
APIDataHandler::APIDataHandler(const std::string& symbol) : current_index(0) {
    std::cout << "Fetching data for " << symbol << "..." << std::endl;
    
    // Construct the URL with the symbol and API key from the config.h file
    std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol="
                    + symbol + "&outputsize=full&apikey=" + config::ALPHA_VANTAGE_API_KEY;

    // Perform the HTTP GET request
    cpr::Response r = cpr::Get(cpr::Url{url});

    // Check if the request was successful
    if (r.status_code != 200) {
        throw std::runtime_error("Failed to fetch data. Status code: " + std::to_string(r.status_code));
    }

    try {
        // Parse the raw text from the response into a JSON object
        json data = json::parse(r.text);

        // Ensure the JSON contains the data we expect
        if (!data.contains("Time Series (Daily)")) {
            throw std::runtime_error("Invalid API response: 'Time Series (Daily)' key not found.");
        }

        // Loop through each day in the "Time Series (Daily)" object
        for (auto& item : data["Time Series (Daily)"].items()) {
            Bar bar; // Create a new Bar object to hold the data for one day
            
            // Extract the data for the current day and populate the Bar object
            bar.timestamp = item.key();
            bar.open      = std::stod(item.value()["1. open"].get<std::string>());
            bar.high      = std::stod(item.value()["2. high"].get<std::string>());
            bar.low       = std::stod(item.value()["3. low"].get<std::string>());
            bar.close     = std::stod(item.value()["4. close"].get<std::string>());
            bar.volume    = std::stol(item.value()["5. volume"].get<std::string>());
            
            // Add the fully populated Bar to our internal list
            bars.push_back(bar);
        }

        // The API returns data from newest to oldest. We reverse it to get a
        // chronological series (oldest to newest) for backtesting.
        std::reverse(bars.begin(), bars.end());
        
        std::cout << "Successfully loaded " << bars.size() << " bars of data." << std::endl;

    } catch (const json::parse_error& e) {
        // Catch any errors that occur during JSON parsing
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }
}

// This is the implementation of the get_next_bar function.
// The Backtester calls this repeatedly to get the next bar of data.
bool APIDataHandler::get_next_bar(Bar& bar) {
    // Check if we have any data left in our list
    if (current_index < bars.size()) {
        // If yes, provide the next bar
        bar = bars[current_index];
        // Move our index to the next position
        current_index++;
        return true; // Indicate success
    }
    // If we've reached the end of the list, indicate there is no more data
    return false;
}