#include <iostream>
#include <string>
using namespace std;

int patients[100], count = 0, nextId = 1;

void RegisterPatient() {
    if (count == 100) {
        cout << "Waiting room is full.\n";
        return;
    }
    patients[count++] = nextId;
    cout << "Registered patient " << nextId++ << "\n";
}

void ServePatient() {
    if (count == 0) {
        cout << "No patients are waiting.\n";
        return;
    }
    cout << "Serving patient " << patients[0] << "\n";
    for (int i = 1; i < count; i++) patients[i - 1] = patients[i];
    count--;
}

void CancelAll() {
    count = 0;
    cout << "All appointments cancelled.\n";
}

bool CanDoctorGoHome() {
    return count == 0;
}

void ShowAllPatient() {
    if (count == 0) {
        cout << "No patients are waiting.\n";
        return;
    }
    cout << "Waiting patient IDs: ";
    // IDs increase with registration, so queue order is already sorted.
    for (int i = 0; i < count; i++) cout << patients[i] << " ";
    cout << "\n";
}

int main() {
    string choice;
    while (true) {
        cout << "\n1. Register\n2. Serve\n3. Cancel all\n";
        cout << "4. Can doctor go home?\n5. Show patients\n6. Exit\nChoice: ";
        if (!getline(cin, choice) || choice == "6") break;
        if (choice == "1") RegisterPatient();
        else if (choice == "2") ServePatient();
        else if (choice == "3") CancelAll();
        else if (choice == "4")
            cout << (CanDoctorGoHome() ? "Yes\n" : "No\n");
        else if (choice == "5") ShowAllPatient();
        else cout << "Invalid choice.\n";
    }
}
    