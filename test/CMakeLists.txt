
# Start Test Definitions Here
include_directories(../src)

add_executable(tests src/cme-subscriber/DepthListTest.cpp src/main.cpp)

target_link_libraries(
        tests
        DepthBook
        gtest
        gtest_main)

add_test(NAME main_test COMMAND tests)