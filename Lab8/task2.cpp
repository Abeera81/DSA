#include <iostream>
using namespace std;

int n;
int **board;

int moveX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int moveY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

bool isValid(int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < n && board[x][y] == -1);
}

int countAccessible(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + moveX[i];
        int ny = y + moveY[i];
        if (isValid(nx, ny))
            count++;
    }
    return count;
}

bool solveTour(int x, int y, int moveNum) {
    if (moveNum == n * n)
        return true;

    int order[8];
    int degree[8];
    int total = 0;

    for (int i = 0; i < 8; i++) {
        int nx = x + moveX[i];
        int ny = y + moveY[i];
        if (isValid(nx, ny)) {
            order[total] = i;
            degree[total] = countAccessible(nx, ny);
            total++;
        }
    }

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (degree[j] > degree[j + 1]) {
                int tempD = degree[j];
                degree[j] = degree[j + 1];
                degree[j + 1] = tempD;

                int tempO = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tempO;
            }
        }
    }

    for (int i = 0; i < total; i++) {
        int dir = order[i];
        int nx = x + moveX[dir];
        int ny = y + moveY[dir];

        board[nx][ny] = moveNum;

        if (solveTour(nx, ny, moveNum + 1))
            return true;

        board[nx][ny] = -1;
    }

    return false;
}

void printBoard() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << board[i][j] << "\t";
        }
        cout << endl;
    }
}

int main() {
    int startX, startY;
    cout << "Enter size of chessboard (N): ";
    cin >> n;
    cout << "Enter starting position (x y): ";
    cin >> startX >> startY;

    board = new int *[n];
    for (int i = 0; i < n; i++) {
        board[i] = new int[n];
        for (int j = 0; j < n; j++)
            board[i][j] = -1;
    }

    board[startX][startY] = 0;

    if (solveTour(startX, startY, 1)) {
        cout << "Knight's tour found:" << endl;
        printBoard();
    } else {
        cout << "No solution exists for the given starting position" << endl;
    }

    for (int i = 0; i < n; i++)
        delete[] board[i];
    delete[] board;

    return 0;
}
