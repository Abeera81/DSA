#include <iostream>
#include <string>
#include <stack>
using namespace std;

struct Node {
    char data;
    Node* left;
    Node* right;
    Node(char val) : data(val), left(NULL), right(NULL) {}
};

// 1. Build a binary tree from the string representation.
int pos = 0;

Node* buildTree(string s) {
    if (pos >= (int)s.size() || s[pos] == ',' || s[pos] == ')')
        return NULL; // empty child

    Node* node = new Node(s[pos]);
    pos = pos + 1;

    if (pos < (int)s.size() && s[pos] == '(') {
        pos++; // skip '('
        node->left = buildTree(s);
        if (pos < (int)s.size() && s[pos] == ',') pos++; 
        node->right = buildTree(s);
        if (pos < (int)s.size() && s[pos] == ')') pos++;
    }
    return node;
}

// 2. Preorder traversal using recursion.
void preorderRecursive(Node* root) {
    if (!root) return;
    cout << root->data << " ";
    preorderRecursive(root->left);
    preorderRecursive(root->right);
}

// 3. Inorder traversal using a stack instead of recursion.
void inorderIterative(Node* root) {
    stack<Node*> st;
    Node* curr = root;
    while (curr != NULL || !st.empty()) {
        while (curr != NULL) {
            st.push(curr);
            curr = curr->left;
        }
        curr = st.top(); st.pop();
        cout << curr->data << " ";
        curr = curr->right;
    }
}

// 4. Postorder traversal using a stack.
void postorderIterative(Node* root) {
    if (!root) return;
    stack<Node*> st1, st2;
    st1.push(root);
    while (!st1.empty()) {
        Node* node = st1.top(); st1.pop();
        st2.push(node);
        if (node->left) st1.push(node->left);
        if (node->right) st1.push(node->right);
    }
    while (!st2.empty()) {
        cout << st2.top()->data << " ";
        st2.pop();
    }
}

// 5. Validate the string without constructing the tree.
bool validateNode(const string& s, int& pos) {
    if (pos >= (int)s.size()) return false;
    if (s[pos] == ',' || s[pos] == ')') return true; 

    if (!isalpha(s[pos])) return false;
    pos++;

    if (pos < (int)s.size() && s[pos] == '(') {
        pos++;
        if (!validateNode(s, pos)) return false;
        if (pos >= (int)s.size() || s[pos] != ',') return false;
        pos++;
        if (!validateNode(s, pos)) return false;
        if (pos >= (int)s.size() || s[pos] != ')') return false;
        pos++;
    }
    return true;
}

bool isValidTreeString(const string& s) {
    int pos = 0;
    return validateNode(s, pos) && pos == (int)s.size();
}

// 6. Height of the tree and depth of each node.
int height(Node* root) {
    if (!root) return -1; 
    return 1 + max(height(root->left), height(root->right));
}

void printDepths(Node* root, int depth = 0) {
    if (!root) return;
    cout << "Node " << root->data << " -> depth " << depth << endl;
    printDepths(root->left, depth + 1);
    printDepths(root->right, depth + 1);
}

int main() {
    string treeStr = "a(b(d,e),c(,f(g,)))";

    cout << "Validating tree string: " << treeStr << endl;
    cout << "Valid? " << (isValidTreeString(treeStr) ? "Yes" : "No") << endl << endl;

    pos = 0;
    Node* root = buildTree(treeStr);

    cout << "Preorder (recursive): ";
    preorderRecursive(root);
    cout << endl;

    cout << "Inorder (iterative with stack): ";
    inorderIterative(root);
    cout << endl;

    cout << "Postorder (iterative with stack): ";
    postorderIterative(root);
    cout << endl << endl;

    cout << "Height of tree: " << height(root) << endl;
    cout << "Depth of each node:" << endl;
    printDepths(root);

    return 0;
}

