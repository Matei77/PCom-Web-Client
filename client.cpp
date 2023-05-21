#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "client.hpp"
#include "requests.hpp"

void Client::RunClient() {
	string command;

	do {
		getline(cin, command);

		conn.OpenConnection();

		if (command == "register") {
			//cout << "register" << endl;
			Register();

		} else if (command == "login") {
			//cout << "login" << endl;
			Login();

		} else if (command == "enter_library") {
			cout << "enter_library" << endl;
		
		} else if (command == "get_books") {
			cout << "get_books" << endl;
		
		} else if (command == "get_book") {
			cout << "get_book" << endl;
		
		} else if (command == "add_book") {
			cout << "add_book" << endl;
		
		} else if (command == "delete_book") {
			cout << "delete_book" << endl;
		
		} else if (command == "logout") {
			cout << "logout" << endl;
		
		} else if (command == "exit") {
			break;

		} else {
			cout << "Unrecognized command." << endl;
		} 

		conn.CloseConnection();

	} while (1);
}


void Client::Register() {
	string message;
	string response;

	string username;
	string password;

	vector<string> body_data;
	
	cout << "username=";
	getline(cin, username);
	cout << "password=";
	getline(cin, password);

	if (username.find(" ") != string::npos || password.find(" ") != string::npos) {
		cout << "Spaces are not allowed in username/password." << endl << endl;

		return;
	}

	body_data.push_back("\"username\": \"" + username + "\"");
	body_data.push_back("\"password\": \"" + password + "\"");


	message = compute_post_request(conn.GetHost(), REGISTER_URL, JSON_PAYLOAD, body_data, vector<string>{});
	// cout << endl << message << endl;

	conn.SendToServer(message);
	response = conn.ReceiveFromServer();
	// cout << response << endl;

	if (response.find("201 Created") != string::npos) {
		cout << "Account was created successfully." << endl << endl;
	} else {
		cout << "The username is already taken." << endl << endl;
	}
}


void Client::Login() {
	string message;
	string response;

	string username;
	string password;

	vector<string> body_data;

	if (GetLoggedIn()) {
		cout << "You are already logged in." << endl << endl;
		return;
	}
	
	cout << "username=";
	getline(cin, username);
	cout << "password=";
	getline(cin, password);

	if (username.find(" ") != string::npos || password.find(" ") != string::npos) {
		cout << "Spaces are not allowed in username/password." << endl << endl;
		return;
	}

	body_data.push_back("\"username\": \"" + username + "\"");
	body_data.push_back("\"password\": \"" + password + "\"");

	message = compute_post_request(conn.GetHost(), LOGIN_URL, JSON_PAYLOAD, body_data, vector<string>{});
	//cout << endl << message << endl;

	conn.SendToServer(message);
	response = conn.ReceiveFromServer();
	//cout << response << endl;

	if (response.find("200 OK") != string::npos) {
		cout << "Logged in successfully." << endl << endl;

	} else if (response.find("No account with this username!") != string::npos) {
		cout << "Loggin failed. There is no account with this username." << endl << endl;
		return;

	} else if (response.find("Credentials are not good!") != string::npos) {
		cout << "Loggin failed. Bad credentials." << endl << endl;
		return;

	} else {
		cout << "Loggin failed." << endl << endl;
		return;
	}

	string cookie;
	auto start_pos = response.find("connect.sid");
	auto end_pos = response.find(";", start_pos);

	cookie = response.substr(start_pos, end_pos - start_pos);
	//cout << cookie << endl;

	cookies.push_back(cookie);
	SetLoggedIn(true);
}


void Client::EnterLibrary() {

}


void Client::GetBooks() {

}


void Client::GetBook() {

}


void Client::AddBook() {

}


void Client::DeleteBook() {

}


void Client::Logout() {

}


void Client::Exit() {

}

