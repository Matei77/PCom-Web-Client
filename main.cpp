#include "client.hpp"

#define PORT 8080
#define SERVER_ADDR "34.254.242.81"

int main() {
	Client client(PORT, SERVER_ADDR);
	client.RunClient();
}