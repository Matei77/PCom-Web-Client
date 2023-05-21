#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "client.hpp"
#include "requests.hpp"

void Client::RunClient() {
	conn.OpenConnection();

	string query_params;
	vector<string> cookies;
	cookies.push_back("key1=value1");
	string message;

	message = compute_get_request(conn.GetHost(), "/api/v1/dummy", query_params, cookies);
	cout <<"Generated message:\n" << message << endl;

	conn.SendToServer(message);

	string response;
	response = conn.ReceiveFromServer();
	cout << response << endl;

	conn.CloseConnection();
}