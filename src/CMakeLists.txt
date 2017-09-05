
set(SOURCE_FILES
        cme-subscriber/DepthList.h
        cme-subscriber/DepthList.cpp cme-subscriber/Handler.cpp cme-subscriber/Handler.h cme-subscriber/CMESubscriber.cpp cme-subscriber/CMESubscriber.h cme-subscriber/CMEFeed.cpp cme-subscriber/CMEFeed.h cme-subscriber/CMEDecoder.cpp cme-subscriber/CMEDecoder.h)

add_library(DepthBook SHARED ${SOURCE_FILES})
