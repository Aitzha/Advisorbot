#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook {
public:
    //construct, reading a csv data file
    OrderBook(std::string filename);
    //return vector of all known products in the dataset (changed by student)
    std::vector<std::string> getKnownProducts(std::string const &currentTime);
    //return vector of Orders according to the sent filters
    std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                          std::string product,
                                          std::string timestamp);


    //gets earliest time in the orderbook
    std::string getEarliestTime();
    //returns the next time after given timestamp
    std::string getNextTime(std::string timestamp);
    //return the previous time before given timestamp
    std::string getPrevTime(std::string timestamp);
    //return the a th order index time stamp
    std::string setCurrentTime(int a);
    //returns "orders" array size
    int getEntriesSize();

    void insertOrder(OrderBookEntry& order);

    std::vector<OrderBookEntry> matchAsksToBids(std::string &timestamp);

    static double getHighPrice(std::vector<OrderBookEntry> &orders);
    static double getLowPrice(std::vector<OrderBookEntry> &orders);
    static double getAvgPrice(std::vector<OrderBookEntry> &orders);

private:
    std::vector<OrderBookEntry> orders;
};