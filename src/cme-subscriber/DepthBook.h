//
// Created by Antoine Waugh on 12/04/2017.
//

#ifndef DEPTHBOOK_H
#define DEPTHBOOK_H

#include <map>
#include <memory>
#include <ostream>
#include <vector>


class TradeList {
private:
    int aggressorSide;
    double min;
    double max;
    int totalVolume;
    int count;
    std::vector<PriceEntry> trades;

public:
    const std::vector<PriceEntry>& getTrades() const { return trades; }

    void insert(double, int, int);
    void clear();

    double getMinPrice() const { return min; }

    double getMaxPrice() const { return max; }

    int getCount() const { return count; }

    int getTotalVolume() const { return totalVolume; }

    int getAggressorSide() const { return aggressorSide; }

    friend std::ostream& operator<<(std::ostream& os, const TradeList& list)
    {
        os << list.getAggressorSide() << ',' << list.getCount() << ','
           << list.getTotalVolume() << ',' << list.getMinPrice() << ','
           << list.getMaxPrice() << ',';

        int remaining = list.getTrades().size();

        for (auto&& item : list.getTrades()) {
            os << item.quantity << '@' << item.price;
            --remaining;
            if (remaining > 0) {
                os << '|';
            }
        }
        return os;
    }
};

class DepthBook {

private:
    std::string timestamp;

    std::string symbol;
    std::string securityGroup;
    std::string matchEventIndicator;

public:
    const std::string& getMatchEventIndicator() const;

private:
    DepthList bids;
    DepthList asks;

    DepthList implBids;
    DepthList implAsks;

    double bid1pDelta;
    double ask1pDelta;

    double bid1vDelta;
    double ask1vDelta;

    TradeList trades;

    long lastRptSeq;

    int securityTradingStatus;

    void clearFlags();
    void resetState();

    MDIncrementalRefresh mdRefresh;
    MDSecurityStatus mdStatus;

public:
    DepthBook(const std::string& symbol, const std::string& securityGroup)
        : symbol(symbol)
        , securityGroup(securityGroup)
        , bids(true)
        , asks(false)
        , implBids(true)
        , implAsks(false)
        , bid1pDelta(0)
        , ask1pDelta(0)
        , bid1vDelta(0)
        , ask1vDelta(0)
    {
    }
    DepthBook()
        : symbol("")
        , securityGroup("")
        , bids(true)
        , asks(false)
        , implBids(true)
        , implAsks(false)
        , bid1pDelta(0)
        , ask1pDelta(0)
        , bid1vDelta(0)
        , ask1vDelta(0)
    {
    }
    const std::string& getTimestamp() const { return timestamp; }

    int getSecurityTradingStatus() const { return securityTradingStatus; }

    long getLastRptSeq() const { return lastRptSeq; }

    bool handleMessage(const std::string&);

    friend std::ostream& operator<<(std::ostream& os, const DepthBook& book)
    {

        os << book.getTimestamp() << ',' << book.getSecurityTradingStatus() << ','
           << book.getMatchEventIndicator() << ',' << book.bids << ',' << book.asks
           << ',' << book.bid1pDelta << ',' << book.ask1pDelta << ','
           << book.bid1vDelta << ',' << book.ask1vDelta << ',' << book.trades << ','
           << book.getLastRptSeq() << ',' << book.implBids << ',' << book.implAsks;
        return os;
    }
};

#endif // DEPTHBOOK_H
