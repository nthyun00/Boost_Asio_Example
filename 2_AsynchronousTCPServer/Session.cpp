#include "Session.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <boost/bind.hpp>

Session::Session(boost::asio::io_service& io_service)
	:socket(io_service)
{

}

Session::~Session()
{

}

boost::asio::ip::tcp::socket& Session::Socket()
{
	return this->socket;
}

void Session::PostReceive()
{
	memset(&buffer, 0x00, sizeof(buffer));
	socket.async_read_some
	(
		boost::asio::buffer(buffer),
		boost::bind
		(
			&Session::handle_receive,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

}

void Session::handle_write(const boost::system::error_code&, size_t)
{

}

void Session::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
			std::cout << "연결 종료" << std::endl;
		else
			std::cout << "Error No." << error.value() << " Error Message : " << error.message() << std::endl;
	}
	else
	{
		std::cout << "클라이언트에서 받은 메시지: " << buffer.data() << ", 받은 크기: " << bytes_transferred << std::endl;
		char message[128] = { 0, };
		sprintf_s(message, 128 - 1, "Re:%s", buffer.data());
		this->message = message;

		boost::asio::async_write
		(
			socket, boost::asio::buffer(message),
			boost::bind
			(
				&Session::handle_write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);

		PostReceive();
	}
}
