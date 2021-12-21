#include "Wallet.h"
#include "CSVReader.h"

Wallet::Wallet() {

}

void Wallet::addCurrency(std::string name, double amount) {
    if(amount < 0) {
        throw std::exception{};
    }

    if(currencies.count(name) == 0) {
        currencies[name] = 0;
    }

    currencies[name] += amount;
}

bool Wallet::removeCurrency(std::string name, double amount) {
    if(amount < 0) {
        return false;
    }

    if(currencies.count(name) == 0 || currencies[name] < amount) {
        return false;
    }

    currencies[name] -= amount;
    return true;
}

bool Wallet::containsCurrency(std::string name, double amount) {
    if(currencies.count(name) == 0) {
        return false;
    } else {
        return currencies[name] >= amount;
    }
}

bool Wallet::canFulfillOrder(OrderBookEntry& order) {
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    if(order.orderType == OrderBookType::ask) {

        std::cout << "Wallet::canFulfillOrder " << currs[0] << " : " << order.amount << std::endl;
        return containsCurrency(currs[0], order.amount);

    } else if(order.orderType == OrderBookType::bid) {

        double amount = order.amount * order.price;
        std::cout << "Wallet::canFulfillOrder " << currs[1] << " : " << amount << std::endl;
        return containsCurrency(currs[1], amount);

    }
    return false;
}

void Wallet::processSale(OrderBookEntry &sale) {
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');

    if(sale.orderType == OrderBookType::asksale) {
        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

    } else if(sale.orderType == OrderBookType::bidsale) {

        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}

std::string Wallet::toString() {
    std::string s;
    for(std::pair<std::string, double> pair : currencies) {
        s += pair.first + " : " + std::to_string(pair.second) + "\n";
    }
    return s;
}







