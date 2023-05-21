#include <iostream>
#include <string>
#include <vector>

#include "client.hpp"
#include "utils.hpp"

using namespace std;

string ComputeGetRequest(string host, string url, string query_params, vector<string> cookies, string jwt_token) {
	string message;
	string line;

	// set method name, URL, request params (if any) and protocol type
	if (!query_params.empty()) {
		line += "GET " + url + "?" + query_params + " HTTP/1.1";
	} else {
		line += "GET " + url + " HTTP/1.1";
	}
	ComputeMessage(message, line);

	// set Host
	line += "Host: " + host;
	ComputeMessage(message, line);

	if (!jwt_token.empty()) {
		line += "Authorization: Bearer " + jwt_token;
		ComputeMessage(message, line);
	}

	// set Cookies (if any)
	if (!cookies.empty()) {
		line += "Cookie: ";
		for (auto cookie : cookies) {
			line += cookie + "; ";
		}

		// remove the last "; "
		line.resize(line.size() - 2);
		ComputeMessage(message, line);
	}

	// add final line to message
	ComputeMessage(message, line);

	return message;
}

string ComputePostRequest(string host, string url, string content_type, vector<string> body_data,
							vector<string> cookies, string jwt_token) {
	string message;
	string line;
	string data_buffer;

	// set method name, URL and protocol type
	line += "POST " + url + " HTTP/1.1";
	ComputeMessage(message, line);

	// set Host
	line += "Host: " + host;
	ComputeMessage(message, line);

	if (!jwt_token.empty()) {
		line += "Authorization: Bearer " + jwt_token;
		ComputeMessage(message, line);
	}

	// set Content-Type
	line += "Content-Type: " + content_type;
	ComputeMessage(message, line);

	// make content buffer
	data_buffer += "{";
	for (auto data : body_data) {
		data_buffer += data + ", ";
	}

	// remove the last ""
	data_buffer.resize(data_buffer.size() - 2);
	data_buffer += "}";


	// set Content-Length
	line += "Content-Length: " + to_string(data_buffer.size());
	ComputeMessage(message, line);

	// set Cookies (if any)
	if (!cookies.empty()) {
		line += "Cookie: ";
		for (auto cookie : cookies) {
			line += cookie + "; ";
		}

		// remove the last "; "
		line.resize(line.size() - 2);
		ComputeMessage(message, line);
	}

	// add final line to message
	ComputeMessage(message, line);

	// add content
	message += data_buffer;

	return message;
}

string ComputeDeleteRequest(string host, string url, vector<string> cookies, string jwt_token) {
	string message;
	string line;

	// set method name, URL and protocol type
	line += "DELETE " + url + " HTTP/1.1";
	ComputeMessage(message, line);

	// set Host
	line += "Host: " + host;
	ComputeMessage(message, line);

	if (!jwt_token.empty()) {
		line += "Authorization: Bearer " + jwt_token;
		ComputeMessage(message, line);
	}

	// set Cookies (if any)
	if (!cookies.empty()) {
		line += "Cookie: ";
		for (auto cookie : cookies) {
			line += cookie + "; ";
		}

		// remove the last "; "
		line.resize(line.size() - 2);
		ComputeMessage(message, line);
	}

	// add final line to message
	ComputeMessage(message, line);

	return message;
}