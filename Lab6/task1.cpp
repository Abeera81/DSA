#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std;

const int LIMIT = 1000;

bool isOperator(string s) {
    return s == "+" || s == "-" || s == "*" || s == "/" || s == "^";
}

bool isUnary(string s) {
    return s == "neg" || s == "pos";
}

bool isOperand(string s) {
    if (s.empty() || isUnary(s)) return false;
    if ((s[0] >= '0' && s[0] <= '9') || s[0] == '.') {
        int dots = 0, digits = 0;
        for (char ch : s) {
            if (ch == '.') dots++;
            else if (ch >= '0' && ch <= '9') digits++;
            else return false;
        }
        return dots <= 1 && digits > 0;
    }
    if (s[0] != '_' && !(s[0] >= 'a' && s[0] <= 'z') &&
        !(s[0] >= 'A' && s[0] <= 'Z')) return false;
    for (char ch : s)
        if (string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_")
            .find(ch) == string::npos) return false;
    return true;
}

int readTokens(string expr, string tokens[], bool infix) {
    if (expr.size() > LIMIT)
        throw runtime_error("Expression is too long");
    string spaced;
    for (char ch : expr) {
        if (infix && string("()+-*/^").find(ch) != string::npos)
            spaced += " " + string(1, ch) + " ";
        else
            spaced += ch;
    }
    istringstream input(spaced);
    int count = 0;
    while (input >> tokens[count]) count++;
    if (count == 0) throw runtime_error("Expression is empty");
    return count;
}

int precedence(string op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (isUnary(op)) return 3;
    return 4;
}

string infix_to_postfix(string expr) {
    string tokens[LIMIT + 1], stack[LIMIT], result;
    int count = readTokens(expr, tokens, true), top = -1;
    bool needsOperand = true;
    for (int i = 0; i < count; i++) {
        string token = tokens[i];
        if (isOperand(token)) {
            if (!needsOperand) throw runtime_error("Missing operator");
            result += token + " ";
            needsOperand = false;
        } else if (token == "(") {
            if (!needsOperand) throw runtime_error("Missing operator");
            stack[++top] = token;
        } else if (token == ")") {
            if (needsOperand) throw runtime_error("Missing operand");
            while (top >= 0 && stack[top] != "(")
                result += stack[top--] + " ";
            if (top == -1) throw runtime_error("Unmatched parenthesis");
            top--;
        } else if (isOperator(token)) {
            if (needsOperand) {
                if (token != "+" && token != "-")
                    throw runtime_error("Missing operand");
                stack[++top] = token == "-" ? "neg" : "pos";
                continue;
            }
            while (top >= 0 && stack[top] != "(" &&
                   (precedence(stack[top]) > precedence(token) ||
                    (precedence(stack[top]) == precedence(token) && token != "^")))
                result += stack[top--] + " ";
            stack[++top] = token;
            needsOperand = true;
        } else throw runtime_error("Invalid token: " + token);
    }
    if (needsOperand) throw runtime_error("Missing operand");
    while (top >= 0) {
        if (stack[top] == "(") throw runtime_error("Unmatched parenthesis");
        result += stack[top--] + " ";
    }
    result.pop_back();
    return result;
}

string convertPostfix(string expr, bool prefix) {
    string tokens[LIMIT + 1], stack[LIMIT];
    int count = readTokens(expr, tokens, false), top = -1;
    for (int i = 0; i < count; i++) {
        string token = tokens[i];
        if (isUnary(token)) {
            if (top < 0) throw runtime_error("Missing operand");
            string value = stack[top];
            if (prefix) stack[top] = token + " " + value;
            else stack[top] = "(" + string(token == "neg" ? "-" : "+") + value + ")";
        } else if (isOperator(token)) {
            if (top < 1) throw runtime_error("Missing operand");
            string right = stack[top--], left = stack[top];
            if (prefix) stack[top] = token + " " + left + " " + right;
            else stack[top] = "(" + left + " " + token + " " + right + ")";
        } else {
            string unsignedToken = token;
            if (token.size() > 1 && (token[0] == '-' || token[0] == '+'))
                unsignedToken = token.substr(1);
            if (!isOperand(unsignedToken))
                throw runtime_error("Invalid operand: " + token);
            stack[++top] = token;
        }
    }
    if (top != 0) throw runtime_error("Too many operands");
    return stack[top];
}

string infix_to_prefix(string expr) {
    return convertPostfix(infix_to_postfix(expr), true);
}

string postfix_to_infix(string expr) {
    return convertPostfix(expr, false);
}

double calculate(double left, double right, string op) {
    double value;
    if (op == "+") value = left + right;
    else if (op == "-") value = left - right;
    else if (op == "*") value = left * right;
    else if (op == "/") {
        if (right == 0) throw runtime_error("Division by zero");
        value = left / right;
    } else value = pow(left, right);
    if (!isfinite(value)) throw runtime_error("Invalid numeric result");
    return value;
}

double evaluate(string expr, bool prefix) {
    string tokens[LIMIT + 1];
    double stack[LIMIT];
    int count = readTokens(expr, tokens, false), top = -1;
    for (int i = 0; i < count; i++) {
        string token = tokens[prefix ? count - 1 - i : i];
        if (isUnary(token)) {
            if (top < 0) throw runtime_error("Missing operand");
            if (token == "neg") stack[top] = -stack[top];
        } else if (isOperator(token)) {
            if (top < 1) throw runtime_error("Missing operand");
            double first = stack[top--], second = stack[top];
            if (prefix) stack[top] = calculate(first, second, token);
            else stack[top] = calculate(second, first, token);
        } else {
            istringstream number(token);
            double value;
            if (!(number >> value) || number.peek() != char_traits<char>::eof() ||
                !isfinite(value)) throw runtime_error("Invalid number: " + token);
            stack[++top] = value;
        }
    }
    if (top != 0) throw runtime_error("Too many operands");
    return stack[top];
}

double evaluate_postfix(string expr) {
    return evaluate(expr, false);
}

double evaluate_prefix(string expr) {
    return evaluate(expr, true);
}

int main() {
    cout << "1. Infix to postfix\n2. Infix to prefix\n3. Postfix to infix\n";
    cout << "4. Evaluate postfix\n5. Evaluate prefix\nChoice: ";
    string choice, expr;
    getline(cin, choice);
    cout << "Use spaces between prefix or postfix tokens.\n";
    cout << "Unary signs in those forms are neg and pos.\nExpression: ";
    getline(cin, expr);
    try {
        if (choice == "1") cout << infix_to_postfix(expr) << "\n";
        else if (choice == "2") cout << infix_to_prefix(expr) << "\n";
        else if (choice == "3") cout << postfix_to_infix(expr) << "\n";
        else if (choice == "4") cout << evaluate_postfix(expr) << "\n";
        else if (choice == "5") cout << evaluate_prefix(expr) << "\n";
        else throw runtime_error("Invalid choice");
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << "\n";
        return 1;
    }
}
