#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <stdexcept>
using namespace std;

const int LIMIT = 1000;

bool isValidBrackets(string s) {
    if (s.size() > LIMIT) throw runtime_error("Input is too long");
    char stack[LIMIT];
    int top = -1;
    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{')
            stack[++top] = ch;
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (top == -1) return false;
            if ((ch == ')' && stack[top] != '(') ||
                (ch == ']' && stack[top] != '[') ||
                (ch == '}' && stack[top] != '{')) return false;
            top--;
        }
    }
    return top == -1;
}

string trim(string text) {
    int first = text.find_first_not_of(" \t\r\n");
    if (first == -1) return "";
    int last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

bool validName(string name) {
    if (name.empty()) return false;
    string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    if (letters.find(name[0]) == string::npos) return false;
    string allowed = letters + "0123456789:.-";
    for (char ch : name)
        if (allowed.find(ch) == string::npos) return false;
    return true;
}

bool validAttributes(string text) {
    istringstream input(text);
    while (input >> ws && input.peek() != char_traits<char>::eof()) {
        string name;
        char ch;
        while (input.get(ch) && ch != '=' &&
               string(" \t\r\n").find(ch) == string::npos)
            name += ch;
        if (!input || !validName(name)) return false;
        if (ch != '=' && (!(input >> ch) || ch != '=')) return false;
        if (!(input >> ch) || (ch != '"' && ch != '\'')) return false;
        string value;
        if (!getline(input, value, ch) || input.eof() ||
            value.find('<') != string::npos) return false;
        int next = input.peek();
        if (next != char_traits<char>::eof() &&
            string(" \t\r\n").find(char(next)) == string::npos) return false;
    }
    return true;
}

pair<bool, vector<string>> validate_markup(string s) {
    if (s.size() > LIMIT) throw runtime_error("Input is too long");
    string stack[LIMIT];
    int top = -1;
    vector<string> errors;
    for (int i = 0; i < int(s.size()); i++) {
        if (s[i] != '<') continue;
        if (s.substr(i, 4) == "<!--" || s.substr(i, 2) == "<?") {
            bool comment = s.substr(i, 4) == "<!--";
            string ending = comment ? "-->" : "?>";
            size_t end = s.find(ending, i + (comment ? 4 : 2));
            if (end == string::npos) {
                errors.push_back("Unfinished comment or instruction");
                break;
            }
            i = end + ending.size() - 1;
            continue;
        }
        int end = i + 1;
        char quote = 0;
        for (; end < int(s.size()); end++) {
            char ch = s[end];
            if (quote) { if (ch == quote) quote = 0; }
            else if (ch == '"' || ch == '\'') quote = ch;
            else if (ch == '>') break;
        }
        if (end == int(s.size())) {
            errors.push_back("Unfinished tag");
            break;
        }
        string tag = s.substr(i + 1, end - i - 1);
        i = end;
        bool closing = !tag.empty() && tag[0] == '/';
        if (closing) tag.erase(0, 1);
        tag = trim(tag);
        bool selfClosing = !tag.empty() && tag.back() == '/';
        if (selfClosing) tag = trim(tag.substr(0, tag.size() - 1));
        int nameEnd = tag.find_first_of(" \t\r\n");
        if (nameEnd == -1) nameEnd = tag.size();
        string name = tag.substr(0, nameEnd);
        if (!validName(name)) errors.push_back("Invalid tag name: " + name);
        else if (closing) {
            if (selfClosing || nameEnd != int(tag.size()))
                errors.push_back("Invalid closing tag: " + name);
            else if (top == -1) errors.push_back("Unexpected closing tag: " + name);
            else if (stack[top--] != name)
                errors.push_back("Wrong closing tag: " + name);
        } else if (!validAttributes(tag.substr(nameEnd)))
            errors.push_back("Invalid attributes: " + name);
        else if (!selfClosing) stack[++top] = name;
    }
    while (top >= 0) errors.push_back("Unclosed tag: " + stack[top--]);
    return {errors.empty(), errors};
}

int main() {
    cout << "1. Check brackets\n2. Check markup\nChoice: ";
    string choice, text;
    getline(cin, choice);
    cout << "Text on one line: ";
    getline(cin, text);
    try {
        if (choice == "1")
            cout << (isValidBrackets(text) ? "Valid\n" : "Invalid\n");
        else if (choice == "2") {
            auto result = validate_markup(text);
            cout << (result.first ? "Valid\n" : "Invalid\n");
            for (string error : result.second) cout << "Error: " << error << "\n";
        } else throw runtime_error("Invalid choice");
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << "\n";
        return 1;
    }
}
