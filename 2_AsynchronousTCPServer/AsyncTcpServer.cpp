#include "AsyncTcpServer.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <boost/bind.hpp>

AsyncTcpServer::AsyncTcpServer(boost::asio::io_service& io_service)
	:acceptor(io_service,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 31400))
{
	//session = std::make_shared<Session>(nullptr);
	start_accept();
}

AsyncTcpServer::~AsyncTcpServer()
{

}

void AsyncTcpServer::start_accept()
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

void AsyncTcpServer::handle_accept(std::shared_ptr<Session>& session, const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "클라이언트 접속 성공" << std::endl;
		session->PostReceive();
	}
}



