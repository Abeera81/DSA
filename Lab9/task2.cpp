#include <iostream>
#include <string>
using namespace std;

class Node {
public:
    string word;
    int count;
    Node* left;
    Node* right;

    Node(string word) {
        this->word = word;
        count = 1;
        left = NULL;
        right = NULL;
    }
};

class BST {
public:
    Node* root;
    string words[100];
    int counts[100];
    int n;
    int unique;
    int total;

    BST() {
        root = NULL;
        unique = 0;
        total = 0;
    }

    Node* insert(Node* node, string word) {
        if (node == NULL) {
            unique++;
            return new Node(word);
        }
        if (word < node->word)
            node->left = insert(node->left, word);
        else if (word > node->word)
            node->right = insert(node->right, word);
        else
            node->count++;
        return node;
    }

    void insertParagraph(string text) {
        string word = "";
        for (int i = 0; i <= text.length(); i++) {
            if (i < text.length() && isalpha(text[i]))
                word += tolower(text[i]);
            else if (word != "") {
                root = insert(root, word);
                total++;
                word = "";
            }
        }
    }

    void inorder(Node* node) {
        if (node == NULL)
            return;
        inorder(node->left);
        cout << node->word << " : " << node->count << endl;
        inorder(node->right);
    }

    void displayAlphabetical() {
        inorder(root);
    }

    void fillArrays(Node* node) {
        if (node == NULL)
            return;
        fillArrays(node->left);
        words[n] = node->word;
        counts[n] = node->count;
        n++;
        fillArrays(node->right);
    }

    void displayByFrequency() {
        n = 0;
        fillArrays(root);

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (counts[j] < counts[j + 1]) {
                    swap(counts[j], counts[j + 1]);
                    swap(words[j], words[j + 1]);
                }
            }
        }

        for (int i = 0; i < n; i++)
            cout << words[i] << " : " << counts[i] << endl;
    }

    void findMostFrequentWord() {
        n = 0;
        fillArrays(root);

        int max = 0;
        for (int i = 0; i < n; i++)
            if (counts[i] > max)
                max = counts[i];

        for (int i = 0; i < n; i++)
            if (counts[i] == max)
                cout << words[i] << " : " << counts[i] << endl;
    }
};

int main() {
    string text = "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";
    BST tree;

    tree.insertParagraph(text);

    cout << "Words in Alphabetical Order :" << endl;
    tree.displayAlphabetical();

    cout << "\nWords by Frequency :" << endl;
    tree.displayByFrequency();

    cout << "\nMost Frequent Word(s) :" << endl;
    tree.findMostFrequentWord();

    cout << "\nTotal Unique Words : " << tree.unique << endl;
    cout << "Total Word Count   : " << tree.total << endl;

    return 0;
}
