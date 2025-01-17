
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <thread>
#include <stack>
#include <list>
#include <set>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <unistd.h>
#include <stdexcept>

using namespace std;
// Function to animate printing of a file's content
void animatePrint(const string &filename)
{
    ifstream file(filename);
    string line;
    vector<string> lines;
    while (getline(file, line))
        cout << line << endl;
    sleep(1);
    file.close();
}

// Structure to represent equipment
struct Equipment
{
    int id;
    string name;
    string type;
    bool isAvailable;
    string currentLocation;
    int condition;
};

// Structure to represent a hospital
struct Hospital
{
    int id;
    string name;
    string location;
    int totalBeds;
    int availableBeds;
    bool hasEmergencyUnit;
    vector<string> specializations;
};

// Structure to represent a city
struct City
{
    string name;
    string state;
    double latitude;
    double longitude;
    int population;
    vector<Hospital> hospitals;
    vector<Equipment> equipment;
    int fireStations;
    int policeStations;
    int emergencyVehicles;
};

// Class to represent a location
class Location
{
private:
    double latitude;
    double longitude;
    string address;
    string city;
    string state;

public:
    // Default Constructor
    Location() : latitude(0.0), longitude(0.0), address(""), city(""), state("") {}

    // Parameterized Constructor
    Location(double latitude, double longitude, const string &address, const string &city, const string &state)
        : latitude(latitude), longitude(longitude), address(address), city(city), state(state) {}

    // Copy Constructor
    Location(const Location &other)
        : latitude(other.latitude), longitude(other.longitude), address(other.address), city(other.city), state(other.state) {}

    // Overloaded Assignment Operator
    Location &operator=(const Location &other)
    {
        if (this != &other)
        {
            latitude = other.latitude;
            longitude = other.longitude;
            address = other.address;
            city = other.city;
            state = other.state;
        }
        return *this;
    }
    friend class Disaster;
    template <typename T>
    friend class BPlusTree;
    friend class DisasterManagementSystem;
    friend ostream &operator<<(ostream &os, const Location &loc);
};

// Overloaded output stream operator for Location
ostream &operator<<(ostream &os, const Location &loc)
{
    os << loc.address << ", " << loc.city << ", " << loc.state << " (" << loc.latitude << ", " << loc.longitude << ")";
    return os;
}

// Class to represent a disaster
class Disaster
{
private:
    int id;
    string type;
    Location location;
    int severity;
    string status;
    string date;
    int affectedPopulation;
    vector<int> assignedTeams;
    vector<string> requiredResources;

public:
    // Constructor for convenience
    Disaster() {}
    Disaster(int id, const string &type, const Location &location, int severity,
             const string &status, const string &date, int affectedPopulation,
             const vector<int> &assignedTeams, const vector<string> &requiredResources)
        : id(id), type(type), location(location), severity(severity),
          status(status), date(date), affectedPopulation(affectedPopulation),
          assignedTeams(assignedTeams), requiredResources(requiredResources) {}

    // Comparison operator for ordering (based on id as an example)
    bool operator<(const Disaster &other) const
    {
        return id < other.id;
    }

    bool operator==(const Disaster &other) const
    {
        return id == other.id;
    }
    template <typename T>
    friend class BPlusTree;
    friend class DisasterManagementSystem;
    template <typename T>
    friend class HashTable;
    friend ostream &operator<<(ostream &os, const Disaster *D);
    friend class AlertManager;
};

// Overloaded output stream operator for Disaster
ostream &operator<<(ostream &os, const Disaster *D)
{
    os << "ID: " << D->id << endl;
    os << "Type: " << D->type << endl;
    os << "Status: " << D->status << endl;
    os << "Severity: " << D->severity << endl;
    os << "Date Of Disaster: " << D->date << endl;
    os << "Affected Population: " << D->affectedPopulation << endl;
    os << "Location: " << D->location << endl;
    os << "Assigned Rescue Teams: ";
    if (!D->assignedTeams.empty())
    {
        for (auto k : D->assignedTeams)
        {
            os << k << " ";
        }
    }
    return os;
}

// Class to represent a rescue team
class Team
{
private:
    int id;
    string skillset;
    string location;
    string status;

public:
    Team() {};
    Team(int id, string skillset, string location, string status)
    {
        this->id = id;
        this->skillset = skillset;
        this->location = location;
        this->status = status;
    }
    friend class RescueTeamManager;
    template <typename T>
    friend class HashTable;
    friend ostream &operator<<(ostream &os, const Team *D);
};

// Overloaded output stream operator for Team
ostream &operator<<(ostream &os, const Team *T)
{
    os << "Team ID: " << T->id << endl;
    os << "Skillset: " << T->skillset << endl;
    os << "Location: " << T->location << endl;
    os << "Status: " << T->status << endl;
    return os;
}

template <typename T>
class HashTable
{
    vector<T *> V;
    int size;
    int total;

public:
    // Constructor to initialize the hash table with a given size
    HashTable(int s)
    {
        V.resize(s, NULL);
        size = s;
        total = 0;
    }

    // Hash function to calculate the index for a given key
    int Hash(int k, int i = 0)
    {
        return (k + i) % size;
    }

    // Function to calculate the load factor of the hash table
    float Load()
    {
        return (1.0 * total) / size;
    }

    // Function to insert an element into the hash table
    void Insert(T *D)
    {
        int pos = Hash(D->id);
        int i = 1;
        while (V[pos] != NULL)
        {
            pos = Hash(D->id, i);
            i++;
        }
        V[pos] = D;
        total++;
        if (Load() > 0.7)
        {
            IncreaseSize(size * 2);
        }
    }

    // Function to increase the size of the hash table
    void IncreaseSize(int newSize)
    {
        vector<T *> oldTable = V;
        size = newSize;
        V.clear();
        V.resize(size, nullptr);
        total = 0;
        for (T *d : oldTable)
        {
            if (d != nullptr)
            {
                Insert(d);
            }
        }
    }

    // Function to search for an element in the hash table
    T *Search(int k)
    {
        int pos = Hash(k);
        int i = 1;
        while (V[pos] != NULL)
        {
            if (V[pos]->id == k)
                break;
            pos = Hash(k, i);
            i++;
        }
        return V[pos];
    }

    // Function to delete an element from the hash table
    void Delete(int k)
    {
        int pos = Hash(k);
        int i = 1;
        while (V[pos] != NULL)
        {
            if (V[pos]->id == k)
            {
                V[pos] = NULL;
                total--;
                return;
            }
            pos = Hash(k, i);
            i++;
        }
    }

    // Function to display all elements in the hash table
    void Display()
    {
        for (auto k : V)
        {
            if (k != NULL)
                cout << k << endl;
        }
    }

    // Function to write the hash table to a file
    void HashToFile(fstream &file)
    {
        for (auto k : V)
        {
            if (k != NULL)
                file.write(reinterpret_cast<char *>(k), sizeof(Team));
        }
    }

    friend class AlertManager;
};

template <typename T>
class BPlusTree
{
private:
    class Node
    {
    private:
        bool isLeaf;
        vector<T *> keys;
        vector<Node *> children;
        Node *next;

    public:
        // Constructor
        Node(bool leaf = false) : isLeaf(leaf), next(nullptr) {}
        friend class BPlusTree<T>;
    };

    Node *root;
    int t;

