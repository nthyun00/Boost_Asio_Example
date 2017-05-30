#include "AsyncTcpServer.h"
#include "Session.h"
#include <iostream>



int main()
{
	system("title server");
	boost::asio::io_service io_service;
	AsyncTcpServer server(io_service);

	io_service.run();

	std::cout << "匙飘况农 立加 辆丰" << std::endl;

	return 0;
}