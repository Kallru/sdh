// NetworkAsio.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

using boost::asio::ip::tcp;

enum { MAX_LENGTH = 1024 };

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void recv(socket_ptr sock)
{
	while (true)
	{
		char reply[MAX_LENGTH];
		::memset(reply, 0, MAX_LENGTH);

		boost::system::error_code error;
		size_t reply_length = sock->read_some(boost::asio::buffer(reply), error);

		std::cout << "\n";
		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		//std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
		//return 1;
	}

	boost::asio::io_service io_service;

	socket_ptr pSocket(new tcp::socket(io_service));
	tcp::resolver resolver(io_service);
	//boost::asio::connect(socket, resolver.resolve({ argv[1], argv[2] }));
	boost::asio::connect(*pSocket, resolver.resolve({ "192.168.1.57", "123456" }));

	// 리시브 스레드 생성
	boost::thread rc(boost::bind(recv, pSocket));

	while (true)
	{
		std::cout << "Enter message: ";
		char request[MAX_LENGTH];
		std::cin.getline(request, MAX_LENGTH);
		size_t request_length = std::strlen(request);
		boost::asio::write(*pSocket, boost::asio::buffer(request, request_length));
	}	

	return 0;
}

