//
//  main.cpp
//  uMail
//
//  Created by Marc Martinez on 2/11/14.
//  Copyright (c) 2014 the universal framework. All rights reserved.
//

#include <iostream>
#include "TCPClient.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

std::string getFileContent(std::string filePath){
    std::string fileContent = "";
    if(boost::filesystem::exists(filePath)){
        std::cout << "Reading File: " << filePath << std::endl;
        boost::filesystem::ifstream fileStream(filePath);
        while(fileStream){
            std::string contentLine = "";
            std::getline(fileStream, contentLine);
            if(!contentLine.empty()){
                fileContent += ( contentLine + "\r\n" );
            }
            
        }
    }
    return fileContent;
}




int main(int argc, const char * argv[]){
    std::string host = "";
    std::string port = "";
    std::string clientAddress = "";
    boost::asio::io_service ioService;
    std::string sessionFilePath = "";
    
    for(int i = 0; i < argc; i++){
        std::string cliArgument = argv[i];
        if(cliArgument == "-help"){
            std::cout << "./umail -s <session file path> -h <host> -p <port> -b <client address>" << std::endl;
        }
        else if(cliArgument == "-h"){
            host = argv[(i + 1)];
            std::cout << "Host: " << host << std::endl;
        }
        else if(cliArgument == "-s"){
            sessionFilePath = argv[(i+1)];
            std::cout << "Session File Path: " << sessionFilePath << std::endl;
        }
        else if(cliArgument == "-p"){
            port = argv[(i + 1)];
            std::cout << "Port: " << port << std::endl;
        }
        else if(cliArgument == "-b"){
            clientAddress = argv[(i + 1)];
            std::cout << "Client Address: " << clientAddress << std::endl;
        }
    }
    
    if(!sessionFilePath.empty() && !port.empty() && !host.empty()){
        if(boost::filesystem::exists(sessionFilePath)){
            try{
                boost::asio::ip::tcp::resolver resolver(ioService);
                boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, port);
                boost::asio::ip::tcp::resolver::iterator endPointIterator = resolver.resolve(query);
                boost::asio::ip::tcp::resolver::iterator endPoint;
                
                std::string sessionContent = getFileContent(sessionFilePath);
                
                TCPClient *tcpClient;
                tcpClient = new TCPClient(ioService, endPointIterator, sessionContent, clientAddress);
                
                ioService.run();
                
            }
            catch(std::exception &e){
                std::cerr << e.what() << std::endl;
            }
        }
    }
    return 0;
}

