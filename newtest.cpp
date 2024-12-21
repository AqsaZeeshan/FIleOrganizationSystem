#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// DependencyNode for linked list representation
struct DependencyNode {
    string dependencyName;
    DependencyNode* next;

    DependencyNode(const string& name) : dependencyName(name), next(nullptr) {}
};

// Updated DependenciesGraph 
class DependenciesGraph {
private:
    struct FileNode {
        string fileName;
        DependencyNode* dependenciesHead;
        FileNode* next;

        FileNode(const string& name) : fileName(name), dependenciesHead(nullptr), next(nullptr) {}
    };

    FileNode* head;

    FileNode* findFile(const string& fileName) {
        FileNode* current = head;
        while (current) {
            if (current->fileName == fileName)
                return current;
            current = current->next;
        }
        return nullptr;
    }

public:
    DependenciesGraph() : head(nullptr) {}

    void addFile(const string& fileName) {
        if (!findFile(fileName)) {
            FileNode* newNode = new FileNode(fileName);
            newNode->next = head;
            head = newNode;
        }
    }

    void addDependency(const string& fileName, const string& dependencyName) {
        FileNode* fileNode = findFile(fileName);
        if (!fileNode) {
            addFile(fileName);
            fileNode = findFile(fileName);
        }

        DependencyNode* newDep = new DependencyNode(dependencyName);
        newDep->next = fileNode->dependenciesHead;
        fileNode->dependenciesHead = newDep;
    }

    void removeDependency(const string& fileName) {
    FileNode* current = head;
    while (current) {
        DependencyNode* prev = nullptr;
        DependencyNode* depCurrent = current->dependenciesHead;

        while (depCurrent) {
            if (depCurrent->dependencyName == fileName) {
                if (prev) prev->next = depCurrent->next;
                else current->dependenciesHead = depCurrent->next;

                delete depCurrent;
                break;
            }
            prev = depCurrent;
            depCurrent = depCurrent->next;
        }
        current = current->next;
    }
}

    void markDependencyAsDeleted(const string& dependencyName) {
        FileNode* current = head;
        while (current) {
            DependencyNode* depCurrent = current->dependenciesHead;
            while (depCurrent) {
                if (depCurrent->dependencyName == dependencyName) {
                    depCurrent->dependencyName = "\033[31m" + depCurrent->dependencyName + "\033[0m"; // Mark dependency name in red
                }
                depCurrent = depCurrent->next;
            }
            current = current->next;
        }
    }

    void displayDependencies() const {
    FileNode* current = head;
    while (current) {
        cout << "File: " << current->fileName << " depends on: ";
        DependencyNode* depCurrent = current->dependenciesHead;
        while (depCurrent) {
            cout << depCurrent->dependencyName;
            if (depCurrent->next)
                cout << ", ";
            depCurrent = depCurrent->next;
        }
        cout << endl;
        current = current->next;
    }
}

