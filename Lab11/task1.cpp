#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }
};

class MinHeap {
    vector<Node*> heap;

    void rise(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[i]->freq < heap[parent]->freq) {
                swap(heap[i], heap[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }

    void sink(int i) {
        int n = heap.size();
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < n && heap[left]->freq < heap[smallest]->freq)
                smallest = left;
            if (right < n && heap[right]->freq < heap[smallest]->freq)
                smallest = right;

            if (smallest == i)
                break;

            swap(heap[i], heap[smallest]);
            i = smallest;
        }
    }

public:
    void push(Node* node) {
        heap.push_back(node);
        rise(heap.size() - 1);
    }

    Node* pop() {
        Node* top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty())
            sink(0);
        return top;
    }

    int size() {
        return heap.size();
    }
};

Node* buildHuffmanTree(string s, vector<int>& f) {
    MinHeap heap;
    for (int i = 0; i < (int)s.size(); i++)
        heap.push(new Node(s[i], f[i]));

    while (heap.size() > 1) {
        Node* left = heap.pop();
        Node* right = heap.pop();

        Node* parent = new Node('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        heap.push(parent);
    }

    return heap.pop();
}

void printCodes(Node* root, string code) {
    if (root == nullptr)
        return;

    if (root->left == nullptr && root->right == nullptr) {
        if (code == "")
            code = "0";
        cout << root->ch << " : " << code << endl;
        return;
    }

    printCodes(root->left, code + "0");
    printCodes(root->right, code + "1");
}

void deleteTree(Node* root) {
    if (root == nullptr)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    int n;
    cout << "Enter number of characters: ";
    cin >> n;

    string s;
    cout << "Enter the characters (as one string): ";
    cin >> s;

    vector<int> f(n);
    cout << "Enter the frequencies: ";
    for (int i = 0; i < n; i++)
        cin >> f[i];

    Node* root = buildHuffmanTree(s, f);

    cout << "\nHuffman codes in preorder traversal:\n";
    printCodes(root, "");

    deleteTree(root);
    return 0;
}
