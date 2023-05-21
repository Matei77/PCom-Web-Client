// Copyright Ionescu Matei-Stefan - 323CAb - 2022-2023
#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <netinet/in.h>
#include <string>

using namespace std;

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)

#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

#define BUFLEN 4096

class Connection {
	private:
		int sockfd;
		uint16_t port;
		string host;
		sockaddr_in server_addr;

	public:
		Connection(uint16_t port, string host);

		void OpenConnection();
		void CloseConnection();
		void SendToServer(string message);
		string ReceiveFromServer();

		string GetHost();
};

inline Connection::Connection(uint16_t port, string host) {
	this->port = port;
	this->host = host;
}

inline string Connection::GetHost() {
	return this->host;
}

#endif  // CONNECTION_HPP_