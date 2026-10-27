#include <iostream>
using namespace std;

int getMaxRowInColumn(int **arr, int rows, int col) {
    int maxRow = 0;
    for (int i = 1; i < rows; i++) {
        if (arr[i][col] > arr[maxRow][col])
            maxRow = i;
    }
    return maxRow;
}

void findPeak(int **arr, int rows, int cols) {
    int low = 0, high = cols - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int rowIndex = getMaxRowInColumn(arr, rows, mid);
        int current = arr[rowIndex][mid];

        int left = (mid - 1 >= 0) ? arr[rowIndex][mid - 1] : -1;
        int right = (mid + 1 < cols) ? arr[rowIndex][mid + 1] : -1;

        if (current >= left && current >= right) {
            cout << "Peak element found: " << current << endl;
            cout << "Position -> Row: " << rowIndex << " Col: " << mid << endl;
            return;
        } else if (left > current) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    cout << "No peak found" << endl;
}

int main() {
    int rows, cols;
    cout << "Enter number of rows and columns: ";
    cin >> rows >> cols;

    int **arr = new int *[rows];
    for (int i = 0; i < rows; i++)
        arr[i] = new int[cols];

    cout << "Enter the elements of the array:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> arr[i][j];
        }
    }

    findPeak(arr, rows, cols);

    for (int i = 0; i < rows; i++)
        delete[] arr[i];
    delete[] arr;

    return 0;
}
