#include <iostream>
#include <string>
using namespace std;

struct Photo {
    int id;
    string name;
    string date;
    string location;
    Photo* prev;
    Photo* next;

    Photo(int i, string n, string d, string l) {
        id = i;
        name = n;
        date = d;
        location = l;
        prev = nullptr;
        next = nullptr;
    }
};

class Album {
private:
    Photo* head;
    Photo* current;

    void printPhoto(Photo* p) {
        cout << "[" << p->id << "] " << p->name << ", " << p->date << ", " << p->location;
    }

    Photo* findPhoto(int id) {
        if (head == nullptr) {
            return nullptr;
        }
        Photo* temp = head;
        do {
            if (temp->id == id) {
                return temp;
            }
            temp = temp->next;
        } while (temp != head);
        return nullptr;
    }

    // unlinks a node from the list and frees it
    void removeNode(Photo* target) {
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
    }

public:
    Album() {
        head = nullptr;
        current = nullptr;
    }

    ~Album() {
        if (head == nullptr) {
            return;
        }
        // break the circle, then delete like a normal list
        head->prev->next = nullptr;
        Photo* temp = head;
        while (temp != nullptr) {
            Photo* nextPhoto = temp->next;
            delete temp;
            temp = nextPhoto;
        }
    }

    void addPhoto(int id, string name, string date, string location) {
        if (findPhoto(id) != nullptr) {
            cout << "A photo with ID " << id << " already exists.\n";
            return;
        }

        Photo* newPhoto = new Photo(id, name, date, location);

        if (head == nullptr) {
            // a single photo points to itself
            newPhoto->next = newPhoto;
            newPhoto->prev = newPhoto;
            head = newPhoto;
            current = newPhoto;
        } else {
            // the last photo is head->prev
            Photo* tail = head->prev;
            tail->next = newPhoto;
            newPhoto->prev = tail;
            newPhoto->next = head;
            head->prev = newPhoto;
        }
        cout << "Photo added.\n";
    }

    void insertAfterCurrent(int id, string name, string date, string location) {
        if (current == nullptr) {
            addPhoto(id, name, date, location);
            return;
        }
        if (findPhoto(id) != nullptr) {
            cout << "A photo with ID " << id << " already exists.\n";
            return;
        }

        Photo* newPhoto = new Photo(id, name, date, location);
        newPhoto->next = current->next;
        newPhoto->prev = current;
        current->next->prev = newPhoto;
        current->next = newPhoto;
        cout << "Photo inserted after current.\n";
    }

    void removePhoto(int id) {
        Photo* target = findPhoto(id);
        if (target == nullptr) {
            cout << "Photo not found.\n";
            return;
        }
        removeNode(target);
        cout << "Photo removed.\n";
    }

    void removeCurrent() {
        if (current == nullptr) {
            cout << "Album is empty.\n";
            return;
        }
        removeNode(current);
        cout << "Current photo removed.\n";
    }

    void moveNext() {
        if (current == nullptr) {
            cout << "Album is empty.\n";
            return;
        }
        current = current->next;
        cout << "Current photo: ";
        printPhoto(current);
        cout << "\n";
    }

    void movePrevious() {
        if (current == nullptr) {
            cout << "Album is empty.\n";
            return;
        }
        current = current->prev;
        cout << "Current photo: ";
        printPhoto(current);
        cout << "\n";
    }

    void displayForward() {
        if (current == nullptr) {
            cout << "Album is empty.\n";
            return;
        }
        Photo* temp = current;
        do {
            printPhoto(temp);
            if (temp == current) {
                cout << "  <- current";
            }
            cout << "\n";
            temp = temp->next;
        } while (temp != current);
    }

    void displayBackward() {
        if (current == nullptr) {
            cout << "Album is empty.\n";
            return;
        }
        Photo* temp = current;
        do {
            printPhoto(temp);
            if (temp == current) {
                cout << "  <- current";
            }
            cout << "\n";
            temp = temp->prev;
        } while (temp != current);
    }

    void search(int id) {
        Photo* found = findPhoto(id);
        if (found == nullptr) {
            cout << "Photo not found.\n";
            return;
        }
        cout << "Photo found: ";
        printPhoto(found);
        cout << "\n";
    }

    void countPhotos() {
        int count = 0;
        if (head != nullptr) {
            Photo* temp = head;
            do {
                count++;
                temp = temp->next;
            } while (temp != head);
        }
        cout << "Total photos: " << count << "\n";
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

void readPhoto(int& id, string& name, string& date, string& location) {
    cout << "Photo ID: ";
    id = readInt();
    cin.ignore(1000, '\n');
    cout << "Photo name: ";
    getline(cin, name);
    cout << "Date taken: ";
    getline(cin, date);
    cout << "Location: ";
    getline(cin, location);
}

int main() {
    Album album;
    int id;
    string name, date, location;

    cout << "Number of photos: ";
    int n = readInt();
    for (int i = 1; i <= n; i++) {
        cout << "\nPhoto " << i << "\n";
        readPhoto(id, name, date, location);
        album.addPhoto(id, name, date, location);
    }

    int choice;
    do {
        cout << "\n";
        cout << "1. Add Photo\n";
        cout << "2. Insert Photo After Current\n";
        cout << "3. Remove Photo\n";
        cout << "4. Remove Current Photo\n";
        cout << "5. Move Next\n";
        cout << "6. Move Previous\n";
        cout << "7. Display Album Forward\n";
        cout << "8. Display Album Backward\n";
        cout << "9. Search Photo\n";
        cout << "10. Count Photos\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        choice = readInt();

        if (choice == 1) {
            readPhoto(id, name, date, location);
            album.addPhoto(id, name, date, location);
        } else if (choice == 2) {
            readPhoto(id, name, date, location);
            album.insertAfterCurrent(id, name, date, location);
        } else if (choice == 3) {
            cout << "Enter ID to remove: ";
            album.removePhoto(readInt());
        } else if (choice == 4) {
            album.removeCurrent();
        } else if (choice == 5) {
            album.moveNext();
        } else if (choice == 6) {
            album.movePrevious();
        } else if (choice == 7) {
            album.displayForward();
        } else if (choice == 8) {
            album.displayBackward();
        } else if (choice == 9) {
            cout << "Enter ID to search: ";
            album.search(readInt());
        } else if (choice == 10) {
            album.countPhotos();
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
