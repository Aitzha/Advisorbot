#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <iostream>
#include <algorithm>

OrderBook::OrderBook(std::string filename) {
    orders = CSVReader::readCSV(filename);
}

std::vector<std::string> OrderBook::getKnownProducts() {
    std::vector<std::string> products;
    std::map<std::string, bool> prodMap;

    for(OrderBookEntry& e : orders) {
        prodMap[e.product] = true;
    }

    for(auto const &e : prodMap) {
        products.push_back(e.first);
    }

    return products;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, std::string product, std::string timestamp) {
    std::vector<OrderBookEntry> orders_sub;
    for(OrderBookEntry& e : orders) {
        if (e.orderType == type
            && e.product == product
            && e.timestamp == timestamp) {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders) {
    double max = -1;
    for(OrderBookEntry& e : orders) {
        if(e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders) {
    double min = orders[0].price;
    for(OrderBookEntry& e : orders) {
        if(e.price < min)
            min = e.price;
    }
    return min;
}

std::string OrderBook::getEarliestTime() {
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp) {

    std::string next_timestamp = "";
    for(OrderBookEntry& e : orders) {
        if(e.timestamp > timestamp) {
            next_timestamp = e.timestamp;
            break;
        }
    }

    if(next_timestamp.empty()) {
        return orders[0].timestamp;
    }


    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry &order) {
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareTimestamps);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string &timestamp) {
    //vector of asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    //vector of bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);

    //sales
    std::vector<OrderBookEntry> sales;

    //sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    //sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    for(OrderBookEntry& ask : asks) {
        if(ask.amount == 0) {
            continue;
        }
        for(OrderBookEntry& bid : bids) {
            if(bid.price >= ask.price) {

                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                if(bid.username == "simUser") {
                    sale.orderType = OrderBookType::bidsale;
                    sale.username = "simUser";
                } else if(ask.username == "simUser") {
                    sale.orderType = OrderBookType::asksale;
                    sale.username = "simUser";
                }


                if(bid.amount == 0) {
                    continue;
                }

                sale.amount = std::min(bid.amount, ask.amount);
                ask.amount -= sale.amount;
                bid.amount -= sale.amount;

                sales.push_back(sale);
            }
        }
    }


    return sales;
}