    ~DependenciesGraph() {
        FileNode* current = head;
        while (current) {
            DependencyNode* depCurrent = current->dependenciesHead;
            while (depCurrent) {
                DependencyNode* temp = depCurrent;
                depCurrent = depCurrent->next;
                delete temp;
            }
            FileNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

// File structure
struct File {
    string fileName;
    string extension;
    int sizeKB;
    string category;
    string creationDate;
    string lastModifiedDate;
    bool isDeleted;

    File() : sizeKB(0), isDeleted(false) {}
};

// DynamicArray class to manage files
class DynamicArray
{
    private:
        File* files;
        int capacity;
        int size;

        void resize()
        {
            capacity = capacity * 2;
            File* newFiles = new File[capacity];
            for(int i = 0; i < size; i++)
            {
                newFiles[i] = files[i];
            }
            delete[] files;
            files = newFiles;
        }
    public:
        DynamicArray(int initialCapacity = 10)
        {
            capacity = initialCapacity;
            size = 0;
            files = new File[capacity];
        }

        void addFile(const File& file)
        {
            if(size == capacity)
            {
                resize();
            }
            files[size++] = file;
        }

        int getSize() const
        {
            return size;
        }

        void removeFile(int index) {
            if (index < 0 || index >= size) throw out_of_range("Index out of range");

            // Dependency cleanup is no longer relevant for File struct
            // Shift files to close the gap
        for (int i = index; i < size - 1; i++) {
            files[i] = files[i + 1];
        }
        size--;
}

        File& operator[](int index)
        {
            if(index < 0 || index >= size)
            {
                throw out_of_range("Index out of range");
            }
            return files[index];
        }

        const File& operator[](int index) const 
        {
            return files[index];  // Allows access but not modification
        }

        void decrementSize() 
        {
            if (size > 0) 
            {
             size--;
            }
        }

        ~DynamicArray() 
        {
            delete[] files;
        }


};

// AVL Tree Node for managing categories
struct AVLNode {
    string category;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const string& category) : category(category), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree class for managing category nodes
class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) const {
        return node ? node->height : 0;
    }

    int getBalanceFactor(AVLNode* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T = x->right;

        x->right = y;
        y->left = T;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T = y->left;

        y->left = x;
        x->right = T;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    AVLNode* insert(AVLNode* node, const string& category) {
        if (!node) return new AVLNode(category);

        if (category < node->category)
            node->left = insert(node->left, category);
        else if (category > node->category)
            node->right = insert(node->right, category);
        else
            return node;

        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

        int balance = getBalanceFactor(node);

        if (balance > 1 && category < node->left->category)
            return rotateRight(node);

        if (balance < -1 && category > node->right->category)
            return rotateLeft(node);

        if (balance > 1 && category > node->left->category) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && category < node->right->category) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    AVLNode* remove(AVLNode* root, const string& category) {
        if (!root) return root;

        if (category < root->category)
            root->left = remove(root->left, category);
        else if (category > root->category)
            root->right = remove(root->right, category);
        else {
            if (!root->left || !root->right) {
                AVLNode* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                AVLNode* temp = minValueNode(root->right);
                root->category = temp->category;
                root->right = remove(root->right, temp->category);
            }
        }

        if (!root) return root;

        root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

        int balance = getBalanceFactor(root);

        if (balance > 1 && getBalanceFactor(root->left) >= 0)
            return rotateRight(root);

        if (balance > 1 && getBalanceFactor(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        if (balance < -1 && getBalanceFactor(root->right) <= 0)
            return rotateLeft(root);

        if (balance < -1 && getBalanceFactor(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    void inorder(AVLNode* node) const {
        if (node) {
            inorder(node->left);
            cout << "Category: " << node->category << endl;
            inorder(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(const string& category) {
        root = insert(root, category);
    }

    void remove(const string& category) {
    root = remove(root, category);
    }

    void display () const {
        inorder(root);
    }
};

class HashMapReplacement {
private:
    vector<pair<string, AVLTree>> categoryMap;

    AVLTree* findCategory(const string& categoryName) {
        for (auto& pair : categoryMap) {
            if (pair.first == categoryName) {
                return &pair.second;
            }
        }
        return nullptr;
    }

public:
    void addCategory(const string& categoryName) {
        if (!findCategory(categoryName)) {
            categoryMap.push_back({categoryName, AVLTree()});
        }
    }

    AVLTree& getTree(const string& categoryName) {
    AVLTree* tree = findCategory(categoryName);
    if (!tree) {
        throw runtime_error("Category not found");
    }
    return *tree;
    }

    void displayAllCategories() {
    for (const auto& pair : categoryMap) {
        cout << "Category: " << pair.first << endl;
        pair.second.display();
        }
    }
};

class CustomStack {
private:
    vector<string> stack;
public:
    void push(const string& operation) {
        stack.push_back(operation);
    }

    void pop() {
        if (!stack.empty()) stack.pop_back();
    }

    string top() const {
        if (!stack.empty()) return stack.back();
        throw runtime_error("Stack is empty");
    }

    bool empty() const {
        return stack.empty();
    }
};



class CustomQueue {
private:
    vector<string> queue;

public:
    void enqueue(const string& value) {
        queue.push_back(value);
    }

    string front() const {
        if (queue.empty()) {
            throw runtime_error("Queue is empty");
        }
        return queue.front();
    }

    void dequeue() {
        if (queue.empty()) {
            throw runtime_error("Queue is empty");
        }
        queue.erase(queue.begin());
    }

    bool empty() const {
        return queue.empty();
    }
};



void readFromFile(const string& filepath, DynamicArray& files, DependenciesGraph& graph);
void showMenu(DynamicArray& files, DependenciesGraph& graph, HashMapReplacement& categoryMap, CustomStack& undoStack, CustomQueue& batchQueue);
void saveToFile(const DynamicArray& files, const string& filepath);
void addFile(DynamicArray& files, HashMapReplacement& categoryMap);
void deleteFile(DynamicArray& files, const string& fileName, DependenciesGraph& graph);
void moveFile(DynamicArray& files, HashMapReplacement& categoryMap);
void displayFiles(const DynamicArray& files);
void displayDependencies(const DependenciesGraph& graph);
void displayCategories(HashMapReplacement& categoryMap);
void sortFiles(DynamicArray& files, int option);
void undo(DynamicArray& files, CustomStack& undoStack);
void batchProcess(CustomQueue& batchQueue, DynamicArray& files, HashMapReplacement& categoryMap, DependenciesGraph& graph);
void merge(DynamicArray& files, int left, int mid, int right);
void mergeSort(DynamicArray& files, int left, int right); // Sort by size
void bubbleSort(DynamicArray& files); // Sort by creation date
void insertionSort(DynamicArray& files); // Sort by name
int partition(DynamicArray& files, int low, int high);
void quickSort(DynamicArray& files, int low, int high); // Sort by last modified date


void readFromFile(const string& filepath, DynamicArray& files, DependenciesGraph& graph) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filepath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        File newFile;
        string dependencies;

        getline(ss, newFile.fileName, ',');
        getline(ss, newFile.extension, ',');
        ss >> newFile.sizeKB;
        getline(ss, newFile.category, ',');
        getline(ss, newFile.creationDate, ',');
        getline(ss, newFile.lastModifiedDate, ',');
        getline(ss, dependencies, ',');

        files.addFile(newFile);
        graph.addFile(newFile.fileName);

        stringstream depStream(dependencies);
        string dep;
        while (getline(depStream, dep, ';')) {
            graph.addDependency(newFile.fileName, dep);
        }
    }

    file.close();
    cout << "File data successfully read into the system.\n";
}
void addFile(DynamicArray& files, HashMapReplacement& categoryMap) {
    // Create a new file
    File newFile;

    // Prompt user for file details
    cout << "Enter file details:\n";
    cout << "File Name: ";
    cin >> newFile.fileName;
    cout << "Extension: ";
    cin >> newFile.extension;
    cout << "Size (KB): ";
    cin >> newFile.sizeKB;
    cout << "Category: ";
    cin >> newFile.category;
    cout << "Creation Date (YYYY-MM-DD): ";
    cin >> newFile.creationDate;
    cout << "Last Modified Date (YYYY-MM-DD): ";
    cin >> newFile.lastModifiedDate;

    // Add the file to the dynamic array
    files.addFile(newFile);

    // Add the category to the category map's AVL tree
    categoryMap.getTree(newFile.category).insert(newFile.fileName);

    // Confirm the addition
    cout << "File \"" << newFile.fileName << "\" added successfully to the system.\n";
}
void displayFiles(const DynamicArray& files) {
    // Check if there are any files to display
    if (files.getSize() == 0) {
        cout << "No files available in the system.\n";
        return;
    }

    // Display all files
    cout << "\nList of Files:\n";
    cout << "=============================================================\n";
    cout << "Name         | Extension | Size (KB) | Category  | Created       | Modified\n";
    cout << "-------------------------------------------------------------\n";

    for (int i = 0; i < files.getSize(); i++) {
        const File& file = files[i];
        if (!file.isDeleted) { // Only display non-deleted files
            cout << file.fileName << " | " << file.extension
                 << " | " << file.sizeKB << " KB | " << file.category
                 << " | " << file.creationDate << " | " << file.lastModifiedDate << "\n";
        }
    }

    cout << "=============================================================\n";
}
// Function to delete a file and mark dependencies as deleted
void deleteFile(DynamicArray& files, const string& fileName, DependenciesGraph& graph) {
    bool fileFound = false;
    for (int i = 0; i < files.getSize(); ++i) {
        if (files[i].fileName == fileName && !files[i].isDeleted) {
            files[i].isDeleted = true;
            fileFound = true;

            // Update dependencies
            graph.removeDependency(fileName);

            cout << "File " << fileName << " marked as deleted.\n";
            break;
        }
    }

    if (!fileFound) {
        cout << "File " << fileName << " not found or already deleted.\n";
    }
}


void displayDependencies(const DependenciesGraph& graph) {
    cout << "\nDisplaying File Dependencies:\n";
    cout << "========================================\n";
    graph.displayDependencies();
    cout << "========================================\n";
}
void saveToFile(const DynamicArray& files, const string& filepath) {
    ofstream outFile(filepath);

    // Check if the file is open
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file \"" << filepath << "\" for writing.\n";
        return;
    }

    // Write the header for the CSV file
    outFile << "FileName,Extension,SizeKB,Category,CreationDate,LastModifiedDate\n";

    // Write each file's details to the file
    for (int i = 0; i < files.getSize(); i++) {
        const File& file = files[i];

        // Only save non-deleted files
        if (!file.isDeleted) {
            outFile << file.fileName << ","
                    << file.extension << ","
                    << file.sizeKB << ","
                    << file.category << ","
                    << file.creationDate << ","
                    << file.lastModifiedDate << "\n";
        }
    }

    outFile.close(); // Close the file after writing
    cout << "File data successfully saved to \"" << filepath << "\".\n";
}

void moveFile(DynamicArray& files, HashMapReplacement& categoryMap) {
    string fileName, newCategory, oldCategory;
    cout << "Enter file name to move: ";
    cin >> fileName;
    cout << "Enter new category: ";
    cin >> newCategory;

    bool fileFound = false;
    for (int i = 0; i < files.getSize(); i++) {
        if (files[i].fileName == fileName) {
            oldCategory = files[i].category;
            files[i].category = newCategory;

            // Update category map
            categoryMap.getTree(oldCategory).remove(fileName); // Ensure remove() exists
            categoryMap.getTree(newCategory).insert(fileName);

            fileFound = true;
            break;
        }
    }

    if (!fileFound) {
        cout << "File not found!\n";
    }
}


void undo(DynamicArray& files, CustomStack& undoStack) {
    if (undoStack.empty()) {
        cout << "Undo stack is empty! No operation to undo.\n";
        return;
    }

    string lastOperation = undoStack.top();
    undoStack.pop();

    if (lastOperation == "add") {
        // Implement logic to undo the last added file
        cout << "Undoing last add operation...\n";
        // Example: files.removeLast(); (Implement removeLast in DynamicArray if needed)
    } else if (lastOperation == "delete") {
        // Implement logic to restore the last deleted file
        cout << "Undoing last delete operation...\n";
        // Example: files.restoreLastDeleted(); (Implement restoreLastDeleted in DynamicArray if needed)
    } else {
        cout << "Unknown operation to undo.\n";
    }
}

void batchProcess(CustomQueue& batchQueue, DynamicArray& files, HashMapReplacement& categoryMap, DependenciesGraph& graph) {
    while (!batchQueue.empty()) {
        string operation = batchQueue.front();
        batchQueue.dequeue();

        stringstream ss(operation);
        string operationType, fileName, category;
        getline(ss, operationType, ',');

        if (operationType == "ADD") {
            // Add file logic
            addFile(files, categoryMap);
        } else if (operationType == "DELETE") {
            cout << "Enter file name to delete: ";
            cin >> fileName;
            deleteFile(files, fileName, graph);
        } else if (operationType == "MOVE") {
            moveFile(files, categoryMap);
        } else {
            cout << "Unknown operation type in batch queue.\n";
        }
    }
}



void merge(DynamicArray& files, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<File> leftArray(n1);
    vector<File> rightArray(n2);

    for (int i = 0; i < n1; i++)
        leftArray[i] = files[left + i];
    for (int i = 0; i < n2; i++)
        rightArray[i] = files[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArray[i].sizeKB <= rightArray[j].sizeKB) {
            files[k++] = leftArray[i++];
        } else {
            files[k++] = rightArray[j++];
        }
    }

    while (i < n1) files[k++] = leftArray[i++];
    while (j < n2) files[k++] = rightArray[j++];
}

void mergeSort(DynamicArray& files, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(files, left, mid);
        mergeSort(files, mid + 1, right);
        merge(files, left, mid, right);
    }
}
void bubbleSort(DynamicArray& files) {
    int n = files.getSize();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (files[j].sizeKB > files[j + 1].sizeKB) {
                swap(files[j], files[j + 1]); // Use custom swap
            }
        }
    }
}

void swap(File& a, File& b) {
    File temp = a;
    a = b;
    b = temp;
}

void insertionSort(DynamicArray& files) {
    int n = files.getSize();
    for (int i = 1; i < n; i++) {
        File key = files[i];
        int j = i - 1;

        while (j >= 0 && files[j].fileName > key.fileName) {
            files[j + 1] = files[j];
            j--;
        }
        files[j + 1] = key;
    }
}

int partition(DynamicArray& files, int low, int high) {
    File pivot = files[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (files[j].fileName < pivot.fileName) {
            ++i;
            swap(files[i], files[j]); // Use custom swap
        }
    }
    swap(files[i + 1], files[high]); // Use custom swap
    return i + 1;
}


void quickSort(DynamicArray& files, int low, int high) {
    if (low < high) {
        int pi = partition(files, low, high);
        quickSort(files, low, pi - 1);
        quickSort(files, pi + 1, high);
    }
}
void sortFiles(DynamicArray& files, int option) {
    cout << "Sorting files...\n";

    switch (option) {
        case 1: // Sort by Name
            insertionSort(files);
            cout << "Files sorted by name.\n";
            break;
        case 2: // Sort by Size
            mergeSort(files, 0, files.getSize() - 1);
            cout << "Files sorted by size.\n";
            break;
        case 3: // Sort by Creation Date
            bubbleSort(files);
            cout << "Files sorted by creation date.\n";
            break;
        case 4: // Sort by Last Modified Date
            quickSort(files, 0, files.getSize() - 1);
            cout << "Files sorted by last modified date.\n";
            break;
        default:
            cout << "Invalid sorting option. Please choose a valid option.\n";
            return;
    }

    // Display sorted files
    displayFiles(files);
}
void displayCategories(HashMapReplacement& categoryMap) {
    cout << "\nDisplaying All Categories:\n";
    cout << "========================================\n";

    categoryMap.displayAllCategories();

    cout << "========================================\n";
}

void showMenu(DynamicArray& files, DependenciesGraph& graph, HashMapReplacement& categoryMap, CustomStack& undoStack, CustomQueue& batchQueue) {
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Add a file\n";
        cout << "2. Delete a file\n";
        cout << "3. Display files\n";
        cout << "4. Sort files\n";
        cout << "5. Undo last operation\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                addFile(files, categoryMap);
                undoStack.push("add");
                break;
            }
            case 2: {
                cout << "Enter the name of the file to delete: ";
                string fileName;
                cin >> fileName;
                deleteFile(files, fileName, graph);
                undoStack.push("delete");
                break;
            }
            case 3: {
                displayFiles(files);
                break;
            }
            case 4: {
                int sortOption;
                cout << "Sort by:\n1. Name\n2. Size\n3. Creation Date\n4. Last Modified Date\n";
                cout << "Enter your choice: ";
                cin >> sortOption;
                sortFiles(files, sortOption);
                break;
            }
            case 5: {
                undo(files, undoStack);
                break;
            }
            case 0:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

int main() {
    string filePath = "MockDataSet.txt";

    // Core components
    DynamicArray files;
    DependenciesGraph graph;
    HashMapReplacement categoryMap;
    CustomStack undoStack;
    CustomQueue batchQueue;

    // Load data
    readFromFile(filePath, files, graph);

    // Launch menu
    showMenu(files, graph, categoryMap, undoStack, batchQueue);

    // Save data
    saveToFile(files, filePath);

    return 0;
}