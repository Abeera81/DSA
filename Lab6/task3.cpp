#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

struct PrintRequest {
    int id, arrivalTime, processingTime;
};

int capacity = 2, frontIndex = 0, count = 0;
PrintRequest* items = new PrintRequest[capacity];

bool isEmpty() { return count == 0; }
bool isFull() { return count == capacity; }

void show() {
    cout << "Queue: ";
    if (isEmpty()) cout << "empty ";
    for (int i = 0; i < count; i++)
        cout << items[(frontIndex + i) % capacity].id << " ";
    cout << "| Capacity: " << capacity << "\n";
}

void resize(int newCapacity) {
    if (newCapacity < 2 || newCapacity < count)
        throw runtime_error("Capacity is too small");
    PrintRequest* bigger = new PrintRequest[newCapacity];
    for (int i = 0; i < count; i++)
        bigger[i] = items[(frontIndex + i) % capacity];
    delete[] items;
    items = bigger;
    capacity = newCapacity;
    frontIndex = 0;
}

void enqueue(PrintRequest r) {
    if (r.arrivalTime < 0 || r.processingTime <= 0)
        throw runtime_error("Invalid request times");
    if (isFull()) resize(capacity * 2);
    items[(frontIndex + count) % capacity] = r;
    count++;
    cout << "Added " << r.id << " | ";
    show();
}

PrintRequest front() {
    if (isEmpty()) throw runtime_error("Queue is empty");
    return items[frontIndex];
}

PrintRequest dequeue() {
    PrintRequest r = front();
    frontIndex = (frontIndex + 1) % capacity;
    count--;
    if (capacity > 2 && count <= capacity / 4) resize(capacity / 2);
    cout << "Removed " << r.id << " | ";
    show();
    return r;
}

int main() {
    int exitCode = 0;
    try {
        PrintRequest requests[100];
        int total;
        cout << "Number of requests (1 to 100): ";
        if (!(cin >> total) || total < 1 || total > 100)
            throw runtime_error("Enter a number from 1 to 100");
        cout << "Enter requests in arrival order. IDs are assigned automatically.\n";
        for (int i = 0; i < total; i++) {
            requests[i].id = i + 1;
            cout << "Request " << i + 1 << " arrival and duration: ";
            if (!(cin >> requests[i].arrivalTime >> requests[i].processingTime) ||
                requests[i].arrivalTime < 0 || requests[i].processingTime <= 0)
                throw runtime_error("Arrival must be nonnegative and duration positive");
            if (i > 0 && requests[i].arrivalTime < requests[i - 1].arrivalTime)
                throw runtime_error("Requests must be in arrival order");
        }
        int next = 0;
        long long time = 0, totalWait = 0;
        while (next < total || !isEmpty()) {
            if (isEmpty() && next < total && time < requests[next].arrivalTime)
                time = requests[next].arrivalTime;
            while (next < total && requests[next].arrivalTime <= time)
                enqueue(requests[next++]);
            PrintRequest r = dequeue();
            totalWait += time - r.arrivalTime;
            time += r.processingTime;
            while (next < total && requests[next].arrivalTime < time)
                enqueue(requests[next++]);
        }
        cout << fixed << setprecision(2);
        cout << "Average waiting time: " << double(totalWait) / total << "\n";
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << "\n";
        exitCode = 1;
    }
    delete[] items;
    return exitCode;
}
