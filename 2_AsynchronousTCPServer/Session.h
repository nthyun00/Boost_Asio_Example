#pragma once
#include <string>
#include <boost/asio.hpp>

class Session
{
public:
	Session(boost::asio::io_service&);
	~Session();
	boost::asio::ip::tcp::socket& Socket();
	void PostReceive();

private:
	void handle_write(const boost::system::error_code&, size_t);
	void handle_receive(const boost::system::error_code& , size_t);
	
	boost::asio::ip::tcp::socket socket;
	std::string message;
	std::array<char, 128> buffer;
};