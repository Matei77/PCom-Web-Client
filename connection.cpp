#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#include "connection.hpp"
#include "utils.hpp"
#include "client.hpp"

void Connection::OpenConnection() {
	int rc;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	
	rc = inet_pton(AF_INET, &host[0], &server_addr.sin_addr.s_addr);
    DIE(rc <= 0, "inet_pton");

	rc = connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
	DIE(rc < 0, "connect");
}

void Connection::CloseConnection() {
	close(sockfd);
}

void Connection::SendToServer(string message) {
	int bytes, sent = 0;
	int total = message.size();

	do {
		bytes = write(sockfd, &message[0] + sent, total - sent);
		DIE(bytes < 0, "write to socket");

		if (bytes == 0) {
			break;
		}
		sent += bytes;
	} while (sent < total);
}

string Connection::ReceiveFromServer() {
	string response;
	string buffer;
	long unsigned int header_end = 0;
	long unsigned int content_length = 0;

	do {
		response.assign(BUFLEN, '\0');
		int bytes = read(sockfd, &response[0], BUFLEN);
		DIE(bytes < 0, "read from socket");

		if (bytes == 0) {
			break;
		}

		buffer += response;

		header_end = buffer.find(HEADER_TERMINATOR);

		if (header_end != string::npos) {
			header_end += HEADER_TERMINATOR_SIZE;

			long unsigned int content_length_start = buffer.find(CONTENT_LENGTH);

			if (content_length_start == string::npos) {
				continue;
			}

			content_length_start += CONTENT_LENGTH_SIZE;
			content_length = strtol(&buffer[0] + content_length_start, NULL, 10);
			break;
		}
	} while(1);

	size_t total = content_length + (size_t) header_end;

	while (buffer.size() < total) {
		response.assign(BUFLEN, '\0');
        int bytes = read(sockfd, &response[0], BUFLEN);
		DIE(bytes < 0, "reading response from socket");

        if (bytes == 0) {
            break;
        }

        buffer += response;
    }

    return buffer;
}