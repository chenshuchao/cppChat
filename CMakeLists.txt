PROJECT(CPPCHAT)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_DEFINITIONS(-O3 -Wall -g)
IF(APPLE) # mac os
    ADD_DEFINITIONS(-std=c++11)
endif()

ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)

ENABLE_TESTING()
ADD_TEST(NAME test COMMAND test.run)

