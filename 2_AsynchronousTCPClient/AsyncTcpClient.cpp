#include <boost/bind.hpp>
#include "AsyncTcpClient.h"

AsyncTcpClient::AsyncTcpClient(boost::asio::io_service& io_service)
	:io_service(io_service),
	socket(io_service),
	seq_number(0)
{

}

void AsyncTcpClient::Connect(boost::asio::ip::tcp::endpoint& endpoint)
{
	socket.async_connect
	(
		endpoint,
		boost::bind
		(
			&AsyncTcpClient::handle_connect,
			this,
			boost::asio::placeholders::error
		)
	);
}

void AsyncTcpClient::post_write()
{
	if (socket.is_open() == false)
		return;

	if (seq_number > 7)
	{
		socket.close();
		return;
	}

	++seq_number;

	char message[128] = { 0, };
	sprintf_s(message, 128 - 1, "%d - Send Message", seq_number);

	write_message = message;

	boost::asio::async_write
	(
		socket,
		boost::asio::buffer(write_message),
		boost::bind
		(
			&AsyncTcpClient::handle_write,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);

	post_receive();
}

void AsyncTcpClient::post_receive()
{
	memset(&receive_buffer, 0x00, sizeof(receive_buffer));

	socket.async_read_some
	(
		boost::asio::buffer(receive_buffer),
		boost::bind
		(
			&AsyncTcpClient::handle_receive,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void AsyncTcpClient::handle_connect(const boost::system::error_code& error)
{
	if (error)
		std::cout << "Connection Failed : " << error.message() << std::endl;
	else
	{
		std::cout << "Connected" << std::endl;
		post_write();
	}
}

void AsyncTcpClient::handle_write(const boost::system::error_code&, size_t)
{
}

void AsyncTcpClient::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
			std::cout << "서버와 연결이 끊어졌습니다." << std::endl;
		else
			std::cout << "Error No. " << error.value() << " Error Message : " << error.message() << std::endl;
	}
	else
	{
		const std::string receive_message = receive_buffer.data();
		std::cout << "서버에서 받은 메시지: " << receive_message << ", 받은 크기: " << bytes_transferred << std::endl;
		post_write();
	}
}
