//
// Created by antoine on 5/09/17.
//

#include <iostream>
#include <sbe/MDIncrementalRefreshBook32.h>
#include <sbe/MDIncrementalRefreshOrderBook43.h>
#include "CMESubscriber.h"

constexpr int BYTE_OFFSET = 12;

void CMESubscriber::OnData(char* buffer, size_t bytes_recvd) {

    int bytes_processed = BYTE_OFFSET;
    while (bytes_processed < bytes_recvd) {

        auto &header = decoder_.DecodeHeader(buffer, bytes_recvd, bytes_recvd);

        switch (header.templateId()) {

            case sbe::MDIncrementalRefreshBook32::sbeTemplateId():
                HandleIncremental(decoder_.DecodeIncrementalRefreshBook());
                break;

        }
    }
}

void CMESubscriber::HandleIncremental(MDIncrementalRefreshBook32& incremental) {
    auto &entry = incremental.noMDEntries();
    while(entry.hasNext()) {
        entry.next();
        auto seqnum = entry.rptSeq();
        if (seqnum == expected_seqnum_) {
            if(InRecoveryMode())  { StopRecovery(); }
            expected_seqnum_++;
            book_.HandleMDEntry(entry.mDUpdateAction(),
                                entry.mDEntryType(),
                                entry.mDEntryPx(),
                                entry.mDEntrySize());
        } else if(seqnum > expected_seqnum_ ) {
            if (!InRecoveryMode()) {
                StartRecovery();
            }
        } else {
            // Redundant packet received
        }
    }
}

void CMESubscriber::HandleSnapshot(MDIncrementalRefreshOrderBook43& refresh) {
    if(!InRecoveryMode()) { return; } // handle race whereby inflight packets may arrivae after recovery was successful

    refresh.rp


}



    header
    auto seqnum = decoder_;//.SeqNum(data);

    // Incremental
    for (auto i&: decoder_.GetEntries()) {

        if (i.SeqNum() == NextExpectedIncremental()) {
            if (!InRecoveryMode()) {
                ProcessIncremental();
            }
            ++incremental_seqnum_;
        } else if (i.SeqNum() > NextExpectedIncremental() && !InRecoveryMode()) {
            StartRecovery();
        }
    }

    // Snapshot
    for (auto i& decoder_.GetEntries()) {
        ProcessSnapshot();
        if (Recovered()) {
            StopRecovery();
        }
    }
}

void CMESubscriber::HandleIncremental(...) {

    incremental_seqnum_ = i.GetSeqNum();

    switch (i.Type) {
        case Insert:
            book_.insert();
            break;
    }

}

void CMESubscriber::ProcessSnapshot() {

}

bool CMESubscriber::Recovered() {
    return (incremental_seqnum_ != snapshot_seqnum_);
}

int CMESubscriber::NextExpectedIncremental() {
   return incremental_seqnum_+1;
}

void CMESubscriber::StartRecovery() {
    feed_.join();
    // consider publishing unreliable book to handler
}

