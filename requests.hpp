// Copyright Ionescu Matei-Stefan - 323CAb - 2022-2023
#ifndef REQUESTS_HPP_
#define REQUESTS_HPP_

#include <vector>
#include <string>

using namespace std; 

string compute_get_request(string host, string url, string query_params, vector<string> cookies);

string compute_post_request(string host, string url, string content_type, string body_data[],
							int body_data_fields_count, string cookies[], int cookies_count);

#endif  // REQUESTS_HPP_