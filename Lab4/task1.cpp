#include <iostream>
#include <string>
using namespace std;

struct Song {
    int id;
    string name;
    int minutes;
    int seconds;
    Song* prev;
    Song* next;

    Song(int i, string n, int m, int s) {
        id = i;
        name = n;
        minutes = m;
        seconds = s;
        prev = nullptr;
        next = nullptr;
    }
};

class Playlist {
private:
    Song* head;
    Song* tail;
    Song* current;

    void printSong(Song* s) {
        cout << "[" << s->id << "] " << s->name << " (" << s->minutes << ":";
        if (s->seconds < 10) {
            cout << "0";
        }
        cout << s->seconds << ")";
    }

    Song* findSong(int id) {
        Song* temp = head;
        while (temp != nullptr) {
            if (temp->id == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

public:
    Playlist() {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }

    ~Playlist() {
        Song* temp = head;
        while (temp != nullptr) {
            Song* nextSong = temp->next;
            delete temp;
            temp = nextSong;
        }
    }

    void addSong(int id, string name, int minutes, int seconds) {
        if (findSong(id) != nullptr) {
            cout << "A song with ID " << id << " already exists.\n";
            return;
        }

        Song* newSong = new Song(id, name, minutes, seconds);

        if (head == nullptr) {
            head = newSong;
            tail = newSong;
            current = newSong;
        } else {
            tail->next = newSong;
            newSong->prev = tail;
            tail = newSong;
        }
        cout << "Song added.\n";
    }

    void deleteSong(int id) {
        Song* target = findSong(id);
        if (target == nullptr) {
            cout << "Song not found.\n";
            return;
        }

        // move current away so it doesn't point to deleted memory
        if (current == target) {
            if (target->next != nullptr) {
                current = target->next;
            } else {
                current = target->prev;
            }
        }

        if (target->prev != nullptr) {
            target->prev->next = target->next;
        } else {
            head = target->next;
        }

        if (target->next != nullptr) {
            target->next->prev = target->prev;
        } else {
            tail = target->prev;
        }

        delete target;
        cout << "Song deleted.\n";
    }

    void displayForward() {
        if (head == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }
        Song* temp = head;
        while (temp != nullptr) {
            printSong(temp);
            if (temp == current) {
                cout << "  <- now playing";
            }
            cout << "\n";
            temp = temp->next;
        }
    }

    void displayBackward() {
        if (tail == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }
        Song* temp = tail;
        while (temp != nullptr) {
            printSong(temp);
            if (temp == current) {
                cout << "  <- now playing";
            }
            cout << "\n";
            temp = temp->prev;
        }
    }

    void search(int id) {
        Song* found = findSong(id);
        if (found == nullptr) {
            cout << "Song not found.\n";
            return;
        }
        cout << "Song found: ";
        printSong(found);
        cout << "\n";
    }

    void playNext() {
        if (current == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }
        if (current->next == nullptr) {
            cout << "This is the last song.\n";
        } else {
            current = current->next;
        }
        cout << "Now playing: ";
        printSong(current);
        cout << "\n";
    }

    void playPrevious() {
        if (current == nullptr) {
            cout << "Playlist is empty.\n";
            return;
        }
        if (current->prev == nullptr) {
            cout << "This is the first song.\n";
        } else {
            current = current->prev;
        }
        cout << "Now playing: ";
        printSong(current);
        cout << "\n";
    }

    // swap next and prev in every node, then swap head and tail
    void reverse() {
        Song* temp = head;
        while (temp != nullptr) {
            Song* oldNext = temp->next;
            temp->next = temp->prev;
            temp->prev = oldNext;
            temp = oldNext;
        }
        Song* oldHead = head;
        head = tail;
        tail = oldHead;
        cout << "Playlist reversed.\n";
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

void readDuration(int& minutes, int& seconds) {
    char colon;
    cout << "Duration (mm:ss): ";
    while (!(cin >> minutes >> colon >> seconds) || colon != ':' || minutes < 0 || seconds < 0 || seconds > 59) {
        if (cin.eof()) {
            minutes = 0;
            seconds = 0;
            return;
        }
        cout << "Invalid duration. Enter as mm:ss (e.g. 3:45): ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

int main() {
    Playlist playlist;
    int choice;

    do {
        cout << "\n";
        cout << "1. Add Song\n";
        cout << "2. Delete Song\n";
        cout << "3. Display Forward\n";
        cout << "4. Display Backward\n";
        cout << "5. Search Song\n";
        cout << "6. Play Next\n";
        cout << "7. Play Previous\n";
        cout << "8. Reverse Playlist\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        choice = readInt();

        if (choice == 1) {
            int id, minutes, seconds;
            string name;
            cout << "Song ID: ";
            id = readInt();
            cout << "Song name: ";
            cin.ignore(1000, '\n');
            getline(cin, name);
            readDuration(minutes, seconds);
            playlist.addSong(id, name, minutes, seconds);
        } else if (choice == 2) {
            cout << "Enter ID to delete: ";
            playlist.deleteSong(readInt());
        } else if (choice == 3) {
            playlist.displayForward();
        } else if (choice == 4) {
            playlist.displayBackward();
        } else if (choice == 5) {
            cout << "Enter ID to search: ";
            playlist.search(readInt());
        } else if (choice == 6) {
            playlist.playNext();
        } else if (choice == 7) {
            playlist.playPrevious();
        } else if (choice == 8) {
            playlist.reverse();
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
