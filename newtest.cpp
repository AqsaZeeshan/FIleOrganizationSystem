#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
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

    void displayDependencies() {
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
class DynamicArray {
private:
    File* files;
    int capacity;
    int size;

    void resize() {
        capacity *= 2;
        File* newFiles = new File[capacity];
        for (int i = 0; i < size; i++) {
            newFiles[i] = files[i];
        }
        delete[] files;
        files = newFiles;
    }

public:
    DynamicArray(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
        files = new File[capacity];
    }

    void addFile(const File& file) {
        if (size == capacity)
            resize();
        files[size++] = file;
    }

    int getSize() const {
        return size;
    }

    File& operator[](int index) {
        if (index < 0 || index >= size)
            throw out_of_range("Index out of range");
        return files[index];
    }

    ~DynamicArray() {
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

    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalanceFactor(AVLNode* node) {
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

    void inorder(AVLNode* node) {
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

    void display() {
        inorder(root);
    }
};

void readFromFile(const string& filepath, DynamicArray& files, DependenciesGraph& graph);
void showMenu(DynamicArray& files, DependenciesGraph& graph, AVLTree& categoryTree);
void saveToFile(const DynamicArray& files, const string& filepath);
void addFile(DynamicArray& files, AVLTree& categoryTree);
void deleteFile(DynamicArray& files, const string& fileName, DependenciesGraph& graph);
void moveFile(DynamicArray& files, AVLTree& categoryTree);
void displayFiles(const DynamicArray& files);
void displayDependencies(const DependenciesGraph& graph);
void displayCategories(const AVLTree& categoryTree);
void sortFiles(DynamicArray& files, int option);
void undo(DynamicArray& files, stack<string>& undoStack);
void batchProcess(queue<string>& batchQueue, DynamicArray& files, AVLTree& categoryTree);
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
void addFile(DynamicArray& files, HashMap& categoryMap) {
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
    categoryMap.getTree(newFile.category).insert(newFile.category);

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

    // Search for the file by name in the dynamic array
    for (int i = 0; i < files.getSize(); i++) {
        File& file = files[i];

        // Check if the file matches the given name
        if (file.fileName == fileName && !file.isDeleted) {
            file.isDeleted = true; // Mark the file as deleted
            fileFound = true;

            // Mark this file as deleted in the dependency graph
            graph.markDependencyAsDeleted(fileName);

            cout << "File \"" << fileName << "\" has been deleted successfully.\n";
            break;
        }
    }

    // If the file was not found, print a message
    if (!fileFound) {
        cout << "File \"" << fileName << "\" not found or already deleted.\n";
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
void moveFile(DynamicArray& files, HashMap& categoryMap) {
    string fileName, newCategory;
    bool fileFound = false;

    // Prompt user for file name
    cout << "Enter the name of the file to move: ";
    cin >> fileName;

    // Search for the file in the dynamic array
    for (int i = 0; i < files.getSize(); i++) {
        File& file = files[i];

        if (file.fileName == fileName && !file.isDeleted) {
            fileFound = true;

            // Display current category
            cout << "The file \"" << file.fileName << "\" is currently in the category: " << file.category << "\n";

            // Prompt user for new category
            cout << "Enter the new category to move the file to: ";
            cin >> newCategory;

            // Update the category in the file
            string oldCategory = file.category;
            file.category = newCategory;

            // Update the AVL trees in the HashMap
            categoryMap.getTree(oldCategory).insert(fileName); // Remove from old tree
            categoryMap.getTree(newCategory).insert(fileName); // Add to new tree

            // Confirm the move
            cout << "File \"" << file.fileName << "\" moved to category \"" << newCategory << "\" successfully.\n";

            // Optionally push to undo stack
            // undoStack.push("MOVE," + fileName + "," + oldCategory + "," + newCategory);
            break;
        }
    }

    // If the file is not found
    if (!fileFound) {
        cout << "File \"" << fileName << "\" not found or already deleted.\n";
    }
}
void undo(DynamicArray& files, stack<string>& undoStack) {
    if (undoStack.empty()) {
        cout << "No operations to undo.\n";
        return;
    }

    string lastOperation = undoStack.top();
    undoStack.pop();

    stringstream ss(lastOperation);
    string operationType, fileName, oldCategory, newCategory;
    getline(ss, operationType, ',');

    if (operationType == "ADD") {
        getline(ss, fileName, ',');
        for (int i = 0; i < files.getSize(); i++) {
            if (files[i].fileName == fileName && !files[i].isDeleted) {
                files[i].isDeleted = true;
                cout << "Undo ADD: File \"" << fileName << "\" removed.\n";
                return;
            }
        }
    } else if (operationType == "DELETE") {
        File restoredFile;
        getline(ss, restoredFile.fileName, ',');
        getline(ss, restoredFile.extension, ',');
        ss >> restoredFile.sizeKB;
        getline(ss, restoredFile.category, ',');
        getline(ss, restoredFile.creationDate, ',');
        getline(ss, restoredFile.lastModifiedDate, ',');

        files.addFile(restoredFile);
        cout << "Undo DELETE: File \"" << restoredFile.fileName << "\" restored.\n";
    } else if (operationType == "MOVE") {
        getline(ss, fileName, ',');
        getline(ss, oldCategory, ',');
        getline(ss, newCategory, ',');

        for (int i = 0; i < files.getSize(); i++) {
            if (files[i].fileName == fileName && !files[i].isDeleted) {
                files[i].category = oldCategory;
                cout << "Undo MOVE: File \"" << fileName << "\" moved back to category \"" << oldCategory << "\".\n";
                return;
            }
        }
    } else {
        cout << "Unknown operation type in undo stack.\n";
    }
}
void batchProcess(queue<string>& batchQueue, DynamicArray& files, HashMap& categoryMap) {
    while (!batchQueue.empty()) {
        string operation = batchQueue.front();
        batchQueue.pop();

        stringstream ss(operation);
        string operationType, fileName, category;
        getline(ss, operationType, ',');

        if (operationType == "ADD") {
            File newFile;
            getline(ss, newFile.fileName, ',');
            getline(ss, newFile.extension, ',');
            ss >> newFile.sizeKB;
            getline(ss, newFile.category, ',');
            getline(ss, newFile.creationDate, ',');
            getline(ss, newFile.lastModifiedDate, ',');

            files.addFile(newFile);
            categoryMap.getTree(newFile.category).insert(newFile.fileName);
            cout << "Batch Process: Added file \"" << newFile.fileName << "\".\n";
        } else if (operationType == "DELETE") {
            getline(ss, fileName, ',');
            bool fileFound = false;
            for (int i = 0; i < files.getSize(); i++) {
                if (files[i].fileName == fileName && !files[i].isDeleted) {
                    files[i].isDeleted = true;
                    fileFound = true;
                    cout << "Batch Process: Deleted file \"" << fileName << "\".\n";
                    break;
                }
            }
            if (!fileFound) {
                cout << "Batch Process: File \"" << fileName << "\" not found.\n";
            }
        } else if (operationType == "MOVE") {
            getline(ss, fileName, ',');
            getline(ss, category, ',');

            bool fileFound = false;
            for (int i = 0; i < files.getSize(); i++) {
                if (files[i].fileName == fileName && !files[i].isDeleted) {
                    files[i].category = category;
                    fileFound = true;
                    cout << "Batch Process: Moved file \"" << fileName << "\" to category \"" << category << "\".\n";
                    break;
                }
            }
            if (!fileFound) {
                cout << "Batch Process: File \"" << fileName << "\" not found.\n";
            }
        } else {
            cout << "Unknown operation type in batch queue.\n";
        }
    }

    cout << "Batch processing completed.\n";
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
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (files[j].creationDate > files[j + 1].creationDate) {
                swap(files[j], files[j + 1]);
            }
        }
    }
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
    string pivot = files[high].lastModifiedDate;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (files[j].lastModifiedDate <= pivot) {
            i++;
            swap(files[i], files[j]);
        }
    }
    swap(files[i + 1], files[high]);
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
void displayCategories(HashMap& categoryMap) {
    cout << "\nDisplaying All Categories:\n";
    cout << "========================================\n";

    categoryMap.displayAllCategories();

    cout << "========================================\n";
}
void showMenu(DynamicArray& files, DependenciesGraph& graph, HashMap& categoryMap) {
    int choice;

    do {
        cout << "\n========================================\n";
        cout << "            File Management Menu         \n";
        cout << "========================================\n";
        cout << "1. Display All Files\n";
        cout << "2. Add a New File\n";
        cout << "3. Delete a File\n";
        cout << "4. Move a File to Another Category\n";
        cout << "5. Display File Dependencies\n";
        cout << "6. Display All Categories\n";
        cout << "7. Sort Files\n";
        cout << "8. Undo Last Operation\n";
        cout << "9. Batch Process Operations\n";
        cout << "10. Save Files to Disk\n";
        cout << "11. Exit\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: // Display all files
                displayFiles(files);
                break;

            case 2: // Add a new file
                addFile(files, categoryMap);
                break;

            case 3: // Delete a file
            {
                string fileName;
                cout << "Enter the name of the file to delete: ";
                cin >> fileName;
                deleteFile(files, fileName, graph);
                break;
            }

            case 4: // Move a file to another category
                moveFile(files, categoryMap);
                break;

            case 5: // Display file dependencies
                displayDependencies(graph);
                break;

            case 6: // Display all categories
                displayCategories(categoryMap);
                break;

            case 7: // Sort files
            {
                int sortOption;
                cout << "Choose sorting option:\n";
                cout << "1. Sort by Name\n";
                cout << "2. Sort by Size\n";
                cout << "3. Sort by Creation Date\n";
                cout << "4. Sort by Last Modified Date\n";
                cout << "Enter your choice: ";
                cin >> sortOption;
                sortFiles(files, sortOption);
                break;
            }

            case 8: // Undo last operation
                undo(files, undoStack);
                break;

            case 9: // Batch process operations
                batchProcess(batchQueue, files, categoryMap);
                break;

            case 10: // Save files to disk
                saveToFile(files, "MockDataSet.txt");
                break;

            case 11: // Exit
                cout << "Exiting program. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 11);
}
int main() {
    string filePath = "MockDataSet.txt";

    // Initialize core components
    DynamicArray files;
    DependenciesGraph graph;
    HashMap categoryMap;

    // Read files and populate data structures
    readFromFile(filePath, files, graph, categoryMap);

    // Display the number of files loaded
    cout << "Number of files read: " << files.getSize() << endl;

    // Launch the menu for user interaction
    showMenu(files, graph, categoryMap);

    // Save files to disk upon exiting the menu
    saveToFile(files, filePath);

    cout << "Program exited successfully.\n";

    return 0;
}