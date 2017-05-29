#include <iostream>
#include <boost/asio.hpp>

const char ServerIP[] = "127.0.0.1";
const unsigned short PortNumber = 31400;

int main()
{
	system("title client");
	boost::asio::io_service io_service;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ServerIP), PortNumber);

	boost::system::error_code connect_error;
	boost::asio::ip::tcp::socket socket(io_service);
	socket.connect(endpoint, connect_error);

	if (connect_error)
	{
		std::cout << "���� ����. Error No. " << connect_error.value() << ", Message: " << connect_error.message() << std::endl;
		return 0;
	}
	else
		std::cout << "���� ����" << std::endl;

	for (int i = 0; i < 7; ++i)
	{
		char message[128] = { 0, };
		sprintf_s(message, 128 - 1, "%d - Send Message", i);
		size_t length = strnlen_s(message, 128 - 1);

		socket.write_some(boost::asio::buffer(message, length));

		std::cout << "������ ���� �޽���: " << message << std::endl;

		std::array<char, 128> buffer;
		buffer.assign(0);
		boost::system::error_code error;
		length = socket.read_some(boost::asio::buffer(buffer), error);

		if (error)
		{
			if (error == boost::asio::error::eof)
				std::cout << "���� ����" << std::endl;
			else
				std::cout << "Error No. " << error.value() << ", Message: " << error.message() << std::endl;
			break;
		}
		std::cout << "�����κ��� ���� �޽���: " << &buffer[0] << std::endl;
	}
	if (socket.is_open())
		socket.close();

	return 0;
}