    // Function to split a child node
    void splitChild(Node *parent, int index, Node *child)
    {
        Node *newChild = new Node(child->isLeaf);
        parent->children.insert(parent->children.begin() + index + 1, newChild);
        parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);

        newChild->keys.assign(child->keys.begin() + t, child->keys.end());
        child->keys.resize(t - 1);

        if (!child->isLeaf)
        {
            newChild->children.assign(child->children.begin() + t, child->children.end());
            child->children.resize(t);
        }

        if (child->isLeaf)
        {
            newChild->next = child->next;
            child->next = newChild;
        }
    }

    // Function to insert a key into a non-full node
    void insertNonFull(Node *node, T *key)
    {
        if (node->isLeaf)
        {
            node->keys.insert(upper_bound(node->keys.begin(), node->keys.end(), key, [](const T *a, const T *b)
                                          { return a->id < b->id; }),
                              key);
        }
        else
        {
            int i = (int)node->keys.size() - 1;
            while (i >= 0 && key->id < node->keys[i]->id)
            {
                i--;
            }
            i++;
            if ((int)node->children[i]->keys.size() == 2 * t - 1)
            {
                splitChild(node, i, node->children[i]);
                if (key->id > node->keys[i]->id)
                {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }

    // Function to remove a key from a node
    void remove(Node *node, int key)
    {
        if (node->isLeaf)
        {
            auto it = std::find_if(node->keys.begin(), node->keys.end(),
                                   [key](const T *obj)
                                   { return obj->id == key; });
            if (it != node->keys.end())
            {
                node->keys.erase(it);
            }
        }
        else
        {
            int idx = std::lower_bound(node->keys.begin(), node->keys.end(), key,
                                       [](const T *a, int b)
                                       { return a->id < b; }) -
                      node->keys.begin();

            if (idx < node->keys.size() && node->keys[idx]->id == key)
            {
                if (node->children[idx]->keys.size() >= t)
                {
                    Node *predNode = node->children[idx];
                    while (!predNode->isLeaf)
                    {
                        predNode = predNode->children.back();
                    }
                    T *pred = predNode->keys.back();
                    node->keys[idx] = pred;
                    remove(node->children[idx], pred->id);
                }
                else if ((int)node->children[idx + 1]->keys.size() >= t)
                {
                    Node *succNode = node->children[idx + 1];
                    while (!succNode->isLeaf)
                    {
                        succNode = succNode->children.front();
                    }
                    T *succ = succNode->keys.front();
                    node->keys[idx] = succ;
                    remove(node->children[idx + 1], succ->id);
                }
                else
                {
                    merge(node, idx);
                    remove(node->children[idx], key);
                }
            }
            else
            {
                if ((int)node->children[idx]->keys.size() < t)
                {
                    if (idx > 0 && (int)node->children[idx - 1]->keys.size() >= t)
                    {
                        borrowFromPrev(node, idx);
                    }
                    else if (idx < (int)node->children.size() - 1 && (int)node->children[idx + 1]->keys.size() >= t)
                    {
                        borrowFromNext(node, idx);
                    }
                    else
                    {
                        if (idx < (int)node->children.size() - 1)
                        {
                            merge(node, idx);
                        }
                        else
                        {
                            merge(node, idx - 1);
                        }
                    }
                }
                remove(node->children[idx], key);
            }
        }
    }

    // Function to borrow a key from the previous sibling
    void borrowFromPrev(Node *node, int index)
    {
        Node *child = node->children[index];
        Node *sibling = node->children[index - 1];

        child->keys.insert(child->keys.begin(), node->keys[index - 1]);
        node->keys[index - 1] = sibling->keys.back();
        sibling->keys.pop_back();

        if (!child->isLeaf)
        {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
    }

    // Function to borrow a key from the next sibling
    void borrowFromNext(Node *node, int index)
    {
        Node *child = node->children[index];
        Node *sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);
        node->keys[index] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());

        if (!child->isLeaf)
        {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }
    }

    // Function to merge a node with its sibling
    void merge(Node *node, int index)
    {
        Node *child = node->children[index];
        Node *sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);
        child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());
        if (!child->isLeaf)
        {
            child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
        }

        node->keys.erase(node->keys.begin() + index);
        node->children.erase(node->children.begin() + index + 1);

        delete sibling;
    }

    // Function to print the B+ tree
    void printTree(Node *node, int level)
    {
        if (node != nullptr)
        {
            for (const T *key : node->keys)
            {
                cout << &*key << " ";
                cout << endl;
            }
            for (Node *child : node->children)
            {
                printTree(child, level + 1);
            }
        }
    }

    // Function to add active disasters to a hash table
    void AddDisasterToHash(HashTable<T> *H, Node *node, int level)
    {
        if (node != nullptr)
        {
            for (T *key : node->keys)
            {
                if (key != nullptr && key->status == "Active")
                {
                    if (H->Search(key->id) == nullptr)
                    {
                        H->Insert(key);
                    }
                }
            }
            for (Node *child : node->children)
            {
                AddDisasterToHash(H, child, level + 1);
            }
        }
    }

    // Function to add the B+ tree to a file
    void AddToFile(Node *node, int level)
    {
        if (node != nullptr)
        {
            for (T *key : node->keys)
            {
                fstream file;
                file.open("Disasters.dat", ios::app | ios::binary);
                file.write(reinterpret_cast<char *>(key), sizeof(Disaster));
                file.close();
            }

            for (Node *child : node->children)
            {
                AddToFile(child, level + 1);
            }
        }
    }

public:
    // Constructor to initialize the B+ tree with a given degree
    BPlusTree(int degree) : root(nullptr), t(degree) {}

    // Wrapper function to print the B+ tree
    void printTree()
    {
        printTree(root, 0);
    }

    // Function to search for a key in the B+ tree
    T *search(int key)
    {
        Node *current = root;
        while (current != nullptr)
        {
            int i = 0;
            while (i < (int)current->keys.size() && key > current->keys[i]->id)
            {
                i++;
            }
            if (i < (int)current->keys.size() && key == current->keys[i]->id)
            {
                return current->keys[i];
            }
            if (current->isLeaf)
            {
                return NULL;
            }
            current = current->children[i];
        }
        return NULL;
    }

    // Function to insert a key into the B+ tree
    void insert(T *key)
    {
        if (root == nullptr)
        {
            root = new Node(true);
            root->keys.push_back(key);
        }
        else
        {
            if ((int)root->keys.size() == 2 * t - 1)
            {
                Node *newRoot = new Node();
                newRoot->children.push_back(root);
                splitChild(newRoot, 0, root);
                root = newRoot;
            }
            insertNonFull(root, key);
        }
    }

    // Function to remove a key from the B+ tree
    void remove(int key)
    {
        if (root == nullptr)
        {
            return;
        }
        remove(root, key);
        if (root->keys.empty() && !root->isLeaf)
        {
            Node *tmp = root;
            root = root->children[0];
            delete tmp;
        }
    }

    // Function to add active disasters to a hash table
    void AddActiveDisaster(HashTable<T> *H)
    {
        AddDisasterToHash(H, root, 0);
    }

    // Function to add the B+ tree to a file
    void AddToFile()
    {
        fstream file;
        file.open("Disasters.dat", ios::out | ios::binary);
        file.close();
        AddToFile(root, 0);
    }
};
class Graph
{
private:
    map<string, vector<pair<string, double>>> adjList; // Adjacency list to store graph

public:
    // Function to add a node to the graph
    void addNode(const string &location)
    {
        if (adjList.find(location) == adjList.end())
        {
            adjList[location] = {};
        }
    }

