#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    [[noreturn]] void init();
private:
    void printMenu();
    void printHelp();
    void printHelpCmd(std::string &cmd);
    void printMarketStats();
    void printMin(std::vector<std::string> const &tokens);
    void printMax(std::vector<std::string> const &tokens);
    void printAvg(std::vector<std::string> const &tokens);
    void printPrediction(std::vector<std::string> const &tokens);
    void gotoNextTimeframe();
    void gotoPrevTimeframe();
    void goForwardTimeframe(int &steps);
    void goBackTimeframe(int &steps);
    void processUserOption();

    std::string currentTime;

    //read 20200317.csv
    OrderBook orderBook{"D:/UoL/OOP/unzipped/Starter code topic 4/test.csv"};
}; 
