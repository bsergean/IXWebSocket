/*
 *  IXSocketTest.cpp
 *  Author: Benjamin Sergeant
 *  Copyright (c) 2019 Machine Zone. All rights reserved.
 */

#include <iostream>
#include <ixwebsocket/IXSocket.h>
#include <ixwebsocket/IXCancellationRequest.h>

#include "IXTest.h"

#include "catch.hpp"

using namespace ix;

TEST_CASE("socket", "[socket]")
{
    SECTION("Connect to google server server. Send GET request without header. Should return 200")
    {
        Socket socket;
        std::string host("www.google.com");
        int port = 80;

        std::string errMsg;
        static std::atomic<bool> requestInitCancellation(false);
        auto isCancellationRequested =
            makeCancellationRequestWithTimeout(1, requestInitCancellation);

        bool success = socket.connect(host, port, errMsg, isCancellationRequested);
        REQUIRE(success);

        std::cout << "writeBytes" << std::endl;
        socket.writeBytes("GET / HTTP/1.1\r\n\r\n", isCancellationRequested);

        auto lineResult = socket.readLine(isCancellationRequested);
        auto lineValid = lineResult.first;
        auto line = lineResult.second;

        REQUIRE(lineValid);

        int status = -1;
        REQUIRE(sscanf(line.c_str(), "HTTP/1.1 %d", &status) == 1);
        REQUIRE(status == 200);
    }
}
