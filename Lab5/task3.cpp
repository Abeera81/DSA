#include <iostream>
#include <string>
using namespace std;

struct Coach {
    int number;
    string type;
    int capacity;
    int passengers;
    Coach* prev;
    Coach* next;

    Coach(int n, string t, int c, int p) {
        number = n;
        type = t;
        capacity = c;
        passengers = p;
        prev = nullptr;
        next = nullptr;
    }
};

class Train {
private:
    Coach* head;
    Coach* current;

    void printCoach(Coach* c) {
        cout << "Coach " << c->number << " (" << c->type << "), "
             << c->passengers << "/" << c->capacity << " passengers";
    }

    Coach* findCoach(int number) {
        if (head == nullptr) {
            return nullptr;
        }
        Coach* temp = head;
        do {
            if (temp->number == number) {
                return temp;
            }
            temp = temp->next;
        } while (temp != head);
        return nullptr;
    }

public:
    Train() {
        head = nullptr;
        current = nullptr;
    }

    ~Train() {
        if (head == nullptr) {
            return;
        }
        // break the circle, then delete like a normal list
        head->prev->next = nullptr;
        Coach* temp = head;
        while (temp != nullptr) {
            Coach* nextCoach = temp->next;
            delete temp;
            temp = nextCoach;
        }
    }

    void addCoach(int number, string type, int capacity, int passengers) {
        if (findCoach(number) != nullptr) {
            cout << "Coach " << number << " already exists.\n";
            return;
        }

        Coach* newCoach = new Coach(number, type, capacity, passengers);

        if (head == nullptr) {
            // a single coach points to itself
            newCoach->next = newCoach;
            newCoach->prev = newCoach;
            head = newCoach;
            current = newCoach;
        } else {
            // the last coach is head->prev
            Coach* tail = head->prev;
            tail->next = newCoach;
            newCoach->prev = tail;
            newCoach->next = head;
            head->prev = newCoach;
        }
        cout << "Coach added.\n";
    }

    void insertAfter(int afterNumber, int number, string type, int capacity, int passengers) {
        Coach* target = findCoach(afterNumber);
        if (target == nullptr) {
            cout << "Coach " << afterNumber << " not found.\n";
            return;
        }
        if (findCoach(number) != nullptr) {
            cout << "Coach " << number << " already exists.\n";
            return;
        }

        Coach* newCoach = new Coach(number, type, capacity, passengers);
        newCoach->next = target->next;
        newCoach->prev = target;
        target->next->prev = newCoach;
        target->next = newCoach;
        cout << "Coach inserted after coach " << afterNumber << ".\n";
    }

    void removeCoach(int number) {
        Coach* target = findCoach(number);
        if (target == nullptr) {
            cout << "Coach not found.\n";
            return;
        }

        if (target->next == target) {
            head = nullptr;
            current = nullptr;
        } else {
            target->prev->next = target->next;
            target->next->prev = target->prev;
            if (target == head) {
                head = target->next;
            }
            if (target == current) {
                current = target->next;
            }
        }

        delete target;
        cout << "Coach removed.\n";
    }

    void moveForward() {
        if (current == nullptr) {
            cout << "Train is empty.\n";
            return;
        }
        current = current->next;
        displayCurrent();
    }

    void moveBackward() {
        if (current == nullptr) {
            cout << "Train is empty.\n";
            return;
        }
        current = current->prev;
        displayCurrent();
    }

    void displayClockwise() {
        if (head == nullptr) {
            cout << "Train is empty.\n";
            return;
        }
        Coach* temp = head;
        do {
            printCoach(temp);
            if (temp == current) {
                cout << "  <- current";
            }
            cout << "\n";
            temp = temp->next;
        } while (temp != head);
    }

    void displayAntiClockwise() {
        if (head == nullptr) {
            cout << "Train is empty.\n";
            return;
        }
        Coach* start = head->prev;
        Coach* temp = start;
        do {
            printCoach(temp);
            if (temp == current) {
                cout << "  <- current";
            }
            cout << "\n";
            temp = temp->prev;
        } while (temp != start);
    }

