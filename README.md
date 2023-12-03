**Name: Ionescu Matei-Ștefan**  
**Group: 323CAb**

# PCom Homework #4 - Web-Client using REST API

This application sends HTTP requests to a server based on the user's commands.
The user has the ability to register, login, logout, and interact with the
books stored on the server. The available commands are: `register`, `login`,
`enter_library`, `get_books`, `get_book`, `add_book`, `delete_book`, `logout`
and `exit`.

## Client
The `Client` class contains informations regarding the connection to the
server, the cookies, the logged it status and the JWT Token.

When the program starts the `RunClient()` method is called and begins reading
commands from the user. The class implements a method for each available
command and the required method will be used when a command is given. Each
command checks if the prerequisites for sending the message to the server are
met, and if a message is sent, receives the server's response, informing the
user if the action was successful.

## Connection
The `Connection` class stores informations about the connection to the server.
The socket file descriptor, the port and the address of the server.

This class also implements methods for opening and closing the connection to
the server and sending and receving data to/from the server.

## Requests
The requests.cpp file contains functions used to generate a POST, GET or DELETE
request. It also contains a fuction that allows parsing the server response and
filling in the necessary information in a variable of the `response_info_t`
type.

## Mentions
1. The json parser used can be found at https://github.com/nlohmann/json. This
was used in order to parse the json data received from the server's response.
2. The application passes all the checker's tests.
3. The implementation is using a C++ adaptation of the HTTP lab.


## References
1. https://gitlab.cs.pub.ro/pcom/pcom-laboratoare-public/-/tree/master/lab9
2. https://pcom.pages.upb.ro/labs/lab9/http.html
