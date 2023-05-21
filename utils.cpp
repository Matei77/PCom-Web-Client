#include <string>

using namespace std;

void ComputeMessage(string &message, string &line) {
    message += line;
    message += "\r\n";
	line.clear();
}

bool isStringNumerical(string str) {
    // Check if the string is empty
    if (str.empty()) {
        return false;
    }

    // Iterate through each character of the string
    for (auto c : str) {
        // Check if the character is not a digit
        if (!isdigit(c)) {
            return false;
        }
    }

    // All characters are digits
    return true;
}