    // Function to add an edge between two nodes
    void addEdge(const string &from, const string &to, double distance)
    {
        adjList[from].push_back({to, distance});
        adjList[to].push_back({from, distance});
    }

    // Function to find the shortest path using Dijkstra's algorithm
    vector<string> dijkstra(const string &start, const string &end)
    {
        unordered_map<string, double> distances; // Stores distances from start to each node
        unordered_map<string, string> previous;  // Stores previous node in optimal path
        set<pair<double, string>> pq;            // Priority queue to store nodes to be processed

        for (const auto &node : adjList)
        {
            distances[node.first] = numeric_limits<double>::max();
        }
        distances[start] = 0;
        pq.insert({0, start});

        while (!pq.empty())
        {
            auto currentPair = *pq.begin();
            string current = currentPair.second;
            pq.erase(pq.begin());

            if (current == end)
                break;

            for (const auto &neighbor : adjList[current])
            {
                string next = neighbor.first;
                double weight = neighbor.second;

                if (distances[current] + weight < distances[next])
                {
                    pq.erase({distances[next], next});
                    distances[next] = distances[current] + weight;
                    previous[next] = current;
                    pq.insert({distances[next], next});
                }
            }
        }

        vector<string> path;
        cout << endl;
        cout << "Shortest Distance To Relief Camp: " << distances[end] << endl;
        for (string at = end; !at.empty(); at = previous[at])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        return (path.size() > 1) ? path : vector<string>{};
    }

    // Function to print the graph
    void printGraph()
    {
        for (const auto &node : adjList)
        {
            cout << node.first << " -> ";
            for (const auto &neighbor : node.second)
            {
                cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
            }
            cout << endl;
        }
    }
};

// RoutingSystem class
class RoutingSystem
{
private:
    Graph graph; // Graph object to manage locations and routes

public:
    // Function to add a location to the graph
    void addLocation(const string &location)
    {
        graph.addNode(location);
    }

    // Function to add a route between two locations
    void addRoute(const string &from, const string &to, double distance)
    {
        graph.addEdge(from, to, distance);
    }

    // Function to calculate the optimal path between two locations
    vector<string> calculateOptimalPath(const string &start, const string &end)
    {
        return graph.dijkstra(start, end);
    }

    // Function to load graph data from a CSV file
    bool loadGraphFromCSV(const string &filename, int startLine) // Changed to bool
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Error: Unable to open file: " << filename << endl;
            return false;
        }

        string line;
        int currentLine = 0;

        while (getline(file, line))
        {
            currentLine++;
            if (currentLine < startLine)
                continue;

            stringstream ss(line);
            string from, to, distanceStr;
            double distance;

            getline(ss, from, ',');
            getline(ss, to, ',');
            getline(ss, distanceStr);

            if (from.empty() || to.empty() || distanceStr.empty())
            {
                cout << "Error: Invalid format in line " << currentLine << endl;
                continue;
            }

            try
            {
                distance = stod(distanceStr);
                addRoute(from, to, distance);
            }
            catch (const invalid_argument &e)
            {
                cout << "Error: Non-numeric distance in line " << currentLine << endl;
                continue;
            }
        }

        file.close();
        return true;
    }

    // Function to print the graph
    void printGraph()
    {
        graph.printGraph();
    }
};

// RescueTeam class to manage rescue teams
class RescueTeam
{
private:
    string teamID;                // ID of the rescue team
    string location;              // Current location of the rescue team
    int capacity;                 // Capacity of the rescue team
    bool availabilityStatus;      // Availability status of the rescue team
    vector<string> skills;        // Skills of the rescue team
    RoutingSystem *routingSystem; // Pointer to the RoutingSystem object

    struct Disaster
    {
        string disaster_ID; // ID of the disaster
        int severity;       // Severity of the disaster
    };

    // HashMap for location mapping to disasters
    unordered_map<string, Disaster> locationToDisaster;

public:
    // Constructor to initialize the rescue team
    RescueTeam(const string &id, const string &loc, int cap, bool status, const vector<string> &skillset, RoutingSystem *rs)
        : teamID(id), location(loc), capacity(cap), availabilityStatus(status), skills(skillset), routingSystem(rs) {}

    // Check for active disaster at a location
    void checkActiveDisaster(const string &loc)
    {
        if (locationToDisaster.find(loc) != locationToDisaster.end())
        {
            cout << "Active disaster at " + loc + ": Disaster ID = " + locationToDisaster[loc].disaster_ID +
                        ", Severity = " + to_string(locationToDisaster[loc].severity)
                 << endl;
        }
        else
        {
            cout << "No active disaster at " + loc << endl;
        }
    }

    // Update location using RoutingSystem's shortest path method
    void updateLocation(const string &targetLocation)
    {
        if (availabilityStatus)
        {
            vector<string> path = routingSystem->calculateOptimalPath(location, targetLocation);
            if (!path.empty())
            {
                cout << "Updating location. Shortest path: ";
                for (size_t i = 0; i < path.size(); ++i)
                {
                    cout << path[i];
                    if (i < path.size() - 1)
                        cout << " -> ";
                }
                cout << endl;
                location = targetLocation; // Update the team's location
            }
            else
            {
                cout << "No valid path to the target location." << endl;
            }
        }
        else
        {
            cout << "Rescue team is not available for relocation." << endl;
        }
    }

    // Check availability based on skills and status
    bool checkAvailability(const vector<string> &requiredSkills)
    {
        if (!availabilityStatus)
        {
            cout << "Rescue team is unavailable." << endl;
            return false;
        }
        for (const string &skill : requiredSkills)
        {
            if (find(skills.begin(), skills.end(), skill) == skills.end())
            {
                cout << "Rescue team lacks required skill: " + skill << endl;
                return false;
            }
        }
        cout << "Rescue team is available and has the required skills." << endl;
        return true;
    }
};
class Alert
{
    int alertID;    // Unique ID for the alert
    int disasterID; // ID of the associated disaster
    int severity;   // Severity level of the alert
    string message; // Message content of the alert
    string time;    // Time of the alert

public:
    Alert(int aID, int dID, int sev, const string &msg = "", const string &t = "")
        : alertID(aID), disasterID(dID), severity(sev), message(msg), time(t) {}

    int getSeverity() const { return severity; }

    friend ostream &operator<<(ostream &os, const Alert &a)
    {
        os << "[AlertID: " << a.alertID << ", DisasterID: " << a.disasterID
           << ", Severity: " << a.severity << ", Message: " << a.message
           << ", Time: " << a.time << "]";
        return os;
    }

    friend class MaxHeap;
    friend class DisasterManagementSystem;
    friend class AlertManager;
};

class MaxHeap
{
private:
    vector<Alert> heap;

    // Heapify-down to maintain heap property
    void heapifyDown(int index)
    {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heap.size() && heap[left].getSeverity() > heap[largest].getSeverity())
        {
            largest = left;
        }
        if (right < heap.size() && heap[right].getSeverity() > heap[largest].getSeverity())
        {
            largest = right;
        }

