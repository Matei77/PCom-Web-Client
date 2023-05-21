// Copyright Ionescu Matei-Stefan - 323CAb - 2022-2023
#ifndef REQUESTS_HPP_
#define REQUESTS_HPP_

#include <vector>
#include <string>

using namespace std; 

string compute_get_request(string host, string url, string query_params, vector<string> cookies);

string compute_post_request(string host, string url, string content_type, vector<string> body_data,
							vector<string> cookies);

#endif  // REQUESTS_HPP_