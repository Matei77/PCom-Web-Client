#include <string>
#include <vector>
#include <iostream>

#include "client.hpp"
#include "utils.hpp"

using namespace std;

string compute_get_request(string host, string url, string query_params, vector<string> cookies) {
	string message;
	string line;

	if (!query_params.empty()) {
		line += "GET " + url + "?" + query_params + " HTTP/1.1";
	} else {
		line += "GET " + url + " HTTP/1.1";
	}
	compute_message(message, line);

	line += "Host: " + host;
	compute_message(message, line);

	if (!cookies.empty()) {
		line += "Cookie: ";
		for (auto cookie : cookies) {
			line += cookie + "; ";
		}
		line.resize(line.size() - 2);

		compute_message(message, line);
	}

	compute_message(message, line);

	return message;
}

// string compute_post_request(string host, string url, string content_type, string body_data[],
// 							int body_data_fields_count, string cookies[], int cookies_count) {
// 	string message;
							
// 	return message;				
// }