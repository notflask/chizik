add_executable(chizik_tests 
    httprequest_test.cpp 
    httpresponse_test.cpp 
    socket_test.cpp
    multithreading_test.cpp
)

target_link_libraries(chizik_tests PRIVATE chizik GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(chizik_tests)
