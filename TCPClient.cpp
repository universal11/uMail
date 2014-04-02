//
//  TCPClient.cpp
//  uMail
//
//  Created by Marc Martinez on 3/8/14.
//  Copyright (c) 2014 the universal framework. All rights reserved.
//

#include "TCPClient.h"
#include <boost/asio.hpp>
#include<boost/array.hpp>
#include <boost/thread/thread.hpp>

TCPClient::TCPClient(boost::asio::io_service &_ioService, boost::asio::ip::tcp::resolver::iterator endpointIterator, std::string data, std::string clientAddress) : ioService(_ioService), socket(_ioService){
    std::cout << "Spawning TCP Client" << std::endl;
    this->data = data;
    this->clientAddress = clientAddress;
    this->socket.open(boost::asio::ip::tcp::v4());
    this->socket.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(clientAddress), 0));
    this->socket.async_connect(*endpointIterator++, boost::bind(&TCPClient::handleConnect, this, boost::asio::placeholders::error));
}

void TCPClient::handleConnect(const boost::system::error_code &error){
    std::cout << "Sending Data: " << this->data << std::endl;
    this->write(this->data);
    this->write("\r\n");
    boost::asio::async_read(this->socket, boost::asio::buffer(this->readBuffer, 8), boost::bind(&TCPClient::handleRead, this, boost::asio::placeholders::error));
}

void TCPClient::read(){
    boost::asio::async_read(this->socket, boost::asio::buffer(this->readBuffer, 8), boost::bind(&TCPClient::handleRead, this, boost::asio::placeholders::error));
    
}

void TCPClient::handleRead(const boost::system::error_code &error){
    if(!error){
        this->response += std::string(this->readBuffer.data(), 8);
        boost::asio::async_read(this->socket, boost::asio::buffer(this->readBuffer, 8), boost::bind(&TCPClient::handleRead, this, boost::asio::placeholders::error));
    }
    else{
        this->close();
    }
}

void TCPClient::close(){
    std::cout << "Response: " << this->response << std::endl;
    std::cout << "Closing socket!" << std::endl;
    this->socket.close();
}

void TCPClient::write(std::string data){
    boost::asio::write(this->socket, boost::asio::buffer(data));
}