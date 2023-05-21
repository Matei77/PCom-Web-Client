// Copyright Ionescu Matei-Stefan - 323CAb - 2022-2023
#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "connection.hpp"

#define REGISTER_URL "/api/v1/tema/auth/register"
#define LOGIN_URL "/api/v1/tema/auth/login"
#define ACCESS_URL "/api/v1/tema/auth/access"
#define BOOKS_URL "/api/v1/tema/auth/books"
#define LOGOUT_URL "/api/v1/tema/auth/logout"

#define JSON_PAYLOAD "application/json"

class Client {
	private:
		Connection conn;

	public:
		Client(uint16_t port, string host);

		void RunClient();

	private:
		void Register();
		void Login();
		void EnterLibrary();
		void GetBooks();
		void GetBook();
		void AddBook();
		void DeleteBook();
		void Logout();
		void Exit();
};

inline Client::Client(uint16_t port, string host) : conn(port, host) { }

#endif  // CLIENT_HPP_