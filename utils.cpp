#include <string>

using namespace std;

void compute_message(string &message, string &line)
{
    message += line;
    message += "\r\n";
	line.clear();
}