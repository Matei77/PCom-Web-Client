// Copyright Ionescu Matei-Stefan - 323CAb - 2022-2023
#ifndef REQUESTS_HPP_
#define REQUESTS_HPP_

#include <string>
#include <vector>

using namespace std;

string ComputeGetRequest(string host, string url, string query_params, vector<string> cookies,
						 string jwt_token);

string ComputePostRequest(string host, string url, string content_type, vector<string> body_data,
						  vector<string> cookies, string jwt_token);

string ComputeDeleteRequest(string host, string url, vector<string> cookies, string jwt_token);

#endif  // REQUESTS_HPP_