#include <iostream>
using namespace std;

struct Person {
    int id;
    Person* next;

    Person(int i) {
        id = i;
        next = nullptr;
    }
};

class JosephusCircle {
private:
    Person* tail;   
    int size;
    int* eliminated;
    int eliminatedCount;

public:
    JosephusCircle() {
        tail = nullptr;
        size = 0;
        eliminated = nullptr;
        eliminatedCount = 0;
    }

    ~JosephusCircle() {
        if (tail != nullptr) {
            Person* temp = tail->next;
            tail->next = nullptr;   
            while (temp != nullptr) {
                Person* nextPerson = temp->next;
                delete temp;
                temp = nextPerson;
            }
        }
        delete[] eliminated;
    }

    void createCircle(int n) {
        for (int i = 1; i <= n; i++) {
            Person* newPerson = new Person(i);
            if (tail == nullptr) {
                tail = newPerson;
                tail->next = tail;
            } else {
                newPerson->next = tail->next;
                tail->next = newPerson;
                tail = newPerson;
            }
        }
        size = n;
        eliminated = new int[n];
    }

    void displayCircle() {
        if (tail == nullptr) {
            cout << "Circle is empty.\n";
            return;
        }
        cout << "Circle: ";
        Person* temp = tail->next;
        do {
            cout << temp->id << " ";
            temp = temp->next;
        } while (temp != tail->next);
        cout << "\n";
    }

    void eliminate(int k) {
        // prev stays one step behind
        Person* prev = tail;

        while (size > 1) {
            for (int i = 1; i < k; i++) {
                prev = prev->next;
            }

            Person* victim = prev->next;
            prev->next = victim->next;
            cout << "Person " << victim->id << " is eliminated.\n";
            eliminated[eliminatedCount] = victim->id;
            eliminatedCount++;
            delete victim;
            size--;
        }

        tail = prev;
    }

    void displayEliminatedOrder() {
        cout << "Elimination order: ";
        for (int i = 0; i < eliminatedCount; i++) {
            cout << eliminated[i] << " ";
        }
        cout << "\n";
    }

    void displaySurvivor() {
        if (tail != nullptr) {
            cout << "Survivor: Person " << tail->id << "\n";
        }
    }
};

int readPositiveInt() {
    int value;
    while (!(cin >> value) || value < 1) {
        if (cin.eof()) {
            return 0;
        }
        cout << "Invalid input. Enter a number of at least 1: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return value;
}

int main() {
    cout << "Enter number of people (N): ";
    int n = readPositiveInt();
    cout << "Enter step count (k): ";
    int k = readPositiveInt();

    if (n == 0 || k == 0) {
        return 0;
    }

    JosephusCircle circle;
    circle.createCircle(n);
    circle.displayCircle();

    cout << "\n";
    circle.eliminate(k);

    cout << "\n";
    circle.displayEliminatedOrder();
    circle.displaySurvivor();

    return 0;
}
