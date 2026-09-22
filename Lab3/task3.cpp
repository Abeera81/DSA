#include<iostream>
using namespace std;

struct Node
{
    int data;
    Node* next;
};

void insertAtHead(Node*& head, int value)
{
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
}

void insertAtThird(Node*& head, int value)
{
    Node* newNode = new Node;
    newNode->data = value;

    // fewer than 2 nodes means there is no 3rd position yet
    if(head == nullptr || head->next == nullptr)
    {
        cout << "List has fewer than 2 nodes - inserting at the end instead." << endl;
        newNode->next = nullptr;

        if(head == nullptr)
        {
            head = newNode;
        }
        else
        {
            head->next = newNode;
        }
        return;
    }

    newNode->next = head->next->next;
    head->next->next = newNode;
}

void displayList(Node* head)
{
    if(head == nullptr)
    {
        cout << "List is empty." << endl;
        return;
    }

    Node* temp = head;
    while(temp != nullptr)
    {
        cout << temp->data << " -> ";
        temp = temp->next;
    }
    cout << "NULL" << endl;
}

void deleteLast(Node*& head)
{
    if(head == nullptr)
    {
        cout << "List is empty - nothing to delete." << endl;
        return;
    }

    if(head->next == nullptr)
    {
        delete head;
        head = nullptr;
        cout << "Last node deleted. List is now empty." << endl;
        return;
    }

    // stop at the second-last node
    Node* temp = head;
    while(temp->next->next != nullptr)
    {
        temp = temp->next;
    }

    delete temp->next;
    temp->next = nullptr;
    cout << "Last node deleted." << endl;
}

int countNodes(Node* head)
{
    int count = 0;
    Node* temp = head;

    while(temp != nullptr)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

void reverseList(Node*& head)
{
    Node* prev = nullptr;
    Node* curr = head;
    Node* next = nullptr;

    while(curr != nullptr)
    {
        next = curr->next;    // save it before the link is overwritten
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
}

int searchValue(Node* head, int value)
{
    Node* temp = head;
    int pos = 1;

    while(temp != nullptr)
    {
        if(temp->data == value)
        {
            return pos;
        }
        temp = temp->next;
        pos++;
    }
    return -1;
}

void destroyList(Node*& head)
{
    while(head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main()
{
    Node* head = nullptr;
    int choice, value;

    do
    {
        cout << "\n Singly Linked List Menu " << endl;
        cout << "1. Insert at head" << endl;
        cout << "2. Insert at 3rd position" << endl;
        cout << "3. Display list" << endl;
        cout << "4. Delete last node" << endl;
        cout << "5. Count nodes" << endl;
        cout << "6. Reverse list" << endl;
        cout << "7. Search for a value" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                cout << "Enter value: ";
                cin >> value;
                insertAtHead(head, value);
                displayList(head);
                break;

            case 2:
                cout << "Enter value: ";
                cin >> value;
                insertAtThird(head, value);
                displayList(head);
                break;

            case 3:
                displayList(head);
                break;

            case 4:
                deleteLast(head);
                displayList(head);
                break;

            case 5:
                cout << "Number of nodes: " << countNodes(head) << endl;
                break;

            case 6:
                reverseList(head);
                cout << "List reversed." << endl;
                displayList(head);
                break;

            case 7:
            {
                cout << "Enter value to search: ";
                cin >> value;
                int pos = searchValue(head, value);

                if(pos == -1)
                {
                    cout << value << " not found in the list." << endl;
                }
                else
                {
                    cout << value << " found at position " << pos << "." << endl;
                }
                break;
            }

            case 8:
                destroyList(head);
                cout << "List destroyed. Exiting." << endl;
                break;

            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while(choice != 8);

    return 0;
}
