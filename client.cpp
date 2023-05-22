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
			cout << "[ERROR] Unrecognized command." << endl;
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
		cout << "[ERROR] Spaces are not allowed in username/password." << endl << endl;
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

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// check if the account was created successfully
	if (resp_info.status_code == "201") {
		cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Account was created successfully." << endl << endl;
	} else {
		string error = resp_info.json_data["error"];
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << error << endl << endl;
	}
}

void Client::Login() {
	string message;
	string response;

	string username;
	string password;

	vector<string> body_data;

	// check if the user is already logged in
	if (logged_in) {
		cout << "[ERROR] You are already logged in." << endl << endl;
		return;
	}

	// read username and password
	cout << "username="; getline(cin, username);
	cout << "password="; getline(cin, password);

	// check if username or password contains spaces
	if (username.find(" ") != string::npos || password.find(" ") != string::npos) {
		cout << "[ERROR] Spaces are not allowed in username/password." << endl << endl;
		return;
	}

	// add the data to the content vector
	body_data.push_back("\"username\": \"" + username + "\"");
	body_data.push_back("\"password\": \"" + password + "\"");

	// create the post request
	message = ComputePostRequest(conn.GetHost(), LOGIN_URL, JSON_PAYLOAD, body_data,
								 vector<string>{}, "");

	// send the message to the server
	conn.SendToServer(message);

	// receive the response from the server
	response = conn.ReceiveFromServer();

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// check if the login was successful
	if (resp_info.status_code == "200") {
		cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Logged in successfully." << endl << endl;

	} else {
		string error = resp_info.json_data["error"];
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << error << endl << endl;
		return;
	}

	// update cookes and login status
	cookies = resp_info.set_cookies;
	logged_in = true;
}

void Client::EnterLibrary() {
	string message;
	string response;

	// check if the user is not logged in
	if (!logged_in) {
		cout << "[ERROR] You are not logged in." << endl << endl;
		return;
	}

	// create the get request
	message = ComputeGetRequest(conn.GetHost(), ACCESS_URL, "", cookies, "");

	// send the message to the server
	conn.SendToServer(message);

	// receive the response from the server
	response = conn.ReceiveFromServer();

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// check if the login was successful
	if (resp_info.status_code == "200") {
		cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Successfully entered library." << endl << endl;

	} else {
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Could not enter library." << endl << endl;
		return;
	}

	// update jwt_token
	jwt_token = resp_info.json_data["token"];
}

void Client::GetBooks() {
	string message;
	string response;

	// check if the user is authorized
	if (jwt_token.empty()) {
		cout << "[ERROR] You are not authorized. Enter library first." << endl << endl;
		return;
	}

	// create the get request
	message = ComputeGetRequest(conn.GetHost(), BOOKS_URL, "", cookies, jwt_token);

	// send the message to the server
	conn.SendToServer(message);

	// receive the response from the server
	response = conn.ReceiveFromServer();

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// check there was an error
	if (resp_info.status_code != "200") {
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Could not get books." << endl << endl;
		return;
	}

	cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";

	if (resp_info.json_data.empty()) {
		cout << "No books found." << endl << endl;
		return;
	}
	
	cout << "Found the following books:" << endl;

	for (const auto item : resp_info.json_data) {
		// not printing the value directly so the strings will not have " characters around them
		// (did this for the checker)
		int id = item["id"];
		string title = item["title"];

		cout << "id=" << id << " --> "
			 << "title=" << title << endl;
	}

	cout << endl;
}

