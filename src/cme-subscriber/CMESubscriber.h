//
// Created by antoine on 5/09/17.
//

#ifndef CPP_CME_SUBSCRIBER_CMESUBSCRIBER_H
#define CPP_CME_SUBSCRIBER_CMESUBSCRIBER_H

#include <cinttypes>
#include "CMEFeed.h"
#include "CMEDecoder.h"
#include "Handler.h"
#include "sbe/MDIncrementalRefreshBook32.h"

class CMESubscriber {
public:
    CMESubscriber(CMEFeed);
    bool Subscribe(Handler handler, int64_t securityId);
    bool Unsubscribe();

private:

    DepthBook book_;
    CMEDecoder decoder_;

    int expected_seqnum_;
    bool recovery_mode_;

    void OnData(size_t);
    bool InRecoveryMode();
    void StartRecovery();
    void StopRecovery();
    void JoinIncremental();
    void LeaveIncremental();
    void HandleIncremental(MDIncrementalRefreshBook32&);
    void JoinSnapshot();
    void LeaveSnapshot();
    void HandleSnapshot();
};


#endif //CPP_CME_SUBSCRIBER_CMESUBSCRIBER_H
