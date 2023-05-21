#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

#include "client.hpp"
#include "requests.hpp"
#include "utils.hpp"
#include "json.hpp"

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
			//cout << "enter_library" << endl;
			EnterLibrary();

		} else if (command == "get_books") {
			//cout << "get_books" << endl;
			GetBooks();

		} else if (command == "get_book") {
			//cout << "get_book" << endl;
			GetBook();

		} else if (command == "add_book") {
			//cout << "add_book" << endl;
			AddBook();

		} else if (command == "delete_book") {
			//cout << "delete_book" << endl;
			DeleteBook();
		
		} else if (command == "logout") {
			//cout << "logout" << endl;
			Logout();
		
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


	message = ComputePostRequest(conn.GetHost(), REGISTER_URL, JSON_PAYLOAD, body_data, vector<string>{}, "");
	// cout << endl << message << endl;

	conn.SendToServer(message);
	response = conn.ReceiveFromServer();
	// cout << response << endl;

	if (response.find("HTTP/1.1 201 Created") != string::npos) {
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

	if (logged_in) {
		cout << "You are already logged in." << endl << endl;
		return;
	}
	
	cout << "username="; getline(cin, username);
	cout << "password="; getline(cin, password);

	if (username.find(" ") != string::npos || password.find(" ") != string::npos) {
		cout << "Spaces are not allowed in username/password." << endl << endl;
		return;
	}

	body_data.push_back("\"username\": \"" + username + "\"");
	body_data.push_back("\"password\": \"" + password + "\"");

	message = ComputePostRequest(conn.GetHost(), LOGIN_URL, JSON_PAYLOAD, body_data, vector<string>{}, "");
	//cout << endl << message << endl;

	conn.SendToServer(message);
	response = conn.ReceiveFromServer();
	//cout << response << endl;

	if (response.find("HTTP/1.1 200 OK") != string::npos) {
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

	auto start_pos = response.find("connect.sid");
	auto end_pos = response.find(";", start_pos);

	string cookie = response.substr(start_pos, end_pos - start_pos);

	cookies.push_back(cookie);
	logged_in = true;
}


void Client::EnterLibrary() {
	string message;
	string response;

	if (!logged_in) {
		cout << "You are not logged in." << endl << endl;
		return;
	}

	message = ComputeGetRequest(conn.GetHost(), ACCESS_URL, "", cookies, "");
	
	conn.SendToServer(message);

	response = conn.ReceiveFromServer();
	// TODO: check response

	auto start_pos = response.find("\"token\":\"");
	start_pos += strlen("\"token\":\"");
	
	auto end_pos = response.find("\"", start_pos);
	
	jwt_token = response.substr(start_pos, end_pos - start_pos);

	cout << "Successfully entered library." << endl << endl;
}


void Client::GetBooks() {
	string message;
	string response;

	if (jwt_token.empty()) {
		cout << "You are not authorized. Enter library first." << endl << endl;
		return;
	}

	message = ComputeGetRequest(conn.GetHost(), BOOKS_URL, "", cookies, jwt_token);
	
	conn.SendToServer(message);

	response = conn.ReceiveFromServer();

	auto pos = response.find(HEADER_TERMINATOR);
	pos += HEADER_TERMINATOR_SIZE;

	string json_str = response.substr(pos);

	nlohmann::json json_data = nlohmann::json::parse(json_str);

	for (const auto & item : json_data) {
		int id = item["id"];
		string title = item["title"];

		cout << "{id=" << id << "; " << "title=" << title << "}" << endl;
	}
}


void Client::GetBook() {
	string message;
	string response;

	string id;

	if (jwt_token.empty()) {
		cout << "You are not authorized. Enter library first." << endl << endl;
		return;
	}

	cout << "id="; getline(cin, id);

	if (!isStringNumerical(id)) {
		cout << "Invalid id." << endl << endl;
		return;
	}

	string url = BOOKS_URL;
	url += "/" + id;

	message = ComputeGetRequest(conn.GetHost(), url, "", cookies, jwt_token);
	
	conn.SendToServer(message);

	response = conn.ReceiveFromServer();
	
	auto pos = response.find(HEADER_TERMINATOR);
	pos += HEADER_TERMINATOR_SIZE;

	string json_str = response.substr(pos);

	nlohmann::json json_data = nlohmann::json::parse(json_str);

	string title = json_data["title"];
	string author = json_data["author"];
	string genre = json_data["genre"];
	string publisher = json_data["publisher"];
	int page_count = json_data["page_count"];

	cout << "title=" << title << endl;
	cout << "author=" << author << endl;
	cout << "genre=" << genre << endl;
	cout << "publisher=" << publisher << endl;
	cout << "page_count=" << page_count << endl;
	cout << endl;
}


void Client::AddBook() {
	string message;
	string response;

	string title;
	string author;
	string genre;
	string publisher;
	string page_count;

	if (jwt_token.empty()) {
		cout << "You are not authorized. Enter library first." << endl << endl;
		return;
	}

	cout << "title="; getline(cin, title);
	cout << "author="; getline(cin, author);
	cout << "genre="; getline(cin, genre);
	cout << "publisher="; getline(cin, publisher);
	cout << "page_count="; getline(cin, page_count);

	if (!isStringNumerical(page_count)) {
		cout << "Invalid page_count." << endl << endl;
		return;
	}

	vector<string> body_data;

	body_data.push_back("\"title\": \"" + title + "\"");
	body_data.push_back("\"author\": \"" + author + "\"");
	body_data.push_back("\"genre\": \"" + genre + "\"");
	body_data.push_back("\"publisher\": \"" + publisher + "\"");
	body_data.push_back("\"page_count\": " + page_count);

	message = ComputePostRequest(conn.GetHost(), BOOKS_URL, JSON_PAYLOAD, body_data, cookies, jwt_token);

	conn.SendToServer(message);

	response = conn.ReceiveFromServer();

	if (response.find("HTTP/1.1 200 OK") != string::npos) {
		cout << "Book added successfully." << endl << endl;

	} else {
		cout << "Invalid book info." << endl << endl;
	}
}


void Client::DeleteBook() {
	string message;
	string response;

	string id;

	if (jwt_token.empty()) {
		cout << "You are not authorized. Enter library first." << endl << endl;
		return;
	}

	cout << "id="; getline(cin, id);

	if (!isStringNumerical(id)) {
		cout << "Invalid id." << endl << endl;
		return;
	}

	string url = BOOKS_URL;
	url += "/" + id;

	message = ComputeDeleteRequest(conn.GetHost(), url, cookies, jwt_token);
	
	conn.SendToServer(message);

	response = conn.ReceiveFromServer();

	if (response.find("HTTP/1.1 200 OK") != string::npos) {
		cout << "Book deleted successfully." << endl << endl;

	} else {
		cout << "No book was deleted." << endl << endl;
	}
}


void Client::Logout() {
	string message;
	string response;

	if (!logged_in) {
		cout << "You are not logged in." << endl << endl;
		return;
	}

	message = ComputeGetRequest(conn.GetHost(), LOGOUT_URL, "", cookies, "");
	
	conn.SendToServer(message);

	response = conn.ReceiveFromServer();

	if (response.find("HTTP/1.1 200 OK") == string::npos) {
		cout << "Log out failed." << endl << endl;
		return;
	}

	jwt_token.clear();
	cookies.clear();
	logged_in = false;

	cout << "Logged out successfully." << endl << endl;

}