        if (largest != index)
        {
            swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

    // Heapify-up to maintain heap property
    void heapifyUp(int index)
    {
        int parent = (index - 1) / 2;
        if (index > 0 && heap[index].getSeverity() > heap[parent].getSeverity())
        {
            swap(heap[index], heap[parent]);
            heapifyUp(parent);
        }
    }

public:
    void insert(const Alert &alert)
    {
        heap.push_back(alert);
        heapifyUp((int)heap.size() - 1);
    }

    Alert extractMax()
    {
        if (heap.empty())
        {
            throw runtime_error("Heap is empty!");
        }
        Alert maxAlert = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return maxAlert;
    }

    bool isEmpty() const
    {
        return heap.empty();
    }
};
class AlertManager
{
private:
    string filename = "alerts.txt";

    // Function to read alerts from file
    vector<Alert> readAlertsFromFile() const
    {
        ifstream file(filename, ios::in);
        vector<Alert> alerts;
        if (!file)
        {
            cerr << "Error: Unable to open file for reading!" << endl;
            return alerts;
        }

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string temp;
            int alertID, disasterID, severity;
            string message, time;

            getline(ss, temp, ':');
            ss >> alertID;
            ss.ignore();

            getline(ss, temp, ':');
            ss >> disasterID;
            ss.ignore();

            getline(ss, temp, ':');
            ss >> severity;
            ss.ignore();

            getline(ss, temp, ':');
            getline(ss, message, ',');

            getline(ss, temp, ':');
            getline(ss, time);

            alerts.emplace_back(alertID, disasterID, severity, message, time);
        }

        file.close();
        return alerts;
    }

    // Function to write alerts to file
    void writeAlertsToFile(const vector<Alert> &alerts) const
    {
        ofstream file(filename, ios::out);
        if (!file)
        {
            cerr << "Error: Unable to open file for writing!" << endl;
            return;
        }

        for (const Alert &alert : alerts)
        {
            file << "AlertID: " << alert.alertID
                 << ", DisasterID: " << alert.disasterID
                 << ", Severity: " << alert.severity
                 << ", Message: " << alert.message
                 << ", Time: " << alert.time << endl;
        }

        file.close();
    }

public:
    // Function to send an alert
    void sendAlert(int alertID, int disasterID, int severity, const string &message = "", const string &time = "")
    {
        ofstream file(filename, ios::out | ios::app);
        if (file)
        {
            file << "AlertID: " << alertID
                 << ", DisasterID: " << disasterID
                 << ", Severity: " << severity
                 << ", Message: " << message
                 << ", Time: " << time << endl;
            file.close();
        }
        else
        {
            cerr << "Error: Unable to open file for writing!" << endl;
        }
        cout << "Alert sent successfully for DisasterID " << disasterID << "!" << endl;
    }

    // Function to respond to alert calls
    void respondToAlertCalls()
    {
        vector<Alert> alerts = readAlertsFromFile();
        if (alerts.empty())
        {
            cout << "No alerts to respond to!" << endl;
            return;
        }

        MaxHeap maxHeap;
        for (const Alert &alert : alerts)
        {
            maxHeap.insert(alert);
        }

        cout << "\nResponding to priority alerts from file:" << endl;
        vector<Alert> unprocessedAlerts;

        while (!maxHeap.isEmpty())
        {
            Alert topAlert = maxHeap.extractMax();
            cout << topAlert << endl;

            // Retain alerts that need further action
            if (topAlert.getSeverity() > 0)
            {
                unprocessedAlerts.push_back(topAlert);
            }
        }

        // Write back unprocessed alerts to the file
        if (!unprocessedAlerts.empty())
        {
            writeAlertsToFile(unprocessedAlerts);
        }
        else
        {
            cout << "All alerts have been processed. Clearing the file." << endl;
            ofstream file(filename, ios::trunc); // Clear the file if no alerts are left
            file.close();
        }
    }

    // Function to display alerts
    void displayAlerts() const
    {
        ifstream file(filename, ios::in);
        if (!file)
        {
            cerr << "Error: Unable to open file for reading!" << endl;
            return;
        }

        cout << "Current Alerts from file:" << endl;
        string line;
        while (getline(file, line))
        {
            cout << line << endl;
        }

        file.close();
    }
};

class TrieNode
{
public:
    unordered_map<char, TrieNode *> children;
    bool isEndOfWord;
    int id;
    TrieNode() : isEndOfWord(false) {}
};

class Trie
{
private:
    TrieNode *root;

    // Helper function for depth-first search
    void dfsHelper(TrieNode *node, string prefix, vector<string> &results)
    {
        if (node->isEndOfWord)
            results.push_back(prefix);

        for (auto &pair : node->children)
        {
            prefix.push_back(pair.first);
            dfsHelper(pair.second, prefix, results);
            prefix.pop_back();
        }
    }

public:
    Trie() : root(new TrieNode()) {}

    // Function to insert data from CSV to Trie
    void insertCsvtoTrie(Trie *trie, const string &filename)
    {
        int i = 0;
        ifstream file(filename);
        string allData, line;
        while (i < 180 && getline(file, line))
        {
            allData += line;
        }
        trie->insertBulk(allData);
        file.close();
    }

    // Function to insert a word into the Trie
    void insert(string word)
    {
        TrieNode *current = root;
        for (char c : word)
        {
            if (current->children.find(c) == current->children.end())
                current->children[c] = new TrieNode();
            current = current->children[c];
        }
        current->isEndOfWord = true;
    }

    // Function to search for a word in the Trie
    bool search(string word)
    {
        TrieNode *node = findNode(word);
        return node != NULL && node->isEndOfWord;
    }

    // Function to check if a prefix exists in the Trie
    bool startsWith(string prefix)
    {
        return findNode(prefix) != NULL;
    }

    // Function to get autocomplete suggestions for a prefix
    vector<string> autocomplete(string prefix)
    {
        vector<string> results;
        TrieNode *node = findNode(prefix);
        if (node)
        {
            string current = prefix;
            dfsHelper(node, current, results);
        }
        return results;
    }

    // Function to insert bulk data into the Trie
    void insertBulk(string data)
    {
        int start = 0, end = 0;
        while ((end = (int)data.find(',', start)) != string::npos)
        {
            insert(data.substr(start, end - start));
            start = end + 1;
        }
        if (start < data.length())
        {
            insert(data.substr(start));
        }
    }

private:
    // Helper function to find a node for a given prefix
    TrieNode *findNode(string prefix)
    {
        TrieNode *current = root;
        for (char c : prefix)
        {
            if (current->children.find(c) == current->children.end())
            {
                return NULL;
            }
            current = current->children[c];
        }
        return current;
    }
};
struct Shelter
{
    string name;
    int capacity;
    int available_space;
    double latitude;
    double longitude;

    Shelter(string n, int cap, int avail, double lat, double lon)
        : name(n), capacity(cap), available_space(avail), latitude(lat), longitude(lon) {}
};

struct KDNode
{
    Shelter shelter;
    KDNode *left;
    KDNode *right;

    KDNode(Shelter s) : shelter(s), left(nullptr), right(nullptr) {}
};

class KDTree
{
private:
    KDNode *root;
    const string filename = "shelter.txt";

    // Recursive function to insert a shelter into the KDTree
    KDNode *insertRec(KDNode *node, Shelter shelter, unsigned depth)
    {
        if (!node)
            return new KDNode(shelter);

        unsigned cd = depth % 2;
        if ((cd == 0 && shelter.latitude < node->shelter.latitude) || (cd == 1 && shelter.longitude < node->shelter.longitude))
            node->left = insertRec(node->left, shelter, depth + 1);
        else
            node->right = insertRec(node->right, shelter, depth + 1);

        return node;
    }

