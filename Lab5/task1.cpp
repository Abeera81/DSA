#include <iostream>
#include <string>
using namespace std;

struct Tab {
    int id;
    string title;
    string url;
    Tab* prev;
    Tab* next;

    Tab(int i, string t, string u) {
        id = i;
        title = t;
        url = u;
        prev = nullptr;
        next = nullptr;
    }
};

class TabManager {
private:
    Tab* current;

    void printTab(Tab* t) {
        cout << "[" << t->id << "] " << t->title << " (" << t->url << ")";
    }

    Tab* findTab(int id) {
        if (current == nullptr) {
            return nullptr;
        }
        Tab* temp = current;
        do {
            if (temp->id == id) {
                return temp;
            }
            temp = temp->next;
        } while (temp != current);
        return nullptr;
    }

public:
    TabManager() {
        current = nullptr;
    }

    ~TabManager() {
        if (current == nullptr) {
            return;
        }
        // break the circle, then delete like a normal list
        current->prev->next = nullptr;
        Tab* temp = current;
        while (temp != nullptr) {
            Tab* nextTab = temp->next;
            delete temp;
            temp = nextTab;
        }
    }

    void openNewTab(int id, string title, string url) {
        if (findTab(id) != nullptr) {
            cout << "A tab with ID " << id << " already exists.\n";
            return;
        }

        Tab* newTab = new Tab(id, title, url);

        if (current == nullptr) {
            // a single tab points to itself
            newTab->next = newTab;
            newTab->prev = newTab;
        } else {
            newTab->next = current->next;
            newTab->prev = current;
            current->next->prev = newTab;
            current->next = newTab;
        }
        current = newTab;
        cout << "Tab opened.\n";
    }

    void closeCurrentTab() {
        if (current == nullptr) {
            cout << "No tabs open.\n";
            return;
        }

        Tab* target = current;

        if (target->next == target) {
            current = nullptr;
        } else {
            target->prev->next = target->next;
            target->next->prev = target->prev;
            current = target->next;
        }

        delete target;
        cout << "Tab closed.\n";
    }

    void moveNext() {
        if (current == nullptr) {
            cout << "No tabs open.\n";
            return;
        }
        current = current->next;
        displayCurrent();
    }

    void movePrevious() {
        if (current == nullptr) {
            cout << "No tabs open.\n";
            return;
        }
        current = current->prev;
        displayCurrent();
    }

    void displayCurrent() {
        if (current == nullptr) {
            cout << "No tabs open.\n";
            return;
        }
        cout << "Current tab: ";
        printTab(current);
        cout << "\n";
    }

    void displayForward() {
        if (current == nullptr) {
            cout << "No tabs open.\n";
            return;
        }
        Tab* temp = current;
        do {
            printTab(temp);
            if (temp == current) {
                cout << "  <- current";
            }
            cout << "\n";
            temp = temp->next;
        } while (temp != current);
    }

    void displayBackward() {
        if (current == nullptr) {
            cout << "No tabs open.\n";
            return;
        }
        Tab* temp = current;
        do {
            printTab(temp);
            if (temp == current) {
                cout << "  <- current";
            }
            cout << "\n";
            temp = temp->prev;
        } while (temp != current);
    }

    void search(int id) {
        Tab* found = findTab(id);
        if (found == nullptr) {
            cout << "Tab not found.\n";
            return;
        }
        cout << "Tab found: ";
        printTab(found);
        cout << "\n";
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

int main() {
    TabManager browser;
    int choice;

    do {
        cout << "\n";
        cout << "1. Open New Tab\n";
        cout << "2. Close Current Tab\n";
        cout << "3. Move Next\n";
        cout << "4. Move Previous\n";
        cout << "5. Display Current Tab\n";
        cout << "6. Display All Tabs Forward\n";
        cout << "7. Display All Tabs Backward\n";
        cout << "8. Search Tab\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        choice = readInt();

        if (choice == 1) {
            int id;
            string title, url;
            cout << "Tab ID: ";
            id = readInt();
            cin.ignore(1000, '\n');
            cout << "Website title: ";
            getline(cin, title);
            cout << "URL: ";
            getline(cin, url);
            browser.openNewTab(id, title, url);
        } else if (choice == 2) {
            browser.closeCurrentTab();
        } else if (choice == 3) {
            browser.moveNext();
        } else if (choice == 4) {
            browser.movePrevious();
        } else if (choice == 5) {
            browser.displayCurrent();
        } else if (choice == 6) {
            browser.displayForward();
        } else if (choice == 7) {
            browser.displayBackward();
        } else if (choice == 8) {
            cout << "Enter ID to search: ";
            browser.search(readInt());
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
