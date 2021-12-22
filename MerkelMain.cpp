#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Wallet.h"

MerkelMain::MerkelMain()
{

}

[[noreturn]] void MerkelMain::init()
{
    //greeting user
    std::cout << "Welcome, I am Advisorbot. "
                 "I am command line program that can help you you with cryptocurrency investing analyse" << std::endl;

    std::cout << "Before start using Advisorbot, you have 1000 USDT in Advisorbot wallet." << std::endl;
    std::cout << "You can deposit additional crypto to your wallet by command \"deposit _cryptoName, _amount\"" << std::endl;
    std::cout << "Please enter \"help\" to to see all available commands" << std::endl;


    currentTime = orderBook.getEarliestTime();
    wallet.addCurrency("USDT", 1000);

    while(true) {
        printMenu();
        processUserOption();
    }
}

void MerkelMain::printMenu()
{
    std::cout << "*********************************************************" << std::endl;
    std::cout << "Hint: enter \"help\" to to see all available commands" << std::endl;
    std::cout << "Advisorbot: how can I help you?\n\n";
}

void MerkelMain::printHelp()
{
    std::cout << "All available commands: " << std::endl;
    std::cout << "help, help <cmd>, prod, min, max, avg, predict, time, step, deposit" << std::endl;
}

void MerkelMain::printHelpCmd(std::string &cmd) {
    if(cmd == "help" || cmd == "Help") {
        std::cout << cmd << " - prints all available commands" << std::endl;
        std::cout << "Format: help" << std::endl;
        std::cout << "Example: help" << std::endl;
        return;
    }

    if(cmd == "prod" || cmd == "Prod") {
        std::cout << cmd << " - prints list of all products in current time step" << std::endl;
        std::cout << "Format: prod" << std::endl;
        std::cout << "Example: prod" << std::endl;
        return;
    }

    if(cmd == "min" || cmd == "Min") {
        std::cout << cmd << " - prints minimum bid or ask for product in current time step" << std::endl;
        std::cout << "Format: min _product bid/ask" << std::endl;
        std::cout << "Example: min ETH/BTC ask" << std::endl;
        return;
    }

    if(cmd == "max" || cmd == "Max") {
        std::cout << cmd << " - prints maximum bid or ask for product in current time step" << std::endl;
        std::cout << "Format: max _product bid/ask" << std::endl;
        std::cout << "Example: max ETH/BTC ask" << std::endl;
        return;
    }

    if(cmd == "avg" || cmd == "Avg") {
        std::cout << cmd << " - prints average ask or bid for the product over the number of time steps" << std::endl;
        std::cout << "Format: avg _product bid/ask _numberOfTimeSteps" << std::endl;
        std::cout << "Example: avg ETH/BTC ask 10" << std::endl;
        return;
    }

    if(cmd == "predict" || cmd == "Predict") {
        std::cout << cmd << " - predict max or min ask or bid for the product for the next time step" << std::endl;
        std::cout << "Format: predict max/min _product bid/ask" << std::endl;
        std::cout << "Example: predict max ETH/BTC ask" << std::endl;
        return;
    }

    if(cmd == "time" || cmd == "Time") {
        std::cout << cmd << " - prints current time in a dataset" << std::endl;
        std::cout << "Format: time" << std::endl;
        std::cout << "Example: time" << std::endl;
        return;
    }

    if(cmd == "step" || cmd == "Step") {
        std::cout << cmd << " - move to the next time step" << std::endl;
        std::cout << "Format: step" << std::endl;
        std::cout << "Example: step" << std::endl;
        return;
    }

    if(cmd == "deposit" || cmd == "Deposit") {
        std::cout << cmd << " - deposit sent number of sent product to the Advisorbot wallet" << std::endl;
        std::cout << "Format: deposit _product _amount" << std::endl;
        std::cout << "Example: deposit BTC 1" << std::endl;
        return;
    }

    std::cout << "MerkelMain::printHelpCmd Unknown command" << std::endl;
}


void MerkelMain::processUserOption() {

    std::string userInput;
    std::getline(std::cin, userInput);
    std::vector<std::string> tokens = CSVReader::tokenise(userInput, ' ');
    if((tokens[0] == "help" || tokens[0] == "Help") && tokens.size() == 1) {
        printHelp();
        return;
    }
    if((tokens[0] == "help" || tokens[0] == "Help") && tokens.size() == 2) {
        printHelpCmd(tokens[1]);
        return;
    }
    if((tokens[0] == "prod" || tokens[0] == "Prod") && tokens.size() == 1) {
        printMarketStats();
        return;
    }
    if((tokens[0] == "min" || tokens[0] == "Min") && tokens.size() == 3) {
        printMin(tokens);
        return;
    }
    if((tokens[0] == "max" || tokens[0] == "Max") && tokens.size() == 3) {
        printMax(tokens);
        return;
    }
    if((tokens[0] == "avg" || tokens[0] == "Avg") && tokens.size() == 1) {
        printMarketStats();
        return;
    }
    if((tokens[0] == "predict" || tokens[0] == "Predict") && tokens.size() == 1) {
        printMarketStats();
        return;
    }
    if((tokens[0] == "time" || tokens[0] == "Time") && tokens.size() == 1) {
        std::cout << currentTime << std::endl;
        return;
    }
    if((tokens[0] == "step" || tokens[0] == "Step") && tokens.size() == 1) {
        gotoNextTimeframe();
        return;
    }


    std::cout << "MerkelMain::processUserOption Unknown command or wrong parameters" << std::endl;
    std::cout << "Remember to separate command and parameters with space" << std::endl;
}


