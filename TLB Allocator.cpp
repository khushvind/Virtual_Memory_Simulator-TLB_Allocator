#include <iostream>
#include <unordered_map>
#include <cmath>
#include <climits>
#include <bits/stdc++.h>

using namespace std;

// Node structure to represent each element in the data structures
struct Node {
    uint32_t value;
    Node* next;
    Node* prev;
    Node(uint32_t val) : value(val), next(nullptr), prev(nullptr) {}
};

// Custom stack implementation using linked list
class CustomStack {
private:
    Node* top; // Pointer to the top element of the stack
    int count; // Number of elements in the stack

public:
    CustomStack() : top(nullptr), count(0) {}

    ~CustomStack() {
        // Destructor to clean up the stack
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    // Push a new value onto the stack
    void push(uint32_t value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        count++;
    }

    // Pop the top value from the stack
    void pop() {
        if (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
            count--;
        } else {
            throw runtime_error("Stack is empty");
        }
    }

    // Peek at the top value without removing it
    uint32_t peek() {
        if (top != nullptr) {
            return top->value;
        }
        throw runtime_error("Stack is empty");
    }

    // Get the number of elements in the stack
    int size() const {
        return count;
    }

    // Check if the stack is empty
    bool empty() const {
        return count == 0;
    }
};

// Custom queue implementation using linked list
class CustomQueue {
private:
    Node* head; // front element of the queue
    Node* tail; // rear element of the queue
    int count; 

public:
    CustomQueue() : head(nullptr), tail(nullptr), count(0) {}

    ~CustomQueue() {
        // Destructor to clean up the queue
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Enqueue a new value to the rear of the queue
    void push(uint32_t value) {
        Node* newNode = new Node(value);
        if (tail != nullptr) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == nullptr) {
            head = newNode;
        }
        count++;
    }

    // Dequeue the front value from the queue
    void pop() {
        if (head != nullptr) {
            Node* temp = head;
            head = head->next;
            if (head == nullptr) {
                tail = nullptr;
            }
            delete temp;
            count--;
        } else {
            throw runtime_error("Queue is empty");
        }
    }

    // Peek at the front value without removing it
    uint32_t front() {
        if (head != nullptr) {
            return head->value;
        }
        throw runtime_error("Queue is empty");
    }

    // Get the number of elements in the queue
    int size() const {
        return count;
    }

    // Check if the queue is empty
    bool empty() const {
        return count == 0;
    }
};

// Custom doubly linked list implementation
class CustomDoubleLinkedList {
private:
    Node* head; // head of the list
    Node* tail; // tail of the list
    int count;  // Number of elements in the list

public:
    CustomDoubleLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~CustomDoubleLinkedList() {
        // Destructor to clean up the list
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Add a new value to the front of the list
    void push_front(uint32_t value) {
        Node* newNode = new Node(value);
        if (head != nullptr) {
            head->prev = newNode;
            newNode->next = head;
        }
        head = newNode;
        if (tail == nullptr) {
            tail = newNode;
        }
        count++;
    }

    // Remove the value from the back of the list
    void pop_back() {
        if (tail != nullptr) {
            Node* temp = tail;
            tail = tail->prev;
            if (tail != nullptr) {
                tail->next = nullptr;
            } else {
                head = nullptr;
            }
            delete temp;
            count--;
        }
    }

    // Get the value from the back of the list
    uint32_t back() const {
        if (tail != nullptr) {
            return tail->value;
        }
        throw runtime_error("List is empty");
    }

    // Erase a specific node from the list
    void erase(Node* node) {
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
        delete node;
        count--;
    }

    // Get the number of elements in the list
    int size() const {
        return count;
    }

    // Get the pointer to the head of the list
    Node* begin() const {
        return head;
    }
};

class TLBReplacementSimulator {
private:
    int K;
    const uint32_t* VPNs;
    int size;

public:
    TLBReplacementSimulator(int K, const uint32_t* VPNs, int size)
        : K(K), VPNs(VPNs), size(size) {}

    ~TLBReplacementSimulator() {
        // No dynamic memory allocation in TLBReplacementSimulator, so nothing to clean up
    }

    // Function to simulate FIFO (First In First Out) replacement algorithm
    int simulateFIFO() {
        CustomQueue tlb;
        unordered_map<uint32_t, int> tlbMap;
        int hits = 0;

        for (int i = 0; i < size; i++) {
            uint32_t vpn = VPNs[i];
            if (tlbMap.find(vpn) != tlbMap.end()) {
                // TLB hit
                hits++;
            } else {
                // TLB miss
                if (tlb.size() == K) {
                    uint32_t oldest = tlb.front();
                    tlb.pop();
                    tlbMap.erase(oldest);
                }
                tlb.push(vpn);
                tlbMap[vpn] = 1;
            }
        }
        return hits;
    }