    void search(int number) {
        Coach* found = findCoach(number);
        if (found == nullptr) {
            cout << "Coach not found.\n";
            return;
        }
        cout << "Coach found: ";
        printCoach(found);
        cout << "\n";
    }

    void maxAvailable() {
        if (head == nullptr) {
            cout << "Train is empty.\n";
            return;
        }
        Coach* best = head;
        Coach* temp = head->next;
        while (temp != head) {
            if (temp->capacity - temp->passengers > best->capacity - best->passengers) {
                best = temp;
            }
            temp = temp->next;
        }
        cout << "Most empty seats: ";
        printCoach(best);
        cout << ", " << best->capacity - best->passengers << " seats free\n";
    }

    void displayCurrent() {
        if (current == nullptr) {
            cout << "Train is empty.\n";
            return;
        }
        cout << "Current coach: ";
        printCoach(current);
        cout << "\n";
    }

    // swap next and prev in every coach, old last coach becomes head
    void reverse() {
        if (head == nullptr) {
            cout << "Train is empty.\n";
            return;
        }
        Coach* temp = head;
        do {
            Coach* oldNext = temp->next;
            temp->next = temp->prev;
            temp->prev = oldNext;
            temp = oldNext;
        } while (temp != head);
        head = head->next;
        cout << "Train direction reversed.\n";
    }
};

int readInt() {
    int value;
    while (!(cin >> value)) {
        if (cin.eof()) {
            return 0;
        }
        cout << "Invalid input. Enter a number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return value;
}

void readCoach(int& number, string& type, int& capacity, int& passengers) {
    cout << "Coach number: ";
    number = readInt();
    cin.ignore(1000, '\n');
    cout << "Coach type: ";
    getline(cin, type);
    cout << "Passenger capacity: ";
    capacity = readInt();
    while (capacity < 0) {
        cout << "Capacity can't be negative. Enter again: ";
        capacity = readInt();
    }
    cout << "Current passengers: ";
    passengers = readInt();
    while (passengers < 0 || passengers > capacity) {
        cout << "Must be between 0 and " << capacity << ". Enter again: ";
        passengers = readInt();
    }
}

int main() {
    Train train;
    int number, capacity, passengers;
    string type;

    cout << "Number of coaches: ";
    int n = readInt();
    for (int i = 1; i <= n; i++) {
        cout << "\nCoach " << i << "\n";
        readCoach(number, type, capacity, passengers);
        train.addCoach(number, type, capacity, passengers);
    }

    int choice;
    do {
        cout << "\n";
        cout << "1. Add Coach\n";
        cout << "2. Insert Coach\n";
        cout << "3. Remove Coach\n";
        cout << "4. Move Forward\n";
        cout << "5. Move Backward\n";
        cout << "6. Display Train Clockwise\n";
        cout << "7. Display Train Anti-clockwise\n";
        cout << "8. Search Coach\n";
        cout << "9. Find Maximum Available Capacity\n";
        cout << "10. Display Current Coach\n";
        cout << "11. Reverse Train Direction\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        choice = readInt();

        if (choice == 1) {
            readCoach(number, type, capacity, passengers);
            train.addCoach(number, type, capacity, passengers);
        } else if (choice == 2) {
            cout << "Insert after coach number: ";
            int afterNumber = readInt();
            readCoach(number, type, capacity, passengers);
            train.insertAfter(afterNumber, number, type, capacity, passengers);
        } else if (choice == 3) {
            cout << "Enter coach number to remove: ";
            train.removeCoach(readInt());
        } else if (choice == 4) {
            train.moveForward();
        } else if (choice == 5) {
            train.moveBackward();
        } else if (choice == 6) {
            train.displayClockwise();
        } else if (choice == 7) {
            train.displayAntiClockwise();
        } else if (choice == 8) {
            cout << "Enter coach number to search: ";
            train.search(readInt());
        } else if (choice == 9) {
            train.maxAvailable();
        } else if (choice == 10) {
            train.displayCurrent();
        } else if (choice == 11) {
            train.reverse();
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
