#include <netinet/in.h>
#include <string>

using namespace std;

class Connection {
	private:
		int sockfd;
		uint16_t port;
		string host_addr;
		sockaddr_in server_addr;

	public:
		Connection(uint16_t port, string host_addr);
		void OpenConnection();
		void CloseConnection();
		void SendToServer(string message);
		string receive_from_server();
};

inline Connection::Connection(uint16_t port, string host_addr) {
	this->port = port;
	this->host_addr = host_addr;
}