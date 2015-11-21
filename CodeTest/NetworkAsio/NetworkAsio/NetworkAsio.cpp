// NetworkAsio.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { MAX_LENGTH = 1024 };

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
		return 1;
	}

	boost::asio::io_service io_service;

	tcp::socket socket(io_service);
	tcp::resolver resolver(io_service);
	boost::asio::connect(socket, resolver.resolve({ argv[1], argv[2] }));

	std::cout << "Enter message: ";
	char request[MAX_LENGTH];
	std::cin.getline(request, MAX_LENGTH);
	size_t request_length = std::strlen(request);
	boost::asio::write(socket, boost::asio::buffer(request, request_length) );

	char reply[MAX_LENGTH];
	::memset(reply, 0, MAX_LENGTH);
	size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, request_length));
	std::cout << "Reply is: ";
	std::cout.write(reply, reply_length);
	std::cout << "\n";

	return 0;
}

