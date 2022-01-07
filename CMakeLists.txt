cmake_minimum_required(VERSION 3.17)
project(Starter_code_topic_4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Starter_code_topic_4
        CSVReader.cpp
        CSVReader.h
        main.cpp
        MerkelMain.cpp
        MerkelMain.h
        OrderBookEntry.cpp
        OrderBookEntry.h
        OrderBook.cpp
        OrderBook.h
        )