    // Function to calculate the distance between two points
    double distance(double lat1, double lon1, double lat2, double lon2)
    {
        return sqrt(pow(lat1 - lat2, 2) + pow(lon1 - lon2, 2));
    }

    // Recursive function to search for the nearest shelter
    void searchNearestRec(KDNode *node, double lat, double lon, unsigned depth, KDNode *&best, double &bestDist)
    {
        if (!node)
            return;

        double d = distance(lat, lon, node->shelter.latitude, node->shelter.longitude);
        if (d < bestDist)
        {
            bestDist = d;
            best = node;
        }

        unsigned cd = depth % 2;
        KDNode *nextBranch = (cd == 0 && lat < node->shelter.latitude) || (cd == 1 && lon < node->shelter.longitude)
                                 ? node->left
                                 : node->right;
        KDNode *otherBranch = (nextBranch == node->left) ? node->right : node->left;

        searchNearestRec(nextBranch, lat, lon, depth + 1, best, bestDist);

        if ((cd == 0 && fabs(lat - node->shelter.latitude) < bestDist) ||
            (cd == 1 && fabs(lon - node->shelter.longitude) < bestDist))
        {
            searchNearestRec(otherBranch, lat, lon, depth + 1, best, bestDist);
        }
    }

    // Recursive function to search for a shelter by name
    KDNode *searchRec(KDNode *node, const string &name)
    {
        if (!node)
            return nullptr;

        if (node->shelter.name == name)
            return node;

        KDNode *leftResult = searchRec(node->left, name);
        if (leftResult)
            return leftResult;

        return searchRec(node->right, name);
    }

    // Recursive function to find the minimum node in a subtree
    KDNode *findMin(KDNode *node, unsigned d, unsigned depth)
    {
        if (!node)
            return nullptr;

        unsigned cd = depth % 2;
        if (cd == d)
        {
            if (!node->left)
                return node;
            return findMin(node->left, d, depth + 1);
        }

        KDNode *leftMin = findMin(node->left, d, depth + 1);
        KDNode *rightMin = findMin(node->right, d, depth + 1);

        KDNode *minNode = node;
        if (leftMin && leftMin->shelter.latitude < minNode->shelter.latitude)
            minNode = leftMin;
        if (rightMin && rightMin->shelter.latitude < minNode->shelter.latitude)
            minNode = rightMin;

        return minNode;
    }

    // Recursive function to delete a shelter from the KDTree
    KDNode *deleteRec(KDNode *node, const string &name, unsigned depth)
    {
        if (!node)
            return nullptr;

        if (node->shelter.name == name)
        {
            if (!node->right)
                return node->left;
            if (!node->left)
                return node->right;

            KDNode *minNode = findMin(node->right, depth % 2, depth + 1);
            node->shelter = minNode->shelter;
            node->right = deleteRec(node->right, minNode->shelter.name, depth + 1);
        }
        else
        {
            unsigned cd = depth % 2;
            if ((cd == 0 && name < node->shelter.name) || (cd == 1 && name < node->shelter.name))
                node->left = deleteRec(node->left, name, depth + 1);
            else
                node->right = deleteRec(node->right, name, depth + 1);
        }
        return node;
    }

    // Recursive function to display all shelters in the KDTree
    void displaySheltersRec(KDNode *node)
    {
        if (!node)
            return;

        cout << "Shelter Name: " << node->shelter.name
             << ", Capacity: " << node->shelter.capacity
             << ", Available Space: " << node->shelter.available_space
             << ", Location: (" << node->shelter.latitude << ", " << node->shelter.longitude << ")" << endl;

        displaySheltersRec(node->left);
        displaySheltersRec(node->right);
    }

    // Recursive function to save the KDTree to a file
    void saveToFileRec(KDNode *node, ofstream &outfile)
    {
        if (!node)
            return;

        outfile << node->shelter.name << ","
                << node->shelter.capacity << ","
                << node->shelter.available_space << ","
                << node->shelter.latitude << ","
                << node->shelter.longitude << endl;

        saveToFileRec(node->left, outfile);
        saveToFileRec(node->right, outfile);
    }

    // Recursive function to load the KDTree from a file
    void loadFromFileRec(ifstream &infile)
    {
        string line;
        while (getline(infile, line))
        {
            stringstream ss(line);
            string name;
            int capacity, available_space;
            double latitude, longitude;

            getline(ss, name, ',');
            ss >> capacity;
            ss.ignore(1);
            ss >> available_space;
            ss.ignore(1);
            ss >> latitude;
            ss.ignore(1);
            ss >> longitude;

            Shelter shelter(name, capacity, available_space, latitude, longitude);
            addShelter(shelter);
        }
    }

public:
    KDTree() : root(nullptr)
    {
        loadData();
    }

    // Function to load data from the file
    void loadData()
    {
        ifstream infile(filename);
        if (!infile.is_open())
            throw runtime_error("Could not open file for loading.");
        loadFromFileRec(infile);
        infile.close();
    }

    // Function to save data to the file
    void saveData()
    {
        ofstream outfile(filename);
        if (!outfile.is_open())
            throw runtime_error("Could not open file for saving.");
        saveToFileRec(root, outfile);
        outfile.close();
    }

    // Function to search for a shelter by name
    Shelter searchShelterByName(const string &name)
    {
        KDNode *node = searchRec(root, name);
        if (!node)
            throw runtime_error("Shelter not found.");
        return node->shelter;
    }

    // Function to add a shelter to the KDTree
    void addShelter(Shelter shelter)
    {
        root = insertRec(root, shelter, 0);
    }

    // Function to update a shelter's details
    void updateShelter(const string &name, int newCapacity, int newAvailableSpace)
    {
        KDNode *node = searchRec(root, name);
        if (!node)
            throw runtime_error("Shelter not found.");
        node->shelter.capacity = newCapacity;
        node->shelter.available_space = newAvailableSpace;
        saveData();
    }

    // Function to delete a shelter from the KDTree
    void deleteShelter(const string &name)
    {
        root = deleteRec(root, name, 0);
        saveData();
    }

    // Function to search for the nearest shelter
    Shelter searchNearestShelter(double lat, double lon)
    {
        KDNode *best = nullptr;
        double bestDist = numeric_limits<double>::max();
        searchNearestRec(root, lat, lon, 0, best, bestDist);
        if (best)
            return best->shelter;
        throw runtime_error("No shelter found.");
    }

    // Function to get the available space in a shelter
    int getAvailableSpace(const string &name)
    {
        KDNode *node = searchRec(root, name);
        if (!node)
            throw runtime_error("Shelter not found.");
        return node->shelter.available_space;
    }

    // Function to get the location of a shelter
    pair<double, double> getLocation(const string &name)
    {
        KDNode *node = searchRec(root, name);
        if (!node)
            throw runtime_error("Shelter not found.");
        return {node->shelter.latitude, node->shelter.longitude};
    }

    // Function to display all shelters in the KDTree
    void displayShelters()
    {
        displaySheltersRec(root);
    }
};

class RescueTeamManager
{
private:
    HashTable<Team> *team;
    AlertManager *alert; // Alert manager for handling alerts

