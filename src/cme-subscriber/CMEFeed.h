//
// Created by antoine on 5/09/17.
//

#ifndef CPP_CME_SUBSCRIBER_CMEFEED_H
#define CPP_CME_SUBSCRIBER_CMEFEED_H

struct MulticastHost {
    int host;
    int port;
};

struct MulticastPair {
    MulticastHost primary;
    MulticastHost secondary;
};

struct CMEFeedConfig {

    MulticastPair incremental;
    MulticastPair snapshot;
};

class CMEFeed {
public:
    CMEFeed(CMEFeedConfig);
    void Join();

};


#endif //CPP_CME_SUBSCRIBER_CMEFEED_H
