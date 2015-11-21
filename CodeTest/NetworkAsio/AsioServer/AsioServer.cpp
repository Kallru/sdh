// AsioServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <vector>

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

std::vector<socket_ptr> g_clinets;

void session(socket_ptr sock)
{
	for (;;)
	{
		char data[max_length];
		::memset(data, 0, max_length);

		boost::system::error_code error;
		size_t length = sock->read_some(boost::asio::buffer(data), error);
		if (error == boost::asio::error::eof)
			break; // Connection closed cleanly by peer.
		else if (error)
			throw boost::system::system_error(error); // Some other error.

		// boost::asio::write(*sock, boost::asio::buffer(data, length));

		for (int i = 0; i < g_clinets.size(); ++i)
		{
			boost::asio::write(*g_clinets[i], boost::asio::buffer(data, length));
		}

		std::cout << "Client Data is :";
		std::cout.write(data, length);
		std::cout << "\n";
	}
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		socket_ptr sock(new tcp::socket(io_service));
		a.accept(*sock);

		g_clinets.push_back(sock);
		boost::thread t(boost::bind(session, sock));
	}
}

int main(int argc, char* argv[])
{
	int port = 123456;

	boost::asio::io_service io_service;
	server(io_service, port);

	return 0;
}


