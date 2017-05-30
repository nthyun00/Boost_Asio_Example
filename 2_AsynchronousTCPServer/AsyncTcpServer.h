#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "Session.h"

class AsyncTcpServer
{
public:
	AsyncTcpServer(boost::asio::io_service&);
	~AsyncTcpServer();

private:
	void start_accept();
	void handle_accept(std::shared_ptr<Session>&, const boost::system::error_code&);

	int seq_number;
	boost::asio::ip::tcp::acceptor acceptor;
	std::shared_ptr<Session> session;
};
