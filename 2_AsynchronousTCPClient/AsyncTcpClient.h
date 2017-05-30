#pragma once
#include <iostream>
#include <boost/asio.hpp>

class AsyncTcpClient
{
public:
	AsyncTcpClient(boost::asio::io_service&);
	void Connect(boost::asio::ip::tcp::endpoint&);

private:
	void post_write();
	void post_receive();
	void handle_connect(const boost::system::error_code&);
	void handle_write(const boost::system::error_code&, size_t);
	void handle_receive(const boost::system::error_code&, size_t);

	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::socket socket;
	int seq_number;
	std::array<char, 128> receive_buffer;
	std::string write_message;
};