    // Helper function to save all teams' data to the file
    void saveTeamToFile()
    {
        fstream file;
        file.open("RescueTeam.dat", ios::out | ios::binary);
        if (!file)
        {
            cout << "Error opening file for writing.\n";
            return;
        }

        team->HashToFile(file);
        file.close();
    }

public:
    RescueTeamManager()
    {
        team = new HashTable<Team>(20);
        alert = new AlertManager;
    }

    // Function to add a new rescue team
    void addRescueTeam(int teamID, const string &skillset, const string &location)
    {
        Team *T = team->Search(teamID);
        if (T != NULL)
        {
            cout << "Team with ID " << teamID << " already exists. Cannot add duplicate team.\n";
            return;
        }

        // Add the team to the hash table
        Team *newTeam = new Team(teamID, skillset, location, "Active");
        team->Insert(newTeam);
        cout << "Rescue team added successfully.\n";
    }

    // Function to add a rescue team from an existing Team object
    void addRescueTeam(Team *R)
    {
        team->Insert(R);
    }

    // Function to display all rescue teams
    void displayTeams() const
    {
        cout << "\nRescue Teams:\n";
        team->Display();
    }

    // Function to delete a rescue team
    void deleteRescueTeam(int teamID)
    {
        Team *T = team->Search(teamID);
        if (T != NULL)
        {
            if (T->status == "Removed")
            {
                cout << "Team with ID " << teamID << " is already removed.\n";
                return;
            }
            T->status = "Removed";
            cout << "Rescue team with ID " << teamID << " has been removed.\n";
            return;
        }
        cout << "No team found with ID " << teamID << ".\n";
    }

    // Function to update a rescue team's details
    void updateRescueTeam(int teamID, const string &newSkillset, const string &newLocation)
    {
        Team *T = team->Search(teamID);
        if (T != NULL)
        {
            if (T->status == "Removed")
            {
                cout << "Team with ID " << teamID << " is already removed.\n";
                return;
            }
            T->skillset = newSkillset;
            T->location = newLocation;
            cout << "Rescue team with ID " << teamID << " has been updated.\n";
            return;
        }
        cout << "No team found with ID " << teamID << ".\n";
    }

    // Function to display alerts
    void DisplayAlert()
    {
        alert->displayAlerts();
    }

    friend class DisasterManagementSystem;
};

class DisasterManagementSystem
{
private:
    KDTree tree;                               // KDTree for managing shelters
    BPlusTree<Disaster> *disasters;            // B+ Tree for managing disasters
    HashTable<Disaster> *Activedisasters;      // Hash table for active disasters
    RescueTeamManager *Rescue;                 // Manager for rescue teams
    priority_queue<pair<int, int>> alertQueue; // Priority queue for alerts // Disaster severity queue
    RoutingSystem routingSystem;
    Trie trie;
    // AlertManager * alert;
    AlertManager *alertManager = new AlertManager();

    int currentDisasterId;
    int currentTeamId;
    int currentShelterId;
    int currentEquipmentId;

    int isLoggedIn; // 0: Not logged in, 1: Admin, 2: Rescue Team
    string currentUser;
    map<string, string> users;
    map<string, string> RescueUsers;

    void loadUsers()
    {
        ifstream file("users.txt");
        string username, password;
        while (getline(file, username))
        {
            getline(file, password);
            users[username] = password;
        }
        file.close();
    }
    void loadRescueUsers()
    {
        ifstream file("rescueUsers.txt");
        string username, password;
        while (getline(file, username))
        {
            getline(file, password);
            RescueUsers[username] = password;
        }
        file.close();
    }
    void showMenu()
    {
        cout << "\nWelcome to Relief Link! How can we help you?" << endl;
        cout << "1. Login as Admin" << endl;
        cout << "2. Login as Rescue Team" << endl;
        cout << "3. Request Help (Citizen)" << endl;
        cout << "4. Exit" << endl;
    }
    void showAdminMenu()
    {
        animatePrint("adminMenu.txt");
        while (true)
        {
            cout << "\nAdmin Menu:" << endl;
            cout << "1. Display Requests" << endl;
            cout << "2. Add Disaster" << endl;
            cout << "3. Update Disaster" << endl;
            cout << "4. Delete Disaster" << endl;
            cout << "5. Display Disaster" << endl;
            cout << "6. Add Rescue Team" << endl;
            cout << "7. Update Rescue Team" << endl;
            cout << "8. Delete Rescue Team" << endl;
            cout << "9. Display Rescue Team" << endl;
            cout << "10. Add Relief Camp" << endl;
            cout << "11. Update Relief Camp" << endl;
            cout << "12. Delete Relief Camp" << endl;
            cout << "13. Search Relief Camp" << endl;
            cout << "14. Display All Relief Camps" << endl;
            cout << "15. Get the available space left in shelter" << endl;
            cout << "16. Search nearest shelter" << endl;
            cout << "17. Get location of shelter" << endl;
            cout << "18. Add Alerts" << endl;
            cout << "19. Display Alerts" << endl;
            cout << "20. Logout" << endl;
            int adminChoice;
            cin >> adminChoice;
            switch (adminChoice)
            {
            case 1:
                displayRequests();
                break;
            case 2:
                addDisaster();
                break;
            case 3:
                updateDisaster();
                break;
            case 4:
                DeleteDisaster();
                break;
            case 5:
                disasters->printTree();
                break;
            case 6:
                addRescueTeam();
                break;
            case 7:
                updateRescueTeam();
                break;
            case 8:
                deleteRescueTeam();
                break;
            case 9:
                Rescue->displayTeams();
                break;
            case 10:
                insertShelter(tree);
                break;
            case 11:
                updateShelter(tree);
                break;
            case 12:
                deleteShelter(tree);
                break;
            case 13:
                searchShelterByNameFunction(tree);
                break;
            case 14:
                displayShelters(tree);
                break;
            case 15:
                getAvailableSpaceForShelter(tree);
                break;
            case 16:
                searchNearestShelter(tree);
                break;
            case 17:
                getLocationForShelter(tree);
                break;
            case 18:
                addalert();
                break;
            case 19:
                alertManager->respondToAlertCalls();
                break;
            case 20:
                isLoggedIn = 0;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
            }
            if (isLoggedIn == 0)
            {
                break;
            }
        }
    }
    void addalert()
    {
        int id, id1, s;
        string m, t;
        cout << "Enter the Alert ID: ";
        cin >> id;
        cout << "Enter the Disaster ID: ";
        cin >> id1;
        cout << "Severity: ";
        cin >> s;
        cout << "Message: ";
        cin >> m;
        cout << "Time: ";
        cin >> t;
        alertManager->sendAlert(id, id1, s, m, t);
    }
    void getAvailableSpaceForShelter(KDTree &tree)
    {
        string name;
        cout << "Enter the shelter name: ";
        cin >> name;
        int availableSpace = tree.getAvailableSpace(name);
        cout << "Available space for " << name << ": " << availableSpace << endl;
    }

    void getLocationForShelter(KDTree &tree)
    {
        string name;
        cout << "Enter the shelter name: ";
        cin >> name;
        auto location = tree.getLocation(name);
        cout << "Location of " << name << ": (" << location.first << ", " << location.second << ")" << endl;
    }

