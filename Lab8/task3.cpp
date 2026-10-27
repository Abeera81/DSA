    #include <iostream>
    using namespace std;

    int stackArr[3][100];
    int top[3];

    void push(int peg, int disk) {
        top[peg]++;
        stackArr[peg][top[peg]] = disk;
    }

    int pop(int peg) {
        int val = stackArr[peg][top[peg]];
        top[peg]--;
        return val;
    }

    int peek(int peg) {
        return stackArr[peg][top[peg]];
    }

    bool isEmpty(int peg) {
        return top[peg] == -1;
    }

    void moveDisks(int fromPeg, int toPeg, char fromName, char toName) {
        if (isEmpty(fromPeg)) {
            int disk = pop(toPeg);
            push(fromPeg, disk);
            cout << "Move disk " << disk << " from " << toName << " to " << fromName << endl;
        } else if (isEmpty(toPeg)) {
            int disk = pop(fromPeg);
            push(toPeg, disk);
            cout << "Move disk " << disk << " from " << fromName << " to " << toName << endl;
        } else if (peek(fromPeg) < peek(toPeg)) {
            int disk = pop(fromPeg);
            push(toPeg, disk);
            cout << "Move disk " << disk << " from " << fromName << " to " << toName << endl;
        } else {
            int disk = pop(toPeg);
            push(fromPeg, disk);
            cout << "Move disk " << disk << " from " << toName << " to " << fromName << endl;
        }
    }

    int main() {
        int n;
        cout << "Enter number of disks: ";
        cin >> n;

        for (int i = 0; i < 3; i++)
            top[i] = -1;

        for (int i = n; i >= 1; i--)
            push(0, i);

        char names[3] = {'A', 'B', 'C'};
        int source = 0, aux = 1, dest = 2;

        if (n % 2 == 0) {
            int temp = aux;
            aux = dest;
            dest = temp;
        }

        int totalMoves = 1;
        for (int i = 0; i < n; i++)
            totalMoves = totalMoves * 2;
        totalMoves = totalMoves - 1;

        for (int i = 1; i <= totalMoves; i++) {
            if (i % 3 == 1)
                moveDisks(source, dest, names[source], names[dest]);
            else if (i % 3 == 2)
                moveDisks(source, aux, names[source], names[aux]);
            else
                moveDisks(aux, dest, names[aux], names[dest]);
        }

        cout << "Total moves made: " << totalMoves << endl;

        return 0;
    }
