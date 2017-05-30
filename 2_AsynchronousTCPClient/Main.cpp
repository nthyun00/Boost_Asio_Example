#include "AsyncTcpClient.h"

int main()
{
	system("title client");
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 31400);

	AsyncTcpClient client(io_service);
	client.Connect(endpoint);
	io_service.run();
	std::cout << "匙飘况农 立加 辆丰" << std::endl;
	return 0;
}