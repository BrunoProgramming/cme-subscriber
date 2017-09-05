//
// Created by antoine on 5/09/17.
//

#include <iostream>
#include <sbe/MDIncrementalRefreshBook32.h>
#include <sbe/MDIncrementalRefreshOrderBook43.h>
#include "CMESubscriber.h"

constexpr int BYTE_OFFSET = 12;

void CMESubscriber::CMESubscriber(CMEFeed feed): feed_(feed) {
    feed_.AddDataHandler(OnData);
    feed_.JoinIncremental();
    StartRecovery();
}

void CMESubscriber::OnData(char* buffer, size_t bytes_recvd) {

    // Need to handle multiple packets chunked into a single message
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

    book_.Clear();

    // need to handle chunked packets
    // where multiple messages have been submitted over wire
    expected_seqnum_ = MDIncrementalRefreshOrderBook43.LastMsgSeqNumProcessed() + 1;

    auto &entry = incremental.noMDEntries();
    while(entry.hasNext()) {
        entry.next();
        book_.HandleMDEntry(entry.mDUpdateAction(),
                            entry.mDEntryType(),
                            entry.mDEntryPx(),
                            entry.mDEntrySize());
    }
}

void CMESubscriber::StartRecovery() {
    recovering_ = true;
    feed_.JoinSnapshot();
    // consider publishing unreliable book to handler
}

