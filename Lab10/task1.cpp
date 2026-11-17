#include <iostream>
using namespace std;

const int MAX_TRUCKS = 20;
const int MAX_CONTAINERS = 50;

struct Container {
    int id;
    int weight;
    Container* next;
};

Container* head = NULL;
Container* tail = NULL;

void addContainer(int id, int weight) {
    Container* newNode = new Container;
    newNode->id = id;
    newNode->weight = weight;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

void clearList() {
    while (head != NULL) {
        Container* temp = head;
        head = head->next;
        delete temp;
    }
    tail = NULL;
}

struct Truck {
    int id;
    int remaining;
    int loaded[MAX_CONTAINERS];
    int count;
};

class TruckQueue {
    Truck data[MAX_TRUCKS];
    int front;
    int rear;

public:
    TruckQueue() {
        front = 0;
        rear = 0;
    }

    void enqueue(Truck t) {
        data[rear] = t;
        rear++;
    }

    Truck dequeue() {
        Truck t = data[front];
        front++;
        return t;
    }

    bool isEmpty() {
        return front == rear;
    }
};

Truck dock[MAX_TRUCKS];
int numTrucks = 0;

bool loadContainers(Container* current, int weightLeft, int capacityLeft) {
    if (current == NULL) {
        return true;
    }

    if (weightLeft > capacityLeft) {
        return false;
    }

    for (int i = 0; i < numTrucks; i++) {

        if (dock[i].remaining < current->weight) {
            continue;
        }

        dock[i].remaining = dock[i].remaining - current->weight;
        dock[i].loaded[dock[i].count] = current->id;
        dock[i].count++;

        if (loadContainers(current->next, weightLeft - current->weight,
                           capacityLeft - current->weight)) {
            return true;
        }

        dock[i].remaining = dock[i].remaining + current->weight;
        dock[i].count--;
    }

    return false;
}

void runTest(int ids[], int weights[], int n, int trucks, int W) {
    for (int i = 0; i < n; i++) {
        addContainer(ids[i], weights[i]);
    }

    TruckQueue q;
    for (int i = 0; i < trucks; i++) {
        Truck t;
        t.id = i + 1;
        t.remaining = W;
        t.count = 0;
        q.enqueue(t);
    }

    numTrucks = 0;
    while (!q.isEmpty()) {
        dock[numTrucks] = q.dequeue();
        numTrucks++;
    }

    cout << "Containers: ";
    int totalWeight = 0;
    Container* temp = head;
    while (temp != NULL) {
        cout << "C" << temp->id << "(" << temp->weight << "t) ";
        totalWeight = totalWeight + temp->weight;
        temp = temp->next;
    }
    cout << endl;
    cout << "Trucks: " << trucks << " trucks, capacity " << W << " tons each" << endl;

    if (loadContainers(head, totalWeight, trucks * W)) {
        for (int i = 0; i < numTrucks; i++) {
            cout << "  Truck " << dock[i].id << ": ";
            if (dock[i].count == 0) {
                cout << "(empty)";
            }
            for (int j = 0; j < dock[i].count; j++) {
                cout << "C" << dock[i].loaded[j] << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Insufficient truck capacity." << endl;
    }

    clearList();
    cout << endl;
}

int main() {
    cout << " Test 1 " << endl;
    int ids1[] = {1, 2, 3, 4, 5, 6};
    int weights1[] = {4, 8, 1, 4, 2, 1};
    runTest(ids1, weights1, 6, 2, 10);

    cout << " Test 2 " << endl;
    int ids2[] = {1, 2, 3, 4};
    int weights2[] = {4, 4, 4, 4};
    runTest(ids2, weights2, 4, 3, 5);

    cout << " Test 3 " << endl;
    int ids3[] = {1, 2, 3};
    int weights3[] = {5, 5, 5};
    runTest(ids3, weights3, 3, 2, 8);

    return 0;
}