void Client::GetBook() {
	string message;
	string response;

	string id;

	// check if the user is authorized
	if (jwt_token.empty()) {
		cout << "[ERROR] You are not authorized. Enter library first." << endl << endl;
		return;
	}

	// read the book id
	cout << "id="; getline(cin, id);

	// check the the id is a number
	if (!IsStringNumerical(id)) {
		cout << "[ERROR] Invalid id." << endl << endl;
		return;
	}

	// create url
	string url = BOOKS_URL;
	url += "/" + id;

	// create the get request
	message = ComputeGetRequest(conn.GetHost(), url, "", cookies, jwt_token);

	// send the message to the server
	conn.SendToServer(message);

	// receive the response from the server
	response = conn.ReceiveFromServer();

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// check for errors
	if (resp_info.status_code != "200") {
		string error = resp_info.json_data["error"];
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << error << endl << endl;
		return;
	}

	// show the book info
	// not printing the value directly so the strings will not have " characters around them
	// (did this for the checker)
	string title = resp_info.json_data["title"];
	string author = resp_info.json_data["author"];
	string genre = resp_info.json_data["genre"];
	string publisher = resp_info.json_data["publisher"];
	int page_count = resp_info.json_data["page_count"];

	cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";
	cout << "Here are the book's informations:" << endl;

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

	// check if the user is authorized
	if (jwt_token.empty()) {
		cout << "[ERROR] You are not authorized. Enter library first." << endl << endl;
		return;
	}

	// read book's info
	cout << "title=";      getline(cin, title);
	cout << "author=";     getline(cin, author);
	cout << "genre=";      getline(cin, genre);
	cout << "publisher=";  getline(cin, publisher);
	cout << "page_count="; getline(cin, page_count);

	// check read values
	if (EmptyOrContainsOnlySpaces(title)) {
		cout << "[ERROR] Invalid title." << endl << endl;
		return;
	}
	
	if (EmptyOrContainsOnlySpaces(author)) {
		cout << "[ERROR] Invalid author." << endl << endl;
		return;
	}

	if (EmptyOrContainsOnlySpaces(genre)) {
		cout << "[ERROR] Invalid genre." << endl << endl;
		return;
	} 
	
	if (EmptyOrContainsOnlySpaces(publisher)) {
		cout << "[ERROR] Invalid publisher." << endl << endl;
		return;
	}

	if (!IsStringNumerical(page_count)) {
		cout << "[ERROR] Invalid page_count." << endl << endl;
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

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// show message to user
	if (resp_info.status_code == "200") {
		cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Book added successfully." << endl << endl;
		return;
	} else {
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Invalid book info." << endl << endl;
	}
}

void Client::DeleteBook() {
	string message;
	string response;

	string id;

	// check if the user is authorized
	if (jwt_token.empty()) {
		cout << "[ERROR] You are not authorized. Enter library first." << endl << endl;
		return;
	}

	// read the book id
	cout << "id="; getline(cin, id);

	// check the the id is a number
	if (!IsStringNumerical(id)) {
		cout << "[ERROR] Invalid id." << endl << endl;
		return;
	}

	// create url
	string url = BOOKS_URL;
	url += "/" + id;

	// create delete message
	message = ComputeDeleteRequest(conn.GetHost(), url, cookies, jwt_token);

	// send the message to the server
	conn.SendToServer(message);

	// receive the response from the server
	response = conn.ReceiveFromServer();

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// give response to user
	if (resp_info.status_code == "200") {
		cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Book deleted successfully." << endl << endl;
		return;
	} else {
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "No book has the given id." << endl << endl;
	}
}

void Client::Logout() {
	string message;
	string response;

	// check if the user is not logged in
	if (!logged_in) {
		cout << "[ERROR] You are not logged in." << endl << endl;
		return;
	}

	// create the get request
	message = ComputeGetRequest(conn.GetHost(), LOGOUT_URL, "", cookies, "");

	// send the message to the server
	conn.SendToServer(message);

	// receive the response from the server
	response = conn.ReceiveFromServer();

	// parse the response
	response_info_t resp_info;
	ParseServerResponse(response, resp_info);

	// give response to user
	if (resp_info.status_code == "200") {
		cout << "[SUCCESS - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Logged out successfully." << endl << endl;

	} else {
		cout << "[ERROR - " << resp_info.status_code << " " << resp_info.status_text << "] ";
		cout << "Log out failed." << endl << endl;
		return;
	}

	// update values
	jwt_token.clear();
	cookies.clear();
	logged_in = false;
}
