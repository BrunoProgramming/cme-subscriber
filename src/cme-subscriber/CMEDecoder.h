//
// Created by antoine on 5/09/17.
//

#ifndef CPP_CME_SUBSCRIBER_CMEDECODER_H
#define CPP_CME_SUBSCRIBER_CMEDECODER_H

#include "sbe/MessageHeader.h"

class CMEDecoder {
public:
    MessageHeader& DecodeHeader(char* buffer, std::uint64_t offset, std::uint64_t bufferlength);
    std::size_t DecodeIncrementalRefreshBook();
    std::size_t DecodeIncrementalRefreshVolume();
    std::size_t DecodeIncrementalRefreshTradeSummary();
    std::size_t DecodeIncrementalRefreshOrderBook();

private:

};


#endif //CPP_CME_SUBSCRIBER_CMEDECODER_H
