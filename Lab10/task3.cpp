#include <iostream>
#include <string>
using namespace std;

const int MAX = 100;

struct Service {
    string serviceID;
    int monthlyCost;
    Service* left;
    Service* right;
};

Service* insert(Service* root, string id, int cost) {
    if (root == NULL) {
        Service* newNode = new Service;
        newNode->serviceID = id;
        newNode->monthlyCost = cost;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (cost < root->monthlyCost) {
        root->left = insert(root->left, id, cost);
    } else {
        root->right = insert(root->right, id, cost);
    }
    return root;
}

void printInorder(Service* root) {
    if (root == NULL) return;
    printInorder(root->left);
    cout << root->serviceID << "(" << root->monthlyCost << ") ";
    printInorder(root->right);
}

void deleteTree(Service* root) {
    if (root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

Service* path[MAX];
int pathSize = 0;

int B;
int found = 0;

void printPath(int sum) {
    found++;
    cout << found << ") ";
    for (int i = 0; i < pathSize; i++) {
        cout << path[i]->serviceID << "(" << path[i]->monthlyCost << ")";
        if (i < pathSize - 1) {
            cout << " -> ";
        }
    }
    cout << "  = " << sum << endl;
}

void extendPath(Service* node, int sum) {
    if (node == NULL) {
        return;
    }

    path[pathSize] = node;
    pathSize++;
    sum = sum + node->monthlyCost;

    if (sum == B) {
        printPath(sum);
    }

    if (sum <= B) {
        extendPath(node->left, sum);
        extendPath(node->right, sum);
    }

    pathSize--;
}

void findAllPaths(Service* node) {
    if (node == NULL) {
        return;
    }
    extendPath(node, 0);
    findAllPaths(node->left);
    findAllPaths(node->right);
}

void runTest(Service* root, int budget) {
    B = budget;
    found = 0;
    pathSize = 0;

    cout << "Target budget B = " << B << endl;
    findAllPaths(root);
    if (found == 0) {
        cout << "No downward path sums to " << B << "." << endl;
    }
    cout << endl;
}

int main() {
    Service* root = NULL;
    root = insert(root, "S1", 50);
    root = insert(root, "S2", 30);
    root = insert(root, "S3", 70);
    root = insert(root, "S4", 20);
    root = insert(root, "S5", 40);
    root = insert(root, "S6", 60);
    root = insert(root, "S7", 80);
    root = insert(root, "S8", 10);
    root = insert(root, "S9", 25);

    cout << "Services (in order of cost): ";
    printInorder(root);
    cout << endl << endl;

    runTest(root, 120);
    runTest(root, 60);
    runTest(root, 5);

    deleteTree(root);
    return 0;
}
