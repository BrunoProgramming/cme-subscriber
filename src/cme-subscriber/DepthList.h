//
// Created by antoine on 4/09/17.
//

#ifndef CPP_CME_SUBSCRIBER_DEPTHLIST_H
#define CPP_CME_SUBSCRIBER_DEPTHLIST_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>

enum MDEntryType { New, Change, Delete };

struct PriceEntry {
    double price;
    int quantity;

    PriceEntry(double price, int quantity) : price(price), quantity(quantity) {}

    friend std::ostream& operator<<(std::ostream& os, const PriceEntry& entry)
    {
        os << entry.price << ',' << entry.quantity;
        return os;
    }
};

class DepthList {

private:
    std::vector<PriceEntry> entries_;
    bool isbuy_;

    int maxDepthSupported = 10; // mve to constructor
    int maxDepthKnown = 0;

    int updateCount;

    void remove(int, double);


public:
    DepthList(const bool&); // simple constructor
    DepthList(const DepthList&); // copy constructor
    DepthList& operator=(const DepthList& rhs); // assignment operator

    void insert(double, int, int, MDEntryType);
    PriceEntry getBestEntry();

    const std::vector<PriceEntry>& getEntries() const
    {
        return entries_;
    }

    void reset()
    {
        entries_.clear();
        maxDepthKnown = 0;
        clear();
    }

    void clear() { updateCount = 0; }

    int getUpdateCount() const { return updateCount; }

    friend std::ostream& operator<<(std::ostream& os, const DepthList& list)
    {
        // Output in CSV
        // Column count needs to be constant to avoid misaligns

        int count = 0;
        for (auto&& entry : list.getEntries()) {
            os << *entry << ',';
            ++count;
        }

        // Fill blank columns if book size < max depth
        for (int i = list.getMaxDepthSupported() - count - 1; i >= 0; --i) {
            os << ",,";
        }

        os << list.getUpdateCount();
        return os;
    }
};


#endif //CPP_CME_SUBSCRIBER_DEPTHLIST_H
