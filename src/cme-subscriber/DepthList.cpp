//
// Created by antoine on 4/09/17.
//

#include "DepthList.h"

DepthList::DepthList(const bool& isBuy)
        : maxDepthKnown(0)
        , isBuy(isBuy)
        , updateCount(0)
{
    entries_.reserve(maxDepthSupported);
}

DepthList::DepthList(const DepthList& rhs)
{
    // create *new* entries_
    // default would simply increment reference count on shared_pointer
    for (auto e : rhs.getEntries()) {
        e = std::make_unique<PriceEntry>(*e);
    }
}

DepthList& DepthList::operator=(const DepthList& rhs)
{
    // create *new* entries_
    // default would simply increment reference count on shared_pointer
    for (auto e : rhs.getEntries()) {
        e = std::make_unique<PriceEntry>(*e);
    }
    return *this;
}

void DepthList::remove(int level, double price)
{
    if ((level < maxDepthKnown)
        && (byPrice.find(price) != byPrice.end())) // validate price exists in map
    {
        entries_.erase(entries_.begin() + level);
        byPrice.erase(price);
        --maxDepthKnown;
    } else {
        std::cout << "Problem detected in depth cache - price level " << price << " does not exist" << std::endl;
    }
}

void DepthList::insert(double price, int quantity, int levelIndex, MDEntryType updateType)
{
    int level = levelIndex - 1; // internal level representation has offset of 1

    // REMOVE
    if (updateType == Delete) {
        remove(level, price);
        ++updateCount;
    }
        // ADD
    else if (updateType == New) {
        if (level < maxDepthKnown + 1) {
            if ((level < maxDepthKnown) && (entries_[level]->price == price)) {
                std::cout << "Depth Item at new price already exists: Level=" << level << " - Price=" << price
                          << std::endl;
            } else {

                // update book entry
                auto e = std::make_shared<PriceEntry>(price, quantity);
                entries_.insert(entries_.begin() + level, e);
                byPrice[price] = e.get();

                // implicit removal of last level in book
                // if size of book has exceeded max due to recent insert
                if (entries_.size() == maxDepthSupported + 1) {
                    byPrice.erase(entries_[maxDepthSupported]->price);
                    entries_.erase(entries_.begin() + maxDepthSupported);
                    --maxDepthKnown;
                }
                ++updateCount;
                ++maxDepthKnown;
            }
        } else {
            std::cout << "Trying to insert Depth Item outside the maximum known level of depth: "
                      << "Level=" << levelIndex << " - "
                      << "Price=" << price
                      << std::endl;
        }
    }
        // CHANGE
    else if (updateType == Change) {
        if (level < maxDepthKnown) {

            // update book entry
            if (entries_[level]->price == price) {
                // implicit update of entry in byPrice map due to shared pointer
                entries_[level]->quantity = quantity;
                ++updateCount;
            } else {
                std::cout << "Depth Item at given price for modification does not exist : " << price << std::endl;
            }
        } else {
            std::cout << "Depth Item at given price for modification does not exist : " << price << std::endl;
        }
    } else {
        std::cout << "Depth Item update type is invalid :" << updateType << std::endl;
    }
}

PriceEntry DepthList::getBestEntry()
{
    if (this->entries_.size() > 0) {
        return *(entries_[0]);
    }
    return PriceEntry{ 0, 0 };
}
