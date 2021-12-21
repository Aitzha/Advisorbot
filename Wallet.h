#pragma once
#include <string>
#include <map>
#include <iostream>
#include "OrderBookEntry.h"

class Wallet {
public:
    Wallet();
    //adds currency to the wallet
    void addCurrency(std::string name, double amount);
    //remove currency from the wallet
    bool removeCurrency(std::string name, double amount);
    //check if wallet contain this amount of currency
    bool containsCurrency(std::string name, double amount);
    //check if the wallet can cope this ask or bid
    bool canFulfillOrder(OrderBookEntry& order);
    //comment please
    void processSale(OrderBookEntry& sale);
    //generate a string representation of the wallet
    std::string toString();

private:
    std::map<std::string, double> currencies;
};

