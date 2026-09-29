#include <iostream>
#include <string>
using namespace std;

struct BitNode {
    int bit;
    BitNode* prev;
    BitNode* next;

    BitNode(int b) {
        bit = b;
        prev = nullptr;
        next = nullptr;
    }
};

class BinaryNumber {
private:
    BitNode* head;  // msb
    BitNode* tail;  // lsb
    int length;

public:
    BinaryNumber() {
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    ~BinaryNumber() {
        clear();
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void insertFront(int b) {
        BitNode* node = new BitNode(b);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        length++;
    }

    void insertBack(int b) {
        BitNode* node = new BitNode(b);
        if (tail == nullptr) {
            head = node;
            tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
        length++;
    }

    void deleteFront() {
        if (head == nullptr) {
            return;
        }
        BitNode* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        length--;
    }

    void clear() {
        while (head != nullptr) {
            deleteFront();
        }
    }

    // add leading zeros until the number fills complete 8-bit blocks
    void padTo8Bits() {
        while (length == 0 || length % 8 != 0) {
            insertFront(0);
        }
    }

    void copyTo(BinaryNumber& target) {
        target.clear();
        BitNode* temp = head;
        while (temp != nullptr) {
            target.insertBack(temp->bit);
            temp = temp->next;
        }
    }

    bool store(string bits) {
        if (bits.length() == 0) {
            return false;
        }
        for (int i = 0; i < bits.length(); i++) {
            if (bits[i] != '0' && bits[i] != '1') {
                return false;
            }
        }
        clear();
        for (int i = 0; i < bits.length(); i++) {
            insertBack(bits[i] - '0');
        }
        padTo8Bits();
        return true;
    }

    void display() {
        BitNode* temp = head;
        int count = 0;
        while (temp != nullptr) {
            if (count > 0 && count % 8 == 0) {
                cout << " ";
            }
            cout << temp->bit;
            count++;
            temp = temp->next;
        }
    }

    void onesComplement() {
        BitNode* temp = head;
        while (temp != nullptr) {
            if (temp->bit == 0) {
                temp->bit = 1;
            } else {
                temp->bit = 0;
            }
            temp = temp->next;
        }
    }

    // start from the tails (rightmost bits) and move left, carrying like normal addition
    void add(BinaryNumber& other, BinaryNumber& result) {
        result.clear();
        BitNode* a = tail;
        BitNode* b = other.tail;
        int carry = 0;

        while (a != nullptr || b != nullptr || carry != 0) {
            int sum = carry;
            if (a != nullptr) {
                sum += a->bit;
                a = a->prev;
            }
            if (b != nullptr) {
                sum += b->bit;
                b = b->prev;
            }
            result.insertFront(sum % 2);
            carry = sum / 2;
        }
        result.padTo8Bits();
    }

    void twosComplement(BinaryNumber& result) {
        copyTo(result);
        result.onesComplement();

        BinaryNumber one, sum;
        one.store("1");
        result.add(one, sum);

        // keep the same number of bits, so an extra carry at the front is dropped
        while (sum.length > length) {
            sum.deleteFront();
        }
        sum.copyTo(result);
    }

    // for every 1 in the multiplier, add the shifted multiplicand to the result
    void multiply(BinaryNumber& other, BinaryNumber& result) {
        result.store("0");

        BinaryNumber shifted, temp;
        copyTo(shifted);

        BitNode* b = other.tail;
        while (b != nullptr) {
            if (b->bit == 1) {
                result.add(shifted, temp);
                temp.copyTo(result);
            }
            shifted.insertBack(0);  
            b = b->prev;
        }

        // remove extra 8-bit blocks of zeros at the front
        while (result.length > 8) {
            BitNode* check = result.head;
            bool allZero = true;
            for (int i = 0; i < 8; i++) {
                if (check->bit == 1) {
                    allZero = false;
                }
                check = check->next;
            }
            if (!allZero) {
                break;
            }
            for (int i = 0; i < 8; i++) {
                result.deleteFront();
            }
        }
    }

    unsigned long long toDecimal() {
        unsigned long long value = 0;
        BitNode* temp = head;
        while (temp != nullptr) {
            value = value * 2 + temp->bit;
            temp = temp->next;
        }
        return value;
    }
};

void show(string label, BinaryNumber& num) {
    cout << label;
    num.display();
    cout << "\n";
}

void readBinary(string label, BinaryNumber& num) {
    string bits;
    cout << "Enter binary number " << label << ": ";
    cin >> bits;
    while (cin && !num.store(bits)) {
        cout << "Only 0 and 1 are allowed. Enter again: ";
        cin >> bits;
    }
    cout << label << " stored as: ";
    num.display();
    cout << "\n";
}

// keeps asking until a number is typed, so letters can't break the menu
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
    BinaryNumber a, b;
    int choice;

    do {
        cout << "\n";
        cout << "1. Store Binary Number A\n";
        cout << "2. Store Binary Number B\n";
        cout << "3. 1's Complement of A\n";
        cout << "4. 2's Complement of A\n";
        cout << "5. Add A + B\n";
        cout << "6. Multiply A * B\n";
        cout << "7. Convert A and B to Decimal\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        choice = readInt();

        if (choice >= 3 && choice <= 7 && a.isEmpty()) {
            cout << "Store number A first.\n";
            continue;
        }
        if (choice >= 5 && choice <= 7 && b.isEmpty()) {
            cout << "Store number B first.\n";
            continue;
        }

        if (choice == 1) {
            readBinary("A", a);
        } else if (choice == 2) {
            readBinary("B", b);
        } else if (choice == 3) {
            BinaryNumber result;
            a.copyTo(result);
            result.onesComplement();
            show("1's complement: ", result);
        } else if (choice == 4) {
            BinaryNumber result;
            a.twosComplement(result);
            show("2's complement: ", result);
        } else if (choice == 5) {
            BinaryNumber result;
            a.add(b, result);
            show("A + B = ", result);
        } else if (choice == 6) {
            BinaryNumber result;
            a.multiply(b, result);
            show("A * B = ", result);
        } else if (choice == 7) {
            cout << "A in decimal: " << a.toDecimal() << "\n";
            cout << "B in decimal: " << b.toDecimal() << "\n";
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
