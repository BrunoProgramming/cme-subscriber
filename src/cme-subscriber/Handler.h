//
// Created by antoine on 5/09/17.
//

#ifndef CPP_CME_SUBSCRIBER_HANDLER_H
#define CPP_CME_SUBSCRIBER_HANDLER_H

#include <iostream>
#include "DepthBook.h"
#include "CMEFeed.h"
#include "CMESubscriber.h"

class Handler {
public:
    void OnDepth(DepthBook *) {
        std::cout << "depthbook" << '\n';
    }
    void OnTrade(DepthBook *) {} // tradebook?
    void OnStatus(DepthBook *) {} // status?
};

int main() {

    Handler h;
    CMESubscriber s(CMEFeed());
    auto subscribed = s.Subscribe(h, 213456);

}

#endif //CPP_CME_SUBSCRIBER_HANDLER_H
