#include <iostream>
#include <boost/asio.hpp>

const unsigned short PortNumber = 31400;

int main()
{
	system("title server");
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), PortNumber);
	boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint);

	boost::asio::ip::tcp::socket socket(io_service);
	acceptor.accept(socket);
	std::cout << "클라이언트 접속" << std::endl;

	while (true)
	{
		std::array<char, 128> buf;
		buf.assign(0);
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error)
		{
			if (error == boost::asio::error::eof)
				std::cout << "연결 종료" << std::endl;
			else
				std::cout << "Error No." << error.value() << " Error Message : " << error.message() << std::endl;

			break;
		}
		std::cout << "클라이언트에서 받은 메시지:" << &buf[0] << std::endl;
		char message[128] = { 0, };
		sprintf_s(message, 128 - 1, "Re : %s", &buf[0]);
		len = strnlen_s(message, 128 - 1);

		socket.write_some(boost::asio::buffer(message, len));//ignored_error
		std::cout << "클라이언트에 보낸 메시지 : " << message << std::endl;
	}
	return 0;
}