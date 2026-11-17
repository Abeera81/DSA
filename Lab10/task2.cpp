#include <iostream>
using namespace std;

const int MAX = 50;

char opSymbol[] = {'+', '*', '-', '/'};
int opValue[]   = { 3,   2,   5,   2 };
int numOps = 4;

int applyOp(int i, int number) {
    if (opSymbol[i] == '+') return number + opValue[i];
    if (opSymbol[i] == '-') return number - opValue[i];
    if (opSymbol[i] == '*') return number * opValue[i];
    if (opSymbol[i] == '/') {
        if (number % opValue[i] != 0) return -1;
        return number / opValue[i];
    }
    return -1;
}

class Stack {
    int data[MAX];
    int top;

public:
    Stack() {
        top = -1;
    }

    void push(int x) {
        top++;
        data[top] = x;
    }

    void pop() {
        top--;
    }

    bool isEmpty() {
        return top == -1;
    }

    int size() {
        return top + 1;
    }

    void print(int start) {
        int value = start;
        cout << value;
        for (int i = 0; i <= top; i++) {
            int op = data[i];
            value = applyOp(op, value);
            cout << " --(" << opSymbol[op] << opValue[op] << ")--> " << value;
        }
        cout << "   [" << size() << " ops]" << endl;
    }
};

int S_start, S_target, K, M_max;
Stack opStack;
int solutions = 0;

void findSequences(int current, int depth) {
    if (current == S_target && !opStack.isEmpty()) {
        solutions++;
        cout << solutions << ") ";
        opStack.print(S_start);
    }

    if (depth == K) {
        return;
    }

    for (int i = 0; i < numOps; i++) {
        int next = applyOp(i, current);

        if (next < 0 || next > M_max) {
            continue;
        }

        opStack.push(i);
        findSequences(next, depth + 1);
        opStack.pop();
    }
}

void runTest(int start, int target, int k, int mmax) {
    S_start = start;
    S_target = target;
    K = k;
    M_max = mmax;
    solutions = 0;

    cout << " Start = " << S_start << ", Target = " << S_target
         << ", K = " << K << ", Mmax = " << M_max << " " << endl;

    if (S_start == S_target) {
        cout << "Already at target (0 operations needed)" << endl;
    }

    findSequences(S_start, 0);

    if (solutions == 0 && S_start != S_target) {
        cout << "No valid sequence found." << endl;
    }
    cout << "Total sequences: " << solutions << endl << endl;
}

int main() {
    cout << "Allowed operations: +3  *2  -5  /2" << endl << endl;

    runTest(2, 10, 3, 50);
    runTest(5, 1, 4, 20);
    runTest(1, 100, 3, 1000);

    return 0;
}
