#include <string>

using namespace std;

void ComputeMessage(string &message, string &line) {
    message += line;
    message += "\r\n";
	line.clear();
}

bool IsStringNumerical(string str) {
    // check if the string is empty
    if (str.empty()) {
        return false;
    }

    // iterate through each character of the string
    for (auto c : str) {
        // check if the character is not a digit
        if (!isdigit(c)) {
            return false;
        }
    }

    // all characters are digits
    return true;
}

bool EmptyOrContainsOnlySpaces(string str) {
    if (str.empty()) {
        return true;
    }

    for (char c : str) {
        if (c != ' ') {
            return false;
        }
    }
    return true;
}
