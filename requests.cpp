#include <iostream>
#include <string>
#include <vector>

#include "client.hpp"
#include "utils.hpp"

using namespace std;

string compute_get_request(string host, string url, string query_params, vector<string> cookies) {
	string message;
	string line;

	// set method name, URL, request params (if any) and protocol type
	if (!query_params.empty()) {
		line += "GET " + url + "?" + query_params + " HTTP/1.1";
	} else {
		line += "GET " + url + " HTTP/1.1";
	}
	compute_message(message, line);

	// set Host
	line += "Host: " + host;
	compute_message(message, line);

	// set Cookies (if any)
	if (!cookies.empty()) {
		line += "Cookie: ";
		for (auto cookie : cookies) {
			line += cookie + "; ";
		}

		// remove the last "; "
		line.resize(line.size() - 2);
		compute_message(message, line);
	}

	// add final line to message
	compute_message(message, line);

	return message;
}

string compute_post_request(string host, string url, string content_type, vector<string> body_data,
							vector<string> cookies) {
	string message;
	string line;
	string data_buffer;

	// set method name, URL and protocol type
	line += "POST " + url + " HTTP/1.1";
	compute_message(message, line);

	// set Host
	line += "Host: " + host;
	compute_message(message, line);

	// set Content-Type
	line += "Content-Type: " + content_type;
	compute_message(message, line);

	// make content buffer
	for (auto data : body_data) {
		data_buffer += data + "&";
	}

	// remove the last "&"
	data_buffer.resize(data_buffer.size() - 1);

	// set Content-Length
	line += "Content-Length: " + to_string(data_buffer.size());
	compute_message(message, line);

	// set Cookies (if any)
	if (!cookies.empty()) {
		line += "Cookie: ";
		for (auto cookie : cookies) {
			line += cookie + "; ";
		}

		// remove the last "; "
		line.resize(line.size() - 2);
		compute_message(message, line);
	}

	// add final line to message
	compute_message(message, line);

	// add content
	message += data_buffer;

	return message;
}