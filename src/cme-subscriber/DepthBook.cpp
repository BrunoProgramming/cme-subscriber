
#include <iostream>
#include <memory>

#include "DepthBook.h"

void TradeList::insert(double price, int quantity, int aggressorSide)
{
    if (min == 0 || price < min) {
        min = price;
    }
    if (max == 0 || price > max) {
        max = price;
    }
    if (aggressorSide != 0) {
        this->aggressorSide = aggressorSide;
    }
    totalVolume += quantity;
    trades.push_back(PriceEntry{ price, quantity });

    ++count;
}

void TradeList::clear()
{
    aggressorSide = 0;
    min = 0;
    max = 0;
    count = 0;
    totalVolume = 0;
    trades.clear();
}

void DepthBook::clearFlags()
{

    // Clear previous state - rename as implies book is being clearned which it isnt
    trades.clear();
    bids.clear();
    asks.clear();
    implBids.clear();
    implAsks.clear();
}

void DepthBook::resetState()
{

    trades.clear();
    bids.reset();
    asks.reset();
    implBids.reset();
    implAsks.reset();
}

bool isStatusUpdate(const std::string& s)
{
    return s.find("35=f") != std::string::npos;
}

bool DepthBook::handleMessage(const std::string& s)
{
    // Update book
    bool bookUpdated = false;

    clearFlags();

    // FIXME: Move to decoder
    if (isStatusUpdate(s)) {

        mdStatus.update(s);
        if (mdStatus.SecurityGroup == this->securityGroup) {

            this->timestamp = mdStatus.TransactTime;
            this->securityTradingStatus = mdStatus.SecurityTradingStatus;
            this->matchEventIndicator = mdStatus.MatchEventIndicator;

            bookUpdated = true;
        }

    } else if (s.find("35=X") != std::string::npos) {
        mdRefresh.update(s);

        double prevBid1p = bids.getBestEntry().price;
        double prevBid1v = bids.getBestEntry().quantity;
        double prevAsk1p = asks.getBestEntry().price;
        double prevAsk1v = asks.getBestEntry().quantity;

        // replace with deep copy of book,
        // then bids.getDeltas(previousBids)
        // where price at top level can represent bid1pdelta
        // and each of the volumes equal the volume deltas

        for (auto entry : mdRefresh.MDEntries) {
            if (entry.Symbol == this->symbol) {
                this->timestamp = mdRefresh.TransactTime;
                this->matchEventIndicator = mdRefresh.MatchEventIndicator;

                // remove book state when market transitions over weekend
                if (this->lastRptSeq > entry.RptSeq) {
                    resetState();
                }

                switch (entry.MDEntryType) {

                case MDEntryType_BID:
                    bids.insert(entry.MDEntryPx, entry.MDEntrySize, entry.MDPriceLevel, entry.MDUpdateAction);
                    this->lastRptSeq = entry.RptSeq;
                    bookUpdated = true;
                    break;

                case MDEntryType_OFFER:
                    asks.insert(entry.MDEntryPx, entry.MDEntrySize, entry.MDPriceLevel, entry.MDUpdateAction);
                    this->lastRptSeq = entry.RptSeq;
                    bookUpdated = true;
                    break;

                case MDEntryType_IMPLIED_BID:
                    implBids.insert(entry.MDEntryPx, entry.MDEntrySize, entry.MDPriceLevel, entry.MDUpdateAction);
                    this->lastRptSeq = entry.RptSeq;
                    bookUpdated = true;
                    break;

                case MDEntryType_IMPLIED_OFFER:
                    implAsks.insert(entry.MDEntryPx, entry.MDEntrySize, entry.MDPriceLevel, entry.MDUpdateAction);
                    this->lastRptSeq = entry.RptSeq;
                    bookUpdated = true;
                    break;

                case MDEntryType_TRADE:
                    trades.insert(entry.MDEntryPx, entry.MDEntrySize, entry.AggressorSide);
                    this->lastRptSeq = entry.RptSeq;
                    bookUpdated = true;
                    break;
                }
            }
        }

        // update top of book deltas
        // this needs to be done external to the depthbook as the book us unaware of
        // how many mdEntries are in a single MDIncrementalRefresh

        if (prevBid1p > 0.0 and prevAsk1p > 0.0) {
            this->bid1pDelta = bids.getBestEntry().price - prevBid1p;
            this->bid1vDelta = bids.getBestEntry().quantity - prevBid1v;
            this->ask1pDelta = asks.getBestEntry().price - prevAsk1p;
            this->ask1vDelta = asks.getBestEntry().quantity - prevAsk1v;
        }

        mdRefresh.clear();
    }
    return bookUpdated;
}

const std::string& DepthBook::getMatchEventIndicator() const
{
    return matchEventIndicator;
}