    void searchNearestShelter(KDTree &tree)
    {
        double latitude, longitude;
        cout << "Enter latitude: ";
        cin >> latitude;
        cout << "Enter longitude: ";
        cin >> longitude;

        Shelter nearest = tree.searchNearestShelter(latitude, longitude);
        cout << "Nearest shelter: " << nearest.name << endl;
    }

    void searchShelterByNameFunction(KDTree &tree)
    {
        string shelterName;
        cin.ignore();
        cout << "Enter the Shelter Name you want to search for: ";
        getline(cin, shelterName);

        try
        {
            Shelter foundShelter = tree.searchShelterByName(shelterName);
            cout << "Shelter Name: " << foundShelter.name
                 << ", Capacity: " << foundShelter.capacity
                 << ", Available Space: " << foundShelter.available_space
                 << ", Location: (" << foundShelter.latitude << ", " << foundShelter.longitude << ")" << endl;
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }

    void insertShelter(KDTree &tree)
    {
        string name;
        int capacity, available_space;
        double latitude, longitude;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Shelter Name: ";
        getline(cin, name);
        cout << "Enter Capacity: ";
        cin >> capacity;
        cout << "Enter Available Space: ";
        cin >> available_space;
        cout << "Enter Latitude: ";
        cin >> latitude;
        cout << "Enter Longitude: ";
        cin >> longitude;
        cin.ignore(); // Ignore newline character after cin

        Shelter shelter(name, capacity, available_space, latitude, longitude);
        tree.addShelter(shelter);
        cout << "Shelter added successfully.\n";
    }

    void updateShelter(KDTree &tree)
    {
        string name;
        int newCapacity, newAvailableSpace;
        cin.ignore();
        cout << "Enter Shelter Name to update: ";
        getline(cin, name);
        cout << "Enter New Capacity: ";
        cin >> newCapacity;
        cout << "Enter New Available Space: ";
        cin >> newAvailableSpace;
        cin.ignore();

        try
        {
            tree.updateShelter(name, newCapacity, newAvailableSpace);
            cout << "Shelter updated successfully.\n";
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }

    void deleteShelter(KDTree &tree)
    {
        string name;

        cout << "Enter Shelter Name to delete: ";
        getline(cin, name);

        try
        {
            tree.deleteShelter(name);
            cout << "Shelter deleted successfully.\n";
        }
        catch (const runtime_error &e)
        {
            cout << e.what() << endl;
        }
    }

    void displayShelters(KDTree &tree)
    {
        cout << "\nList of Shelters:\n";
        tree.displayShelters();
    }

    void displayalerts()
    {
        if (!alertQueue.empty())
            alertManager->displayAlerts();
    }
    void addRescueTeam()
    {
        int id;
        string skillset, location;
        cout << "Enter the Rescue Team ID: ";
        cin >> id;
        cout << "Enter the Rescue Team skillset: ";
        cin >> skillset;
        cout << "Location under Rescue Team: ";
        cin >> location;
        Rescue->addRescueTeam(id, skillset, location);
    }
    void updateRescueTeam()
    {
        int id;
        string skillset, location;
        cout << "Enter the Rescue Team ID: ";
        cin >> id;
        cout << "Enter the Rescue Team New Skillset: ";
        cin >> skillset;
        cout << "New Location under Rescue Team: ";
        cin >> location;
        Rescue->updateRescueTeam(id, skillset, location);
    }
    void deleteRescueTeam()
    {
        int id;
        string skillset, location;
        cout << "Enter the Rescue Team ID to be deleted: ";
        cin >> id;
        Rescue->deleteRescueTeam(id);
    }
    void displayRequests()
    {
        // read the file request.txt completely
        ifstream file;
        cout << "Requests:" << endl;
        file.open("request.txt");
        string line;
        while (getline(file, line))
        {
            cout << line << endl;
        }
    }
    void showRescueTeamMenu()
    {
        while (true)
        {
            animatePrint("rescueMenu.txt");
            cout << "\nRescue Team Menu:" << endl;
            cout << "1. View Active Disasters" << endl;
            cout << "2. Priority wise Alert Calls" << endl;
            cout << "3. Logout" << endl;
            int rescueTeamChoice;

            cin >> rescueTeamChoice;
            switch (rescueTeamChoice)
            {
            case 1:
                viewActiveDisasters();
                break;
            case 2:
                alertManager->respondToAlertCalls();
                break;
            case 3:
                isLoggedIn = 0;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
            }
            if (!isLoggedIn)
                break;
        }
    }
    void showCitizenMenu(RoutingSystem &routingSystem)
    {
        animatePrint("Help.txt");
        if (!routingSystem.loadGraphFromCSV("noida_city_graph.csv", 200))
        {
            cout << "Failed to load graph data. Cannot proceed with the request." << endl;
            return;
        }
        trie.insertCsvtoTrie(&trie, "noida_city_graph.csv");

        cout << "\nCitizen Help Menu:" << endl;
        cout << "1. Request Help" << endl;
        cout << "2. View Graph" << endl;
        cout << "3. Exit" << endl;

        int citizenChoice;
        cin >> citizenChoice;

        switch (citizenChoice)
        {
        case 1:
            requestHelp(routingSystem);
            break;
        case 2:
            routingSystem.printGraph();
            break;
        case 3:
            cout << "Exiting Citizen Menu. Stay safe!" << endl;
            break;
        default:
            cout << "Invalid choice, please try again." << endl;
        }
    }

    void addDisaster()
    {
        string type, status, date, address, city, state;
        double lat, lon;
        int severity, affected_population;

        cout << "Enter Disaster Type: ";
        cin >> type;

        cout << "Enter Status: ";
        cin >> status;

        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> date;

        cout << "Enter Location (latitude longitude): ";
        cin >> lat >> lon;

        // Clear the input buffer for getline
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Address: ";
        getline(cin, address);

        cout << "Enter City: ";
        getline(cin, city);

        cout << "Enter State: ";
        getline(cin, state);

        cout << "Enter Severity (1-5): ";
        cin >> severity;

        cout << "Enter Affected Population: ";
        cin >> affected_population;

        Location loc(lat, lon, address, city, state);
        Disaster *d = new Disaster(currentDisasterId++, type, loc, severity, status, date, affected_population, {}, {});
        disasters->insert(d);
        cout << "Disaster added successfully!" << endl;
    }

    void updateDisaster()
    {
        int id;
        cout << "Enter Disaster ID: ";
        cin >> id;
        Disaster *d = disasters->search(id);
        if (d == nullptr)
        {
            cout << "Disaster not found." << endl;
            return;
        }
        cout << "Enter new Status: ";
        cin >> d->status;
        cout << "Enter new Severity: ";
        cin >> d->severity;
        cout << "Disaster updated successfully!" << endl;
    }
    void viewActiveDisasters()
    {
        cout << "\nActive Disasters:" << endl;
        Activedisasters->Display();
    }

    void requestHelp(RoutingSystem &routingSystem)
    {
        string location;
        cout << "Enter your location: ";
        getline(cin, location);
        getline(cin, location); // To handle newline issue after previous input
        vector<string> suggestions = trie.autocomplete(location);
        if (suggestions.empty())
        {
            cout << "No suggestions found for your location." << endl;
            return;
        }
        cout << "Did you mean:" << endl;
        int sugg = 1;
        for (const string &suggestion : suggestions)
        {
            cout << sugg++ << ". " << suggestion << endl;
        }
        cout << "Enter the location number from suggestions: ";
        cin >> sugg;
        location = suggestions[sugg - 1];
        vector<string> path = routingSystem.calculateOptimalPath(location, "Relief Camp");
        if (path.empty())
        {
            cout << "No path found from " << location << " to Relief Camp." << endl;
            return;
        }

        string pathString = "";
        for (size_t i = 0; i < path.size(); i++)
        {
            pathString += path[i];
            if (i < path.size() - 1)
                pathString += " -> ";
        }

        cout << "Shortest path: " << pathString << endl;

        cout << "Enter Disaster Type: ";
        string type;
        cin >> type;

        cout << "Enter Disaster Severity: ";
        int severity;
        cin >> severity;

        cout << "Enter Contact Number: ";
        string contact;
        cin >> contact;

        ofstream file("request.txt", ios::app);
        if (file.is_open())
        {
            file << "Location: " << location << ", Disaster Type: " << type
                 << ", Severity: " << severity << ", Contact No.: " << contact
                 << ", Path: " << pathString << endl;
            file.close();
        }
        cout << "Help requested successfully!" << endl;
        cout << "Will reach you out soon!!" << endl;
    }

    void viewAllDisasters()
    {
        cout << "Disasters Reported:" << endl;
        disasters->printTree();
    }

    void UpdateStatus()
    {
        if (disasters == NULL)
        {
            cout << "No disasters to assign teams to." << endl;
            return;
        }

        int disasterId;
        cout << "Enter Disaster ID to Update Severity: ";
        cin >> disasterId;
        Disaster *find = disasters->search(disasterId);
        if (find == NULL)
        {
            cout << "Invalid disaster ID." << endl;
            return;
        }

        string status;
        cout << "Enter Updated Status: ";
        cin >> status;
        find->status = status;
        cout << "Status Updated Successfully!" << endl;
    }

    void UpdateSeverity()
    {
        if (disasters == NULL)
        {
            cout << "No disasters to assign teams to." << endl;
            return;
        }
        int disasterId;
        cout << "Enter Disaster ID to Update Severity: ";
        cin >> disasterId;
        Disaster *find = disasters->search(disasterId);
        if (find == NULL)
        {
            cout << "Invalid disaster ID." << endl;
            return;
        }

        int severity;
        cout << "Enter Updated Severity: ";
        cin >> severity;
        find->severity = severity;
        cout << "Status Updated Successfully!" << endl;
    }

    void UpdateLocation(int Did, Location L)
    {
        if (disasters == NULL)
        {
            cout << "No disasters to assign teams to." << endl;
            return;
        }
        int disasterId;
        cout << "Enter Disaster ID to Update Severity: ";
        cin >> disasterId;
        Disaster *find = disasters->search(disasterId);
        if (find == NULL)
        {
            cout << "Invalid disaster ID." << endl;
            return;
        }
        double latitude, longitude;
        string address, city, state;
        cout << "Enter latitude and longitude of disaster: ";
        cin >> latitude >> longitude;
        cout << "Enter Address: ";
        cin >> address;
        cout << "Enter City: ";
        cin >> city;
        cout << "Enter State: ";
        cin >> state;
        Location loc(latitude, longitude, address, city, state);
        find->location = loc;
        cout << "Status Updated Successfully!" << endl;
    }

    void DeleteDisaster()
    {
        if (disasters == NULL)
        {
            cout << "No disasters to assign teams to." << endl;
            return;
        }
        int disasterId;
        cout << "Enter Disaster ID to Delete: ";
        cin >> disasterId;
        Disaster *find = disasters->search(disasterId);
        if (find == NULL)
        {
            cout << "Invalid disaster ID." << endl;
            return;
        }

        disasters->remove(disasterId);
        cout << "Disaster Deleted Successfully!" << endl;
    }

    void assignTeams()
    {
        if (disasters == NULL)
        {
            cout << "No disasters to assign teams to." << endl;
            return;
        }

        int disasterId;
        cout << "Enter disaster ID to assign teams: ";
        cin >> disasterId;
        Disaster *find = disasters->search(disasterId);
        if (find == NULL)
        {
            cout << "Invalid disaster ID." << endl;
            return;
        }

        int teamId;
        cout << "Enter team ID to assign: ";
        cin >> teamId;
        // Will change according to rescue team
        if (Rescue->team->Search(teamId) == NULL)
        {
            cout << "Invalid team ID." << endl;
            return;
        }

        find->assignedTeams.push_back(teamId);
        cout << "Team assigned successfully!" << endl;
    }

public:
    DisasterManagementSystem() : currentDisasterId(0), currentTeamId(1), currentShelterId(1), currentEquipmentId(1), isLoggedIn(false)
    {
        Rescue = new RescueTeamManager;
        disasters = new BPlusTree<Disaster>(5);
        Activedisasters = new HashTable<Disaster>(20);
        loadUsers();
        loadRescueUsers();
    }
    void run()
    {
        fstream file;
        file.open("Disasters.dat", ios::in | ios::binary);
        if (file)
        {
            while (true)
            {
                Disaster *D = new Disaster; // Create a new object for each read
                file.read(reinterpret_cast<char *>(D), sizeof(Disaster));

                if (file.gcount() != sizeof(Disaster))
                {             // Ensure full object was read
                    delete D; // Free memory if read is incomplete
                    break;
                }

                disasters->insert(D);
                currentDisasterId = max(currentDisasterId, D->id);
            }
        }
        currentDisasterId++;
        file.close();
        file.open("RescueTeam.dat", ios::in | ios::binary);
        if (file)
        {
            while (true)
            {
                Team *R = new Team; // Create a new object for each read
                file.read(reinterpret_cast<char *>(R), sizeof(Team));

                if (file.gcount() != sizeof(Team))
                {             // Ensure full object was read
                    delete R; // Free memory if read is incomplete
                    break;
                }

                Rescue->addRescueTeam(R);
            }
        }
        file.close();
        disasters->AddActiveDisaster(Activedisasters);
        while (true)
        {
            if (isLoggedIn == 0)
            {
                showMenu();
                int choice;
                cin >> choice;

                if (choice == 1)
                {
                    // Admin login
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;

                    if (users.find(username) != users.end() && users[username] == password)
                    {
                        isLoggedIn = 1;
                        currentUser = username;
                        cout << "Login successful!" << endl;
                        sleep(2);
                        showAdminMenu();
                    }
                    else
                    {
                        cout << "Invalid credentials. Try again." << endl;
                    }
                }
                else if (choice == 2)
                {
                    // Rescue Team login
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;

                    if (RescueUsers.find(username) != RescueUsers.end() && RescueUsers[username] == password)
                    {
                        isLoggedIn = 2;
                        currentUser = username;
                        cout << "Login successful!" << endl;
                        sleep(2);
                        showRescueTeamMenu();
                    }
                    else
                    {
                        cout << "Invalid credentials. Try again." << endl;
                    }
                }
                else if (choice == 3)
                {
                    showCitizenMenu(routingSystem);
                }
                else if (choice == 4)
                {
                    break;
                }
                else
                {
                    cout << "Invalid choice, please try again." << endl;
                }
            }
        }
        disasters->AddToFile();
        Rescue->saveTeamToFile();
        tree.saveData();
    }
};

int main()
{
    animatePrint("Logo.txt");
    animatePrint("Welcome.txt");
    KDTree tree;
    DisasterManagementSystem dms;
    dms.run();
    animatePrint("Thanks.txt");
    return 0;
}