    // Function to simulate LIFO (Last In First Out) replacement algorithm
    int simulateLIFO() {
        CustomStack tlb = CustomStack();
        unordered_map<uint32_t, int> tlbMap;
        int hits = 0;

        for (int i = 0; i < size; i++) {
            uint32_t vpn = VPNs[i];
            if (tlbMap.find(vpn) != tlbMap.end()) {
                // TLB hit
                hits++;
            } else {
                // TLB miss
                if (tlb.size() == K) {
                    uint32_t recent = tlb.peek(); // Access the most recent element
                    tlb.pop(); // Remove the most recent element
                    tlbMap.erase(recent);
                }
                tlb.push(vpn); // Add new vpn to the stack
                tlbMap[vpn] = 1;
            }
        }
        return hits;
    }

    // Function to simulate LRU (Least Recently Used) replacement algorithm
    int simulateLRU() {
        CustomDoubleLinkedList tlb;
        unordered_map<uint32_t, Node*> tlbMap;
        int hits = 0;

        for (int i = 0; i < size; i++) {
            uint32_t vpn = VPNs[i];
            if (tlbMap.find(vpn) != tlbMap.end()) {
                // TLB hit
                hits++;
                // Move the accessed page to the front of the list
                tlb.erase(tlbMap[vpn]);
                tlb.push_front(vpn);
                tlbMap[vpn] = tlb.begin();
            } else {
                // TLB miss
                if (tlb.size() == K) {
                    // Remove the least recently used page
                    uint32_t lruPage = tlb.back();
                    tlb.pop_back();
                    tlbMap.erase(lruPage);
                }
                // Add the new page to the front of the list
                tlb.push_front(vpn);
                tlbMap[vpn] = tlb.begin();
            }
        }
        return hits;
    }

    unordered_map<uint32_t, Node*> createOccurrenceMap() {
        unordered_map<uint32_t,Node*> occurrenceMap;
        for (int i = size-1; i >= 0; i--) {
            Node* temp = occurrenceMap[VPNs[i]];
            occurrenceMap[VPNs[i]] = new Node(i);
            occurrenceMap[VPNs[i]]->next = temp;
        }
        return occurrenceMap;
    }

    // Function to simulate OPT (Optimal) replacement algorithm 
    int simulateOPT() {
        unordered_map<uint32_t, Node*> occurrenceMap = createOccurrenceMap();
        unordered_map<uint32_t, int> tlbMap;
        int hits = 0;

        for (int i = 0; i < size; i++) {
            uint32_t vpn = VPNs[i];
            
            if (tlbMap.find(vpn) != tlbMap.end()) {
                // TLB hit
                hits++;
            } else {
                // TLB miss
                if (tlbMap.size() == K) {
                    // Pptimal vpn to replace
                    uint32_t farthest = i;
                    uint32_t replaceVPN = 0;
                    for (const auto& entry : tlbMap) {
                        uint32_t vpn_ = entry.first;
                        while (occurrenceMap[vpn_] && occurrenceMap[vpn_]->value <= i) {
                            Node* temp = occurrenceMap[vpn_];
                            occurrenceMap[vpn_] = occurrenceMap[vpn_]->next;
                            delete temp; // Free the memory to avoid memory leak
                        }
                        auto occurrence = occurrenceMap[vpn_];
                        if (occurrence == nullptr) {
                            replaceVPN = vpn_;
                            break;
                        } else if (occurrence->value > farthest) {
                            farthest = occurrence->value;
                            replaceVPN = vpn_;
                        }
                    }
                    tlbMap.erase(replaceVPN);
                }
                tlbMap[vpn] = 1;
            }
            // Clean up the current VPN's occurrence list
            Node* &currentOccurrence = occurrenceMap[vpn];
            while (currentOccurrence && currentOccurrence->value <= i) {
                Node* temp = currentOccurrence;
                currentOccurrence = currentOccurrence->next;
                delete temp;
            }
        }

        // Clean up the occurrenceMap to avoid memory leaks
        for (auto& entry : occurrenceMap) {
            Node* current = entry.second;
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }

        return hits;
    }
};

int main() {
    int T;
    cin >> T;
    while (T--) {
        // Address Space, Page Size, TLB Size, Number of Addresses
        int S, P, K, N;
        cin >> S >> P >> K >> N;
        uint32_t* pageNumbers = new uint32_t[N];
        uint32_t* VPNs = new uint32_t[N];
        for (uint32_t i = 0; i < N; i++) {
            cin >> hex >> pageNumbers[i]; 
        }
        cin>>dec;
        int n_offset = log2(P)+10;
        
        for (uint32_t i = 0; i < N; i++) {
            VPNs[i] = pageNumbers[i] >> n_offset;
        }

        TLBReplacementSimulator simulator(K, VPNs, N);

        int fifoHits = simulator.simulateFIFO();
        int lifoHits = simulator.simulateLIFO();
        int lruHits = simulator.simulateLRU();
        int optHits = simulator.simulateOPT();

        cout << fifoHits << " " << lifoHits << " " << lruHits << " " << optHits << endl;
        delete[] pageNumbers;
        delete[] VPNs;
    }

    return 0;
}