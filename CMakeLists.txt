cmake_minimum_required(VERSION 3.15)
project(TradingBotCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the packages installed by vcpkg
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Add your executable and list its source files
add_executable(trading_bot
    src/main.cpp
    src/APIDataHandler.cpp
    src/Backtester.cpp
)

# Link the found libraries to your executable
target_link_libraries(trading_bot PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)