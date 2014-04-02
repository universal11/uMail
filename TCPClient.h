//
//  TCPClient.h
//  uMail
//
//  Created by Marc Martinez on 3/8/14.
//  Copyright (c) 2014 the universal framework. All rights reserved.
//

#ifndef __uMail__TCPClient__
#define __uMail__TCPClient__

#include <iostream>
#include <boost/asio.hpp>
#include<boost/array.hpp>
#include <boost/thread/thread.hpp>

#endif /* defined(__uMail__TCPClient__) */

class TCPClient{
public:
    TCPClient(boost::asio::io_service &_ioService);
    TCPClient(boost::asio::io_service &_ioService, boost::asio::ip::tcp::resolver::iterator endpointIterator, std::string data, std::string clientAddress);
    void read();
    void write(std::string data);
    void close();
    void send(std::string header, std::string data);
private:
    void handleConnect(const boost::system::error_code &error);
    void handleRead(const boost::system::error_code &error);
    boost::asio::io_service &ioService;
    boost::asio::ip::tcp::socket socket;
    boost::array<char, 1024> readBuffer;
    std::string response;
    std::string data;
    std::string clientAddress;

};