#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int data) {
        this->data = data;
        left = NULL;
        right = NULL;
    }
};

class BST {
public:
    Node* root;
    int count;

    BST() {
        root = NULL;
    }

    Node* insert(Node* node, int data) {
        if (node == NULL)
            return new Node(data);
        if (data < node->data)
            node->left = insert(node->left, data);
        else
            node->right = insert(node->right, data);
        return node;
    }

    void inorder(Node* node) {
        if (node == NULL)
            return;
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    void preSuc(int data) {
        Node* pre = NULL;
        Node* suc = NULL;
        Node* curr = root;

        while (curr != NULL) {
            if (data < curr->data) {
                suc = curr;
                curr = curr->left;
            }
            else if (data > curr->data) {
                pre = curr;
                curr = curr->right;
            }
            else {
                Node* temp = curr->left;
                while (temp != NULL) {
                    pre = temp;
                    temp = temp->right;
                }
                temp = curr->right;
                while (temp != NULL) {
                    suc = temp;
                    temp = temp->left;
                }
                break;
            }
        }

        cout << "Predecessor of " << data << " : " << pre->data << endl;
        cout << "Successor of " << data << " : " << suc->data << endl;
    }

    void rangeRecursive(Node* node, int low, int high) {
        if (node == NULL)
            return;
        count++;
        if (low < node->data)
            rangeRecursive(node->left, low, high);
        if (node->data >= low && node->data <= high)
            cout << node->data << " ";
        if (high > node->data)
            rangeRecursive(node->right, low, high);
    }

    void rangeIterative(int low, int high) {
        Node* stack[100];
        int top = -1;
        Node* curr = root;

        while (curr != NULL || top != -1) {
            while (curr != NULL) {
                stack[++top] = curr;
                if (low < curr->data)
                    curr = curr->left;
                else
                    curr = NULL;
            }
            curr = stack[top--];
            count++;
            if (curr->data >= low && curr->data <= high)
                cout << curr->data << " ";
            if (high > curr->data)
                curr = curr->right;
            else
                curr = NULL;
        }
    }
};

int main() {
    int arr[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    BST tree;

    for (int i = 0; i < 11; i++)
        tree.root = tree.insert(tree.root, arr[i]);

    cout << "Inorder : ";
    tree.inorder(tree.root);
    cout << endl << endl;

    tree.preSuc(40);

    tree.count = 0;
    cout << "\nRange 30 to 70 (Recursive) : ";
    tree.rangeRecursive(tree.root, 30, 70);
    cout << "\nComparisons : " << tree.count << endl;

    tree.count = 0;
    cout << "\nRange 30 to 70 (Iterative) : ";
    tree.rangeIterative(30, 70);
    cout << "\nComparisons : " << tree.count << endl;

    return 0;
}
