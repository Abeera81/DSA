#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int data;
    ListNode* next;

    ListNode(int val) {
        data = val;
        next = nullptr;
    }
};

class MinHeap {
    vector<ListNode*> heap;

    void rise(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[i]->data < heap[parent]->data) {
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

            if (left < n && heap[left]->data < heap[smallest]->data)
                smallest = left;
            if (right < n && heap[right]->data < heap[smallest]->data)
                smallest = right;

            if (smallest == i)
                break;

            swap(heap[i], heap[smallest]);
            i = smallest;
        }
    }

public:
    void push(ListNode* node) {
        heap.push_back(node);
        rise(heap.size() - 1);
    }

    ListNode* pop() {
        ListNode* top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty())
            sink(0);
        return top;
    }

    bool empty() {
        return heap.empty();
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    MinHeap heap;
    for (int i = 0; i < (int)lists.size(); i++) {
        if (lists[i] != nullptr)
            heap.push(lists[i]);
    }

    ListNode dummy(0);
    ListNode* tail = &dummy;

    while (!heap.empty()) {
        ListNode* smallest = heap.pop();
        tail->next = smallest;
        tail = smallest;

        if (smallest->next != nullptr)
            heap.push(smallest->next);
    }

    tail->next = nullptr;
    return dummy.next;
}

void printList(ListNode* head) {
    while (head != nullptr) {
        cout << head->data;
        if (head->next != nullptr)
            cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    int k;
    cout << "Enter number of linked lists: ";
    cin >> k;

    vector<ListNode*> lists(k, nullptr);

    for (int i = 0; i < k; i++) {
        int size;
        cout << "Enter size of list " << i + 1 << ": ";
        cin >> size;

        if (size > 0)
            cout << "Enter " << size << " sorted elements: ";

        ListNode* tail = nullptr;
        for (int j = 0; j < size; j++) {
            int val;
            cin >> val;
            ListNode* node = new ListNode(val);
            if (lists[i] == nullptr)
                lists[i] = node;
            else
                tail->next = node;
            tail = node;
        }
    }

    cout << "\nInput lists:\n";
    for (int i = 0; i < k; i++) {
        cout << "List " << i + 1 << ": ";
        printList(lists[i]);
    }

    ListNode* merged = mergeKLists(lists);

    cout << "\nMerged list: ";
    printList(merged);

    deleteList(merged);
    return 0;
}
