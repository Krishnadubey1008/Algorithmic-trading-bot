cmake_minimum_required(VERSION 3.15)
project(TradingBotCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the packages installed by vcpkg
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# --- Executable 1: Your original single-asset bot ---
add_executable(trading_bot
    src/main.cpp
    src/APIDataHandler.cpp
    src/Backtester.cpp
)
target_link_libraries(trading_bot PRIVATE cpr::cpr nlohmann_json::nlohmann_json)


# --- Executable 2: The new pairs trading bot ---
add_executable(pairs_trading_bot
    src/main_pairs.cpp
    src/PairsDataHandler.cpp
    src/PairsBacktester.cpp
)
target_link_libraries(pairs_trading_bot PRIVATE cpr::cpr nlohmann_json::nlohmann_json)