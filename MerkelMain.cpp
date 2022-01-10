#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

MerkelMain::MerkelMain()
{

}

[[noreturn]] void MerkelMain::init()
{
    //greeting user
    std::cout << "Welcome, I am Advisorbot. "
                 "I am command a line program that can help you with cryptocurrency investing analysis" << std::endl;
    std::cout << "Please enter \"help\" to to see all available commands" << std::endl;

    currentTime = orderBook.getEarliestTime();

    while(true) {
        printMenu();
        processUserInput();
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
    std::cout << "help, help <cmd>, prod, min, max, avg, predict, time, "
                 "step <parameter1>, step <parameter1> <parameter2>" << std::endl;
}

void MerkelMain::printHelpCmd(std::string &cmd) {
    if(cmd == "help" || cmd == "Help") {
        std::cout << cmd << " - prints all available commands" << std::endl;
        std::cout << "Format: help" << std::endl;
        std::cout << "Example: help" << std::endl;
        return;
    }

    if(cmd == "prod" || cmd == "Prod") {
        std::cout << cmd << " - prints list of all products in current timestamp" << std::endl;
        std::cout << "Format: prod" << std::endl;
        std::cout << "Example: prod" << std::endl;
        return;
    }

    if(cmd == "min" || cmd == "Min") {
        std::cout << cmd << " - prints minimum bid or ask for product in current timestamp" << std::endl;
        std::cout << "Format: min _product bid/ask" << std::endl;
        std::cout << "Example: min ETH/BTC ask" << std::endl;
        return;
    }

    if(cmd == "max" || cmd == "Max") {
        std::cout << cmd << " - prints maximum bid or ask for product in current timestamp" << std::endl;
        std::cout << "Format: max _product bid/ask" << std::endl;
        std::cout << "Example: max ETH/BTC ask" << std::endl;
        return;
    }

    if(cmd == "avg" || cmd == "Avg") {
        std::cout << cmd << " - prints average ask or bid for the product over the number of timestamps" << std::endl;
        std::cout << "Format: avg _product bid/ask _numberOfTimeSteps" << std::endl;
        std::cout << "Example: avg ETH/BTC ask 10" << std::endl;
        return;
    }

    if(cmd == "predict" || cmd == "Predict") {
        std::cout << cmd << " - predict max or min ask or bid for the product for the next timestamp" << std::endl;
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
        std::cout << "There are two versions of this command" << std::endl;
        std::cout << "First version have only 1 parameter and it moves to the next or the previous timestamp" << std::endl;
        std::cout << "Format: step forth/back" << std::endl;
        std::cout << "Example: step forth" << std::endl;

        std::cout << "Second version have 2 parameters and it moves n timestamps back or forth" << std::endl;
        std::cout << "Format: step forth/back _numberOfTimeStamps" << std::endl;
        std::cout << "Example: step forth 5" << std::endl;
        return;
    }


    std::cout << "MerkelMain::printHelpCmd Unknown command" << std::endl;
}


void MerkelMain::processUserInput() {
    //Deleted getUserInput function and made MerkelMain::processUserInput read and tokenise input itself.
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
    if((tokens[0] == "avg" || tokens[0] == "Avg") && tokens.size() == 4) {
        printAvg(tokens);
        return;
    }
    if((tokens[0] == "predict" || tokens[0] == "Predict") && tokens.size() == 4) {
        printPrediction(tokens);
        return;
    }
    if((tokens[0] == "time" || tokens[0] == "Time") && tokens.size() == 1) {
        std::cout << currentTime << std::endl;
        return;
    }
    if((tokens[0] == "step" || tokens[0] == "Step") && tokens.size() == 2) {
        if(tokens[1] == "forth" || tokens[1] == "Forth") {
            gotoNextTimeframe();
        } else if(tokens[1] == "back" || tokens[1] == "Back") {
            gotoPrevTimeframe();
        } else {
            std::cout << "MerkelMain::processUserInput wrong first parameter for step function" << std::endl;
        }
        return;
    }
    if((tokens[0] == "step" || tokens[0] == "Step") && tokens.size() == 3) {
        if(tokens[1] == "forth" || tokens[1] == "Forth") {
            goForthTimeframes(tokens[2]);
        } else if(tokens[1] == "back" || tokens[1] == "Back") {
            goBackTimeframes(tokens[2]);
        } else {
            std::cout << "MerkelMain::processUserInput wrong first parameter for step function" << std::endl;
        }
        return;
    }


    std::cout << "MerkelMain::processUserInput Unknown command or wrong parameters" << std::endl;
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
    OrderBookType orderType;
    std::string product = tokens[1];
    std::vector<OrderBookEntry> entries;

    //check for existence of the product
    std::vector<std::string> listOfProducts = orderBook.getKnownProducts(currentTime);
    for(std::string &s : listOfProducts) {
        if(s == product) {
            break;
        }

        if(s != product && s == listOfProducts.back()) {
            std::cout << "MerkelMain::printMin no matching product for current time" << std::endl;
            return;
        }
    }

    //assigning value "orderType" variable
    orderType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if(orderType == OrderBookType::unknown) {
        std::cout << "MerkelMain::printMin unknowns order orderType" << std::endl;
        return;
    }


    //saving all match with parameters orders
    entries = orderBook.getOrders(orderType, product, currentTime);
    if(entries.empty()) {
        std::cout << "MerkelMain::printMin No matching orders found" << std::endl;
        return;
    }

    //printing number of matches
    if(orderType == OrderBookType::ask) {
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Min ask: ";
    } else {
        std::cout << "Bids seen: " << entries.size() << std::endl;
        std::cout << "Min bid: ";
    }

    //printing the lowest price
    std::cout << OrderBook::getLowPrice(entries) << std::endl;
}

void MerkelMain::printMax(std::vector<std::string> const &tokens) {
    OrderBookType orderType;
    std::string product = tokens[1];
    std::vector<OrderBookEntry> entries;

    //check for existence of the product
    std::vector<std::string> listOfProducts = orderBook.getKnownProducts(currentTime);
    for(std::string &s : listOfProducts) {
        if(s == product) {
            break;
        }

        if(s != product && s == listOfProducts.back()) {
            std::cout << "MerkelMain::printMax no matching product for current time" << std::endl;
            return;
        }
    }

    //assigning value "orderType" variable
    orderType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if(orderType == OrderBookType::unknown) {
        std::cout << "MerkelMain::printMax unknowns order orderType" << std::endl;
        return;
    }

    //saving all match with parameters orders
    entries = orderBook.getOrders(orderType, product, currentTime);
    if(entries.empty()) {
        std::cout << "MerkelMain::printMax No matching orders found" << std::endl;
        return;
    }

    //printing number of matches
    if(orderType == OrderBookType::ask) {
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: ";
    } else {
        std::cout << "Bids seen: " << entries.size() << std::endl;
        std::cout << "Max bid: ";
    }

    //printing the highest price
    std::cout << OrderBook::getHighPrice(entries) << std::endl;
}

void MerkelMain::printAvg(const std::vector<std::string> &tokens) {
    OrderBookType orderType;
    std::string product = tokens[1];
    int numberOfTimeStamps = 0;


    //assigning value to "value" variable
    orderType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if(orderType == OrderBookType::unknown) {
        std::cout << "MerkelMain::printAvg unknowns order orderType" << std::endl;
        return;
    }

    //assigning value to "numberOfTimeStamps" variable
    try {
        numberOfTimeStamps = std::stoi(tokens[3]);
    }catch(const std::exception& e){
        std::cout << "MerkelMain::printAvg Bad integer " << tokens[3]<< std::endl;
        return;
    }

    std::string timeStep = currentTime;
    double totalAvg = 0;

    //moving to the prev timestamps and adding their avg to totalAvg
    for(int i = 0; i < numberOfTimeStamps; i++) {
        std::vector<OrderBookEntry> entries = orderBook.getOrders(orderType, product, timeStep);

        //If timeStamp is empty, it means the previous timeStamp was the first and no timeStamp left to check.
        if(timeStep.empty()) {
            std::cout << "MerkelMain::printAvg reached the first timestamp" << std::endl;
            if(i == 0) {
                std::cout << "Impossible to return average price, because current timestamp is the first" << std::endl;
            } else {
                std::cout << "Total average price from " << i << " timestamps: " << totalAvg / i << std::endl;
            }
            return;
        }

        //print if orders are not found on current timestamp
        if(entries.empty()) {
            std::cout << i + 1 << ") Orders aren't found at " << timeStep << " timestamp" << std::endl;
            timeStep = orderBook.getPrevTime(timeStep);
            continue;
        }

        double avg = OrderBook::getAvgPrice(entries);

        std::cout << i + 1 << ") Average price at " << timeStep << ": " << avg << std::endl;
        totalAvg += avg;
        timeStep = orderBook.getPrevTime(timeStep);
    }

    //print total average price
    std::cout << "Total average price: " << totalAvg / numberOfTimeStamps << std::endl;
}

void MerkelMain::printPrediction(const std::vector<std::string> &tokens) {
    //change name of variable to "measurement" or "unit"
    std::string value;
    std::string product = tokens[2];
    OrderBookType orderType;


    //assigning value to "value" variable
    if(tokens[1] == "max") {
        value = "max";
    } else if(tokens[1] == "min") {
        value = "min";
    } else {
        std::cout << "MerkelMain::printPrediction unknowns value" << std::endl;
        return;
    }

    //assigning value "orderType" variable
    orderType = OrderBookEntry::stringToOrderBookType(tokens[3]);
    if(orderType == OrderBookType::unknown) {
        std::cout << "MerkelMain::printAvg unknowns order orderType" << std::endl;
        return;
    }


    std::string timeStamp = currentTime;
    std::vector<OrderBookEntry> entries;
    double prevPrice = -1;
    double curPrice = -1;
    int cnt = 0;
    //finding previous and current price of the product
    while(cnt < 1000) {
        //If timeStamp is empty, it means the previous timeStamp was the first and no timeStamp left to check.
        //Also, if while loop was not interrupt then, prevPrice or curPrice are not assigned and not enough information
        //collected to make prediction.
        if(timeStamp.empty()) {
            std::cout << "MerkelMain::printPrediction reached the first timestamp" << std::endl;
            std::cout << "Impossible to print prediction because "
                         "there is no enough information about this product on the previous timestamps" << std::endl;
            return;
        }

        entries = orderBook.getOrders(orderType, product, timeStamp);
        //skipping current timeStamp if entries is empty.
        if(entries.empty()) {
            timeStamp = orderBook.getPrevTime(timeStamp);
            cnt++;
            continue;
        }

        //assigning values to curPrice and prevPrice variables
        if(curPrice == -1) {
            if(value == "max") {
                curPrice = OrderBook::getHighPrice(entries);
            } else if(value == "min") {
                curPrice = OrderBook::getLowPrice(entries);
            }
            cnt = 0;
        } else if(prevPrice == -1) {
            if(value == "max") {
                prevPrice = OrderBook::getHighPrice(entries);
            } else if(value == "min") {
                prevPrice = OrderBook::getLowPrice(entries);
            }
            break;
        }

        timeStamp = orderBook.getPrevTime(timeStamp);
        cnt++;
    }

    //if no previous price was found, print exception for user
    if(prevPrice == -1) {
        std::cout << "MerkelMain::printPrediction Sorry, but advisorBot couldn't make prediction" << std::endl;
        std::cout << "Most likely you entered wrong product name or there is no " << tokens[3];
        std::cout << " in csv file, for product you entered" << std::endl;
        return;
    }

    //print answer
    std::cout << "Previous price: " <<  prevPrice << " " << "Current price " <<  curPrice << std::endl;
    std::cout << "Prediction: new " << value + " " << product + " " << tokens[3] + " " << "price might be ";
    std::cout << ((curPrice - prevPrice) * -2 / 10) + curPrice << std::endl;
}

void MerkelMain::gotoNextTimeframe() {
    /*
     * Since I have changed OrderBook::matchAsksToBids function so it returns sales of all products and also changed
     * how MerkelMain::gotoNextTimeframe prints them
    */
    /*
     * And to make it more clear for the user, I changed OrderBook::getNextTime so MerkelMain::gotoNextTimeframe loop
     * to the first timestamp if it reaches the final timestamp
     */
    std::cout << "Going to the next time frame. " << std::endl;

    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(currentTime);
    std::cout  << "Sales: " << sales.size() << std::endl;

    std::string currentProduct = "";
    for(OrderBookEntry &e : sales) {
        if(currentProduct != e.product) {
            currentProduct = e.product;
            std::cout << currentProduct << std::endl;
        }

        std::cout << "Sale price: " << e.price << " amount: " << e.amount << std::endl;
    }

    currentTime = orderBook.getNextTime(currentTime);
    if(currentTime.empty()) {
        std::cout << "MerkelMain::gotoNextTimeframe reached the last timestamp" << std::endl;
        std::cout << "Current time will be set to the first timestamp" << std::endl;
        currentTime = orderBook.setCurrentTime(1);
    }
    std::cout << "new timestamp: " << currentTime << std::endl;
}

void MerkelMain::gotoPrevTimeframe() {
    std::cout << "Going to the previous time frame." << std::endl;
    currentTime = orderBook.getPrevTime(currentTime);
    if(currentTime.empty()) {
        std::cout << "MerkelMain::gotoNextTimeframe reached the first timestamp" << std::endl;
        std::cout << "Current time will be set to the last timestamp" << std::endl;
        currentTime = orderBook.setCurrentTime(orderBook.getEntriesSize());
    }
    std::cout << "new timestamp: " << currentTime << std::endl;
}

void MerkelMain::goForthTimeframes(std::string &steps) {
    int numberOfTimeStamps = 0;

    //assigning value to "numberOfTimeStamps" variable
    try {
        numberOfTimeStamps = std::stoi(steps);
    }catch(const std::exception& e){
        std::cout << "MerkelMain::goForthTimeframes Bad integer " << steps << std::endl;
        return;
    }

    for(int i = 0; i < numberOfTimeStamps; i++) {
        currentTime = orderBook.getNextTime(currentTime);
        if(currentTime.empty()) {
            std::cout << "MerkelMain::goForthTimeframes reached the last timestamp" << std::endl;
            std::cout << "Current time will be set to the first timestamp" << std::endl;
            currentTime = orderBook.setCurrentTime(1);
        }
    }
    std::cout << "new timestamp: " << currentTime << std::endl;
}

void MerkelMain::goBackTimeframes(std::string &steps) {
    int numberOfTimeStamps = 0;

    //assigning value to "numberOfTimeStamps" variable
    try {
        numberOfTimeStamps = std::stoi(steps);
    }catch(const std::exception& e){
        std::cout << "MerkelMain::goForthTimeframes Bad integer " << steps << std::endl;
        return;
    }

    for(int i = 0; i < numberOfTimeStamps; i++) {
        currentTime = orderBook.getPrevTime(currentTime);
        if(currentTime.empty()) {
            std::cout << "MerkelMain::goBackTimeframes reached the first timestamp" << std::endl;
            std::cout << "Current time will be set to the last timestamp" << std::endl;
            currentTime = orderBook.setCurrentTime(orderBook.getEntriesSize());
        }
    }
    std::cout << "new timestamp: " << currentTime << std::endl;
}