void MerkelMain::printMarketStats() {
    std::vector<std::string> products = orderBook.getKnownProducts(currentTime);
    std::cout << "Products found: " << products.size() << std::endl;
    for(std::string const &s : products) {
        std::cout << "Product: " << s << std::endl;
    }
}

void MerkelMain::printMin(const std::vector<std::string> &tokens) {
    OrderBookType type;
    std::string product = tokens[1];
    std::vector<OrderBookEntry> entries;

    //finding order type
    if(tokens[2] == "ask") {
        type = OrderBookType::ask;
    } else if(tokens[2] == "bid") {
        type = OrderBookType::bid;
    } else {
        std::cout << "MerkelMain::printMin unknowns order type" << std::endl;
        return;
    }

    //saving all match with parameters orders
    entries = orderBook.getOrders(type, product, currentTime);

    if(entries.empty()) {
        std::cout << "No matching orders found" << std::endl;
        return;
    }

    //printing number of matches
    if(type == OrderBookType::ask) {
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Min ask: ";
    } else {
        std::cout << "Bids seen: " << entries.size() << std::endl;
        std::cout << "Min bid: ";
    }

    std::cout << OrderBook::getLowPrice(entries) << std::endl;
}

void MerkelMain::printMax(std::vector<std::string> const &tokens) {
    OrderBookType type;
    std::string product = tokens[1];
    std::vector<OrderBookEntry> entries;

    //finding order type
    if(tokens[2] == "ask") {
        type = OrderBookType::ask;
    } else if(tokens[2] == "bid") {
        type = OrderBookType::bid;
    } else {
        std::cout << "MerkelMain::printMax unknowns order type" << std::endl;
        return;
    }

    //saving all match with parameters orders
    entries = orderBook.getOrders(type, product, currentTime);

    if(entries.empty()) {
        std::cout << "No matching orders found" << std::endl;
        return;
    }

    //printing number of matches
    if(type == OrderBookType::ask) {
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: ";
    } else {
        std::cout << "Bids seen: " << entries.size() << std::endl;
        std::cout << "Max bid: ";
    }

    std::cout << OrderBook::getHighPrice(entries) << std::endl;
}

void MerkelMain::enterAsk() {
    std::cout << "Make an ask - enter the amount: product(crypto you want to sell/crypto you want to get), price and amount (eg ETH/BTC,10,0.5) " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3) {
        std::cout << "MerkelMain::enterAsk Bad input" << std::endl;
    } else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::ask);
            obe.username = "simUser";
            if(wallet.canFulfillOrder(obe)) {
                std::cout << "You successfully added ask" << std::endl;
                orderBook.insertOrder(obe);
            } else {
                std::cout << "Wallet has insufficient funds" << std::endl;
            }

        } catch(const std::exception& e) {
            std::cout << "MerkelMain::enterAsk Bad input" << std::endl;
        }
    }
}

void MerkelMain::enterBid() {
    std::cout << "Make a bid - enter the amount: product(crypto you want to buy/crypto you want to pay with), price and amount (eg ETH/BTC,10,0.5) " << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3) {
        std::cout << "MerkelMain::enterBid Bad input" << std::endl;
    } else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::bid);
            obe.username = "simUser";
            if(wallet.canFulfillOrder(obe)) {
                std::cout << "You successfully added bid" << std::endl;
                orderBook.insertOrder(obe);
            } else {
                std::cout << "Wallet has insufficient funds" << std::endl;
            }

        } catch(const std::exception& e) {
            std::cout << "MerkelMain::enterBid Bad input" << std::endl;
        }
    }
}

void MerkelMain::printWallet() {
    std::cout << wallet.toString();
    wallet.removeCurrency("USDT", 1500);
}

void MerkelMain::gotoNextTimeframe() {
    std::cout << "Going to next time frame. " << std::endl;
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout  << "Sales: " << sales.size() << std::endl;
    for(OrderBookEntry& sale : sales) {
        std::cout << "Sale price: " << sale.price << " amount: " << sale.amount << std::endl;
        if(sale.username == "simUser") {
            wallet.processSale(sale);
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}





