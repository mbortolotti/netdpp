// Netdpp - Network interface to Ketek DPP dll server
//
// Tcp socket server adapted from blocking_tcp_echo_server example in boost library
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "dpp/axasd.h"
#include "dpp/dpp_exception.h"

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

const std::string APPNAME = "Netdpp";
const std::string VERSION = "0.2.0";
const int TCP_BUFFER_SIZE = 1024;

const std::string COMMAND_START = "start";
const std::string COMMAND_STOP = "stop";
const std::string COMMAND_CONTINUE = "continue";
const std::string COMMAND_ERASE = "erase";
const std::string COMMAND_DATA = "data";
const std::string COMMAND_STATUS = "status";
const std::string COMMAND_SETTINGS = "settings";

void dpp_session(socket_ptr sock)
{
	try
	{
		for (;;)
		{
			char data[TCP_BUFFER_SIZE];

			boost::system::error_code error;
			size_t length = sock->read_some(boost::asio::buffer(data), error);
			if (error == boost::asio::error::eof) {
				std::cout << "Connection closed cleanly by peer" << std::endl;
				break;
			}
			else if (error)
				throw boost::system::system_error(error); // Some other error.
			else {
				std::string input(data, length);
				if (input.find(COMMAND_START) != std::string::npos) 
				{
					axasd::getInstance().start(false);
				}
				else if (input.find(COMMAND_STOP) != std::string::npos) 
				{
					axasd::getInstance().stop();
				}
				else if (input.find(COMMAND_CONTINUE) != std::string::npos) 
				{
					axasd::getInstance().start(true);
				}
				else if (input.find(COMMAND_ERASE) != std::string::npos) 
				{
					axasd::getInstance().erase();
				}
				else if (input.find(COMMAND_DATA) != std::string::npos) 
				{
					std::string data = axasd::getInstance().getData();
					boost::asio::write(*sock, boost::asio::buffer(data, data.length()));
				}
				else if (input.find(COMMAND_STATUS) != std::string::npos) 
				{
					std::string status = axasd::getInstance().getStatus();
					boost::asio::write(*sock, boost::asio::buffer(status, status.length()));
				}
				else if (input.find(COMMAND_SETTINGS) != std::string::npos) 
				{
					std::string settings = axasd::getInstance().getSettings();
					boost::asio::write(*sock, boost::asio::buffer(settings, settings.length()));
				}
				else if (input.find("=") != std::string::npos) 
				{
					axasd::getInstance().updateSettings(input);
				}
			}
		}
	}
	catch (dpp_exception& dpp_ex)
	{
		std::cerr << "Dpp error: " << dpp_ex.what() << "\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void dpp_server(boost::asio::io_service& io_service, unsigned short port)
{
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	std::cout << APPNAME << " listening on port " << port << std::endl;
	for (;;)
	{
		socket_ptr sock(new tcp::socket(io_service));
		a.accept(*sock);
		boost::thread t(boost::bind(dpp_session, sock));
	}
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: " << APPNAME << " <port>\n";
			return 1;
		}
		std::cout << "Starting " << APPNAME << " version " << VERSION << " ..." << std::endl;

		axasd::getInstance().initDpp();

		boost::asio::io_service io_service;
		dpp_server(io_service, std::stoi(argv[1]));
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cerr << "Exiting " << APPNAME << std::endl;
	}

	return 0;
}
