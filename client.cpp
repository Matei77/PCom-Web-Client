#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "client.hpp"
#include "json.hpp"
#include "requests.hpp"
#include "utils.hpp"

using namespace std;

void Client::RunClient() {
	string command;

	do {
		// read command
		cout << "-> "; getline(cin, command);

		// open connection to server to send the request
		conn.OpenConnection();

		// check what command was given
		if (command == "register") {
			Register();
		} else if (command == "login") {
			Login();
		} else if (command == "enter_library") {
			EnterLibrary();
		} else if (command == "get_books") {
			GetBooks();
		} else if (command == "get_book") {
			GetBook();
		} else if (command == "add_book") {
			AddBook();
		} else if (command == "delete_book") {
			DeleteBook();
		} else if (command == "logout") {
			Logout();
		} else if (command == "exit") {
			break;
		} else {
			cout << "Unrecognized command." << endl;
		}

		// close the connection to the server
		conn.CloseConnection();

	} while (1);
}

void Client::Register() {
	string message;
	string response;

	string username;
	string password;

	vector<string> body_data;

	// read username and password
	cout << "username="; getline(cin, username);
	cout << "password="; getline(cin, password);

	// check if username or password contains spaces
	if (username.find(" ") != string::npos || password.find(" ") != string::npos) {
		cout << "Spaces are not allowed in username/password." << endl << endl;
		return;
	}

	// add the data to the content vector
	body_data.push_back("\"username\": \"" + username + "\"");
	body_data.push_back("\"password\": \"" + password + "\"");

	// create the post request
	message = ComputePostRequest(conn.GetHost(), REGISTER_URL, JSON_PAYLOAD, body_data,
								 vector<string>{}, "");

	// send the message to the server
	conn.SendToServer(message);

	// receive the response from the server
	response = conn.ReceiveFromServer();
	cout << response << endl;

	// check if the account was created successfully
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

	message = ComputePostRequest(conn.GetHost(), LOGIN_URL, JSON_PAYLOAD, body_data,
								 vector<string>{}, "");

	conn.SendToServer(message);
	response = conn.ReceiveFromServer();
	cout << response << endl;

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
	cout << response << endl;
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
	cout << response << endl;

	auto pos = response.find(HEADER_TERMINATOR);
	pos += HEADER_TERMINATOR_SIZE;

	string json_str = response.substr(pos);

	nlohmann::json json_data = nlohmann::json::parse(json_str);

	if (json_data.empty()) {
		cout << "No books found." << endl << endl;
		return;
	}

	for (const auto &item : json_data) {
		int id = item["id"];
		string title = item["title"];

		cout << "id=" << id << "; "
			 << "title=" << title << endl;
	}

	cout << endl;
}

void Client::GetBook() {
	string message;
	string response;

	string id;

	if (jwt_token.empty()) {
		cout << "You are not authorized. Enter library first." << endl << endl;
		return;
	}

	cout << "id=";
	getline(cin, id);

	if (!IsStringNumerical(id)) {
		cout << "Invalid id." << endl << endl;
		return;
	}

	string url = BOOKS_URL;
	url += "/" + id;

	message = ComputeGetRequest(conn.GetHost(), url, "", cookies, jwt_token);

	conn.SendToServer(message);

	response = conn.ReceiveFromServer();
	cout << response << endl;

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

	cout << "title=";      getline(cin, title);
	cout << "author=";     getline(cin, author);
	cout << "genre=";      getline(cin, genre);
	cout << "publisher=";  getline(cin, publisher);
	cout << "page_count="; getline(cin, page_count);

	// check read values
	if (EmptyOrContainsOnlySpaces(title)) {
		cout << "Invalid title." << endl << endl;
		return;
	}
	
	if (EmptyOrContainsOnlySpaces(author)) {
		cout << "Invalid author." << endl << endl;
		return;
	}

	if (EmptyOrContainsOnlySpaces(genre)) {
		cout << "Invalid genre." << endl << endl;
		return;
	} 
	
	if (EmptyOrContainsOnlySpaces(publisher)) {
		cout << "Invalid publisher." << endl << endl;
		return;
	}

	if (!IsStringNumerical(page_count)) {
		cout << "Invalid page_count." << endl << endl;
		return;
	}

	vector<string> body_data;

	body_data.push_back("\"title\": \"" + title + "\"");
	body_data.push_back("\"author\": \"" + author + "\"");
	body_data.push_back("\"genre\": \"" + genre + "\"");
	body_data.push_back("\"publisher\": \"" + publisher + "\"");
	body_data.push_back("\"page_count\": " + page_count);

	message =
		ComputePostRequest(conn.GetHost(), BOOKS_URL, JSON_PAYLOAD, body_data, cookies, jwt_token);

	conn.SendToServer(message);

	response = conn.ReceiveFromServer();
	cout << response << endl;

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

	cout << "id=";
	getline(cin, id);

	if (!IsStringNumerical(id)) {
		cout << "Invalid id." << endl << endl;
		return;
	}

	string url = BOOKS_URL;
	url += "/" + id;

	message = ComputeDeleteRequest(conn.GetHost(), url, cookies, jwt_token);

	conn.SendToServer(message);

	response = conn.ReceiveFromServer();
	cout << response << endl;

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
	cout << response << endl;

	if (response.find("HTTP/1.1 200 OK") == string::npos) {
		cout << "Log out failed." << endl << endl;
		return;
	}

	jwt_token.clear();
	cookies.clear();
	logged_in = false;

	cout << "Logged out successfully." << endl << endl;
}
