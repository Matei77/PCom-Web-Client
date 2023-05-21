#include "connection.hpp"

class Client {
	private:
		Connection conn;

	public:
		Client(uint16_t port, string host_addr);

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

inline Client::Client(uint16_t port, string host_addr) : conn(port, host_addr) { }