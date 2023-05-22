// Copyright Ionescu Matei-Stefan - 323CAb - 2022-2023
#ifndef REQUESTS_HPP_
#define REQUESTS_HPP_

#include <string>
#include <vector>

#include "json.hpp"

using namespace std;

struct response_info_t  {
	string status_code;
	string status_text;
	vector<string> set_cookies;
	nlohmann::json json_data;
};

string ComputeGetRequest(string host, string url, string query_params, vector<string> cookies,
						 string jwt_token);

string ComputePostRequest(string host, string url, string content_type, vector<string> body_data,
						  vector<string> cookies, string jwt_token);

string ComputeDeleteRequest(string host, string url, vector<string> cookies, string jwt_token);

void ParseServerResponse(string response, response_info_t &resp_info);


#endif  // REQUESTS_HPP_