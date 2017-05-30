#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

class Session
{
public:
	Session(boost::asio::io_service& io_service)
		:socket(io_service)
	{

	}

	~Session()
	{

	}

	boost::asio::ip::tcp::socket& Socket()
	{
		return this->socket;
	}

	void PostReceive()
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
private:
	void handle_write(const boost::system::error_code&, size_t)
	{

	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
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

	boost::asio::ip::tcp::socket socket;
	std::string message;
	std::array<char, 128> buffer;
};

const unsigned short PortNumber = 31400;

class AsyncTcpServer
{
public:
	AsyncTcpServer(boost::asio::io_service& io_service)
		:acceptor(io_service,
			boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PortNumber))
	{
		//session = std::make_shared<Session>(nullptr);
		start_accept();
	}
	~AsyncTcpServer()
	{

	}

private:
	void start_accept()
	{
		std::cout << "클라이언트 접속 대기..." << std::endl;
		session = std::make_shared<Session>(acceptor.get_io_service());
		acceptor.async_accept(
			session->Socket(),
			boost::bind
			(
				&AsyncTcpServer::handle_accept, 
				this,
				session, 
				boost::asio::placeholders::error
			)
		);
	}

	void handle_accept(std::shared_ptr<Session>& session, const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "클라이언트 접속 성공" << std::endl;
			session->PostReceive();
		}
	}

	int seq_number;
	boost::asio::ip::tcp::acceptor acceptor;
	std::shared_ptr<Session> session;
};

int main()
{
	system("title server");
	boost::asio::io_service io_service;
	AsyncTcpServer server(io_service);

	io_service.run();

	std::cout << "네트워크 접속 종료" << std::endl;

	return 0;
}