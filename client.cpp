#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "client.hpp"
#include "requests.hpp"

void Client::RunClient() {
	conn.OpenConnection();
	
	string response;
	string message;

	string query_params;
	
	vector<string> cookies;
	//cookies.push_back("key1=value1");
	
	vector<string> body_data;
	body_data.push_back("key1=value1");
	body_data.push_back("key2=value2");

	message = compute_get_request(conn.GetHost(), "/api/v1/dummy", "", vector<string>{});
	cout << message << endl;

	conn.SendToServer(message);
	response = conn.ReceiveFromServer();
	cout << response << endl;


	message = compute_post_request(conn.GetHost(), "/api/v1/dummy", "application/x-www-form-urlencoded", body_data, vector<string>{});
	cout << message << endl;

	conn.SendToServer(message);
	response = conn.ReceiveFromServer();
	cout << response << endl;

	conn.CloseConnection();
}