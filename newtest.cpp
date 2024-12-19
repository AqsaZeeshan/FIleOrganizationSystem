//created this to edit fucntions and test codes, while keeping the initial code in test.cpp
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <queue>

using namespace std;

struct DependencyNode 
{
    string dependencyName;  // Name of the dependency
    DependencyNode* next;   // Pointer to the next node

    DependencyNode(const string& name) : dependencyName(name), next(nullptr) {}
};

struct File 
{
    string fileName;
    string extension;
    int sizeKB;
    string category;
    string creationDate;
    string lastModifiedDate;
    DependencyNode* dependenciesHead;
    bool isDeleted;

    File() : dependenciesHead(nullptr), isDeleted(false) {} 
};

// forward declerations

void sortFiles(class DynamicArray& files, vector<vector<int> >& dependencyGraph, vector<string>& fileNames);  
void mergeSort(class DynamicArray& files, int left, int right);
void bubbleSort(class DynamicArray& files);
void insertionSort(class DynamicArray& files);
void quickSort(class DynamicArray& files, int low, int high);
void searchFiles(const DynamicArray& files, const vector<string>& fileNames, vector<vector<int> >& dependencyGraph);
void undo(class DynamicArray& files);

void displayFiles(const DynamicArray& files, vector<vector<int> >& dependencyGraph, vector<string>& fileNames);
void addFile(DynamicArray& files,vector<vector<int> >& dependencyGraph, vector<string>& fileNames);
void deleteFile(DynamicArray& files, vector<vector<int> >& dependencyGraph, vector<string>& fileNames);
void moveFile(class DynamicArray& files);
void searchAllFile(const DynamicArray& files, const vector<string>& fileNames, const string& searchName, vector<vector<int> >& dependencyGraph);
void displayByCategory();
void displayDependencies(const DynamicArray& files, const vector<vector<int> >& dependencyGraph, const vector<string>& fileNames);
void addDependency(DependencyNode*& head, const string& dependencyName);

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

        void removeFile(int index) 
        {
            if (index < 0 || index >= size) {
                throw out_of_range("Index out of range");
            }

            // Clean up dependencies for the file being removed
            DependencyNode* current = files[index].dependenciesHead;
            while (current != nullptr) {
                DependencyNode* toDelete = current;
                current = current->next;
                delete toDelete;
            }

            // Shift all files after the removed one to fill the gap
            for (int i = index; i < size - 1; i++) {
                files[i] = files[i + 1];
            }

            // Decrement the size
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
            for (int i = 0; i < size; i++) 
            {
                DependencyNode* current = files[i].dependenciesHead;
                while (current != nullptr) {
                    DependencyNode* toDelete = current;
                    current = current->next;
                    delete toDelete;
                }
            }
            delete[] files;
        }


};

// AVL Tree Node
struct AVLNode {
    File* file;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(File* file) : file(file), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree Class
class AVLTree {
public:
    AVLNode* root;

    AVLTree() : root(nullptr) {}

    // Utility functions for AVL tree balancing
    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(AVLNode* node) {
        return height(node->left) - height(node->right);
    }

    void updateHeight(AVLNode* node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    AVLNode* rotateRight(AVLNode* node) {
        AVLNode* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    AVLNode* rotateLeft(AVLNode* node) {
        AVLNode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    AVLNode* balance(AVLNode* node) {
        updateHeight(node);
        if (balanceFactor(node) > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if (balanceFactor(node) < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
        return node;
    }

    // Insert a file into the AVL tree
    AVLNode* insert(AVLNode* node, File* file) {
        if (!node) return new AVLNode(file);
        if (file->fileName < node->file->fileName) {
            node->left = insert(node->left, file);
        } else {
            node->right = insert(node->right, file);
        }
        return balance(node);
    }

    // Delete a file from the AVL tree
    AVLNode* remove(AVLNode* node, File* file) {
        if (!node) return node;
        if (file->fileName < node->file->fileName) {
            node->left = remove(node->left, file);
        } else if (file->fileName > node->file->fileName) {
            node->right = remove(node->right, file);
        } else {
            if (!node->left) {
                AVLNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                AVLNode* temp = node->left;
                delete node;
                return temp;
            }
            AVLNode* temp = getMin(node->right);
            node->file = temp->file;
            node->right = remove(node->right, temp->file);
        }
        return balance(node);
    }

    AVLNode* getMin(AVLNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    // Insert a file into the tree
    void insertFile(File* file) {
        root = insert(root, file);
    }

    // Remove a file from the tree
    void removeFile(File* file) {
        root = remove(root, file);
    }

    void inorderTraversal(AVLNode* node) {
        if (!node) return;
        inorderTraversal(node->left);
        cout << "File: " << node->file->fileName << " | Category: " << node->file->category << endl;
        inorderTraversal(node->right);
    }

    void displayFiles() {
        inorderTraversal(root);
    }
};

class Stack 
{
private:
    static const int MAX_SIZE = 100; 
    string data[MAX_SIZE];      
    int topIndex;                 

public:

    Stack() : topIndex(-1) {}


    void push(const string& value) 
    {
        if (topIndex == MAX_SIZE - 1) {
            throw overflow_error("Stack overflow: Cannot push more elements.");
        }
        data[++topIndex] = value;
    }

    void pop() 
    {
        if (topIndex == -1) {
            throw underflow_error("Stack underflow: Cannot pop from an empty stack.");
        }
        --topIndex;
    }

    string top() const 
    {
        if (topIndex == -1) {
            throw underflow_error("Stack is empty: No top element.");
        }
        return data[topIndex];
    }


    bool isEmpty() const 
    {
        return topIndex == -1;
    }


    int size() const 
    {
        return topIndex + 1;
    }
};

class Queue 
{
private:
    static const int MAX_SIZE = 100; 
    string data[MAX_SIZE];   
    int frontIndex;               
    int rearIndex;                   
    int currentSize;                 

public:

    Queue() : frontIndex(0), rearIndex(-1), currentSize(0) {}

    void enqueue(const string& value) {
        if (currentSize == MAX_SIZE) {
            throw overflow_error("Queue overflow: Cannot enqueue more elements.");
        }
        rearIndex = (rearIndex + 1) % MAX_SIZE;
        data[rearIndex] = value;
        currentSize++;
    }

    void dequeue() 
    {
        if (isEmpty()) {
            throw underflow_error("Queue underflow: Cannot dequeue from an empty queue.");
        }
        frontIndex = (frontIndex + 1) % MAX_SIZE;
        currentSize--;
    }

    string front() const 
    {
        if (isEmpty()) {
            throw underflow_error("Queue is empty: No front element.");
        }
        return data[frontIndex];
    }

    bool isEmpty() const 
    {
        return currentSize == 0;
    }

    int size() const 
    {
        return currentSize;
    }
};

// Global hash map to store files in different categories
unordered_map<string, AVLTree> categoryTreeMap;

// Undo stack and batch queue
Stack undoStack;
Queue batchQueue;

void addDependency(DependencyNode*& head, const string& dependencyName) 
{
    DependencyNode* newNode = new DependencyNode(dependencyName);
    newNode->next = head;
    head = newNode;
}

void readFromFile(DynamicArray& files, const string& filepath, vector<vector<int> >& dependencyGraph, vector<string>& fileNames) 
{
    ifstream file(filepath); // Open the file
    if (!file.is_open()) 
    {
        cerr<< "Error: Unable to open file " << filepath << endl;
        return;
    }

    string line; 
    getline(file, line); // Skip the header line

    while (getline(file, line)) 
    {
        stringstream ss(line); // this converts the current line variable to to a stringstream.
        File fileEntry; // temp file
        string sizeStr, dependenciesStr;

        // Read values from the line
        getline(ss, fileEntry.fileName, ',');
        getline(ss, fileEntry.extension, ',');
        getline(ss, sizeStr, ',');
        fileEntry.sizeKB = stoi(sizeStr);
        getline(ss, fileEntry.category, ',');
        getline(ss, fileEntry.creationDate, ',');
        getline(ss, fileEntry.lastModifiedDate, ',');
        getline(ss, dependenciesStr, ',');

        // Parse dependencies if any
        stringstream depStream(dependenciesStr);
        string dep;
        while (getline(depStream, dep, ';')) { // Assuming dependencies are separated by semicolons
            addDependency(fileEntry.dependenciesHead, dep);
            int depIndex = find(fileNames.begin(), fileNames.end(), dep) - fileNames.begin(); 
            if (depIndex == fileNames.size()) 
            { 
                fileNames.push_back(dep); 
                dependencyGraph.push_back(vector<int>()); 
            } 
            dependencyGraph[depIndex].push_back(files.getSize()); 
        }       

        // Add the File object to the DynamicArray
        files.addFile(fileEntry);
        fileNames.push_back(fileEntry.fileName); 
        dependencyGraph.push_back(vector<int>()); 
    }    

    file.close(); // Close the file
    cout << "File data successfully read into the DynamicArray.\n";
}

void showMenu(DynamicArray& files,vector<vector<int> >& dependencyGraph, vector<string>& fileNames) 
{
    int choice;
    do {
        cout << "==========================================" << endl;
        cout << "         File Organization System         " <<endl;
        cout << "==========================================" <<endl;
        cout << "1. View All Files" <<endl;//done
        cout << "2. Add a New File" <<endl;//done
        cout << "3. Delete a File" << endl;// should be done
        cout << "4. Move a File to a Different Category" <<endl;// should be done
        cout << "5. Sort Files" << endl;//done
        cout << "     a) By Name" << endl;
        cout << "     b) By Size" << endl;
        cout << "     c) By Creation Date" << endl;
        cout << "     d) By Last Modified Date" << endl;
        cout << "6. Search Files" <<endl;//done
        cout << "     a) By Name" << endl;
        cout << "7. Display Files by Category" << endl; // should be done
        cout << "8. Display File Dependencies (Graph View)" << endl;
        cout << "9. Undo Last Operation.  "<<endl;
        cout << "10. Exit" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                displayFiles(files, dependencyGraph, fileNames);
                break;
            case 2:
                addFile(files, dependencyGraph, fileNames);
                break;
            case 3:
                deleteFile(files, dependencyGraph, fileNames);
                break;
            case 4:
                moveFile(files);
                break;
            case 5:
                sortFiles(files, dependencyGraph, fileNames);
                break;
            case 6:
                searchFiles(files,fileNames, dependencyGraph);
                break;
            case 7:
                //displayByCategory();
                break;
            case 8:
                displayDependencies(files, dependencyGraph, fileNames);
                break;
            case 9: 
                undo(files);
                break;
            case 10:
                cout << "Exiting File Organization System..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 10);
}

void deleteFile(DynamicArray& files, vector<vector<int> >& dependencyGraph, vector<string>& fileNames) {
    string fileName;
    cout << "Enter the name of the file to delete: ";
    cin >> fileName;
    
    bool fileFound = false;
    for (int i = 0; i < files.getSize(); i++) {
        if (  fileName == files[i].fileName)  {
            fileFound = true;
            int fileIndex = find(fileNames.begin(), fileNames.end(), fileName) - fileNames.begin();
            if (!dependencyGraph[fileIndex].empty()) {
                cout << "Error: The file is a dependency for other files. Do you still want to delete it? (yes/no): ";
                string choice;
                cin >> choice;
                if (choice == "no") {
                    return;
                } 
                
            files[i].isDeleted = true;
           for (int dependentIndex : dependencyGraph[fileIndex]) { 
                DependencyNode* current = files[dependentIndex].dependenciesHead; 
                while (current != nullptr) {
                    if (current->dependencyName == fileName) {
                        current->dependencyName = "\033[31m" + current->dependencyName + "\033[0m"; // Color the dependency name red
                    }
                    current = current->next;
                }
            }
            string dependenciesStr = "";
            DependencyNode* dep = files[i].dependenciesHead;
            while (dep != nullptr) 
            {
                dependenciesStr += dep->dependencyName;
                if (dep->next != nullptr) 
                {
                    dependenciesStr += "|";
                }
                dep = dep->next;
            }
            // delete for undo
             string operation = "DELETE," + files[i].fileName + "," +
                       files[i].extension + "," +
                       files[i].category + "," +
                       to_string(files[i].sizeKB) + "," +
                       files[i].creationDate + "," +
                       files[i].lastModifiedDate + "," +
                       dependenciesStr;
                        undoStack.push(operation);
            // Remove the file from the DynamicArray
            for (int k = i; k < files.getSize() - 1; k++) {
                files[k] = files[k + 1];
            }
            files.decrementSize(); // Decrease the size of the array
            break;
        }
    }

    if (!fileFound) 
    {
        cout << "Error: File not found." << endl;
    }
}
}



void displayDependencies(const DynamicArray& files, const vector<vector<int> >& dependencyGraph, const vector<string>& fileNames) 
{
    cout << "==========================================" << endl;
    cout << "         File Dependencies                " << endl;
    cout << "==========================================" << endl;

    for (int i = 0; i < files.getSize(); i++) {
        const File& file = files[i];
        cout << "File Name: " << file.fileName << endl;
        cout << "Dependencies: ";
        DependencyNode* head = file.dependenciesHead;
        while (head != nullptr) {
            cout << head->dependencyName;
            if (head->next != nullptr) {
                cout << " -> ";
            }
            head = head->next;
        }
        cout << endl;
        cout << "------------------------------------------" << endl;
    }

    cout << "==========================================" << endl;
}
void searchAllFile(const DynamicArray& files, const vector<string>& fileNames, const string& searchName, vector<vector<int> >& dependencyGraph) {
    bool found = false;
    for (int i = 0; i < files.getSize(); i++) {
        
        if (files[i].fileName == searchName) {
            found = true;
            // Display file details
            cout << "==========================================" << endl;
            cout << "             File Details                 " << endl;
            cout << "==========================================" << endl;
            cout << "File Name: " << files[i].fileName;
            cout     << ", Extension: " << files[i].extension
                 << ", Size: " << files[i].sizeKB << " KB"
                 << ", Category: " << files[i].category
                 << ", Creation Date: " << files[i].creationDate
                 << ", Last Modified Date: " << files[i].lastModifiedDate;

            // Display dependencies
            cout << ", Dependencies: ";
            DependencyNode* head = files[i].dependenciesHead;
            if (head == nullptr) {
                cout << "No dependencies";
            } else {
                while (head != nullptr) {
                    cout << head->dependencyName;
                    if (head->next != nullptr) {
                        cout << " -> ";
                    }
                    head = head->next;
                }
            }
            cout << endl;
            cout << "------------------------------------------" << endl;
        }
    }

    if (!found) {
        cout << "File with the name '" << searchName << "' not found." << endl;
    }
}

void searchFiles(const DynamicArray& files, const vector<string>& fileNames, vector<vector<int> >& dependencyGraph)
{
    int ch;
    string name;
    cout<<"Enter file name to search: ";cin>> name;
    cout<<"     1. Search all files."<< endl;
    cout<<"     2. Search Category."<< endl;
    cin>>ch;
    switch(ch)
    {
        case 1:
            searchAllFile(files, fileNames, name, dependencyGraph);
            break;
        case 2:
            int ch2;
            cout<<"Enter category you wish to search: ";
            cout<<"     1. Documents."<< endl;
            cout<<"     2. Desktop."<< endl;
            cout<<"     3. Media."<< endl;
            cout<<"     4. Programming."<< endl;
            cin>>ch2;
            switch(ch)
            {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
            }
            break;           
    }

}
void displayFiles(const DynamicArray& files, vector<vector<int> >& dependencyGraph, vector<string>& fileNames) {
    cout << "==========================================" << endl;
    cout << "             File Details                 " << endl;
    cout << "==========================================" << endl;

    for (int i = 0; i < files.getSize(); i++) {
        const File& file = files[i];
        cout << "File Name: " << file.fileName 
             << ", Extension: " << file.extension 
             << ", Size: " << file.sizeKB << " KB" 
             << ", Category: " << file.category 
             << ", Creation Date: " << file.creationDate 
             << ", Last Modified Date: " << file.lastModifiedDate;
    }

    cout << ", Dependencies: "; displayDependencies(files, dependencyGraph, fileNames);
    cout<<endl; 

    cout << "==========================================" << endl;
}


void sortFiles(DynamicArray& files, vector<vector<int> >& dependencyGraph, vector<string>& fileNames)
{
    int sortChoice;
    cout << "==========================================" << endl;
    cout << "            Sorting Options               " << endl;
    cout << "==========================================" << endl;
    cout << "1. Sort by Name (Merge Sort)" << endl;
    cout << "2. Sort by Size (Bubble Sort)" << endl;
    cout << "3. Sort by Creation Date (Insertion Sort)" << endl;
    cout << "4. Sort by Last Modified Date (Quick Sort)" << endl;
    cout << "==========================================" << endl;
    cout << "Enter your choice: ";
    cin >> sortChoice;
    switch (sortChoice) 
    {
        case 1:
            mergeSort(files, 0, files.getSize() - 1); 
            cout << "Files sorted by Name.\n";
            displayFiles(files, dependencyGraph, fileNames); 
            break;
        case 2:
            bubbleSort(files); 
            cout << "Files sorted by Size.\n";
            displayFiles(files, dependencyGraph, fileNames); 
            break;
        case 3:
            insertionSort(files); 
            cout << "Files sorted by Creation Date.\n";
            displayFiles(files, dependencyGraph, fileNames); 
            break;
        case 4:
            quickSort(files, 0, files.getSize() - 1); 
            cout << "Files sorted by Last Modified Date.\n";
            displayFiles(files, dependencyGraph, fileNames); 
            break;
        default:
            cout << "Invalid choice. Returning to main menu.\n";
    }
}

// sort files by namwe
void merge(DynamicArray& files, int left, int mid, int right) 
{
    int tempSize = right - left + 1;
    File* temp = new File[tempSize]; 
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (files[i].fileName <= files[j].fileName) {
            temp[k++] = files[i++];
        } else {
            temp[k++] = files[j++];
        }
    }

    while (i <= mid) temp[k++] = files[i++];
    while (j <= right) temp[k++] = files[j++];

    for (int l = 0; l < tempSize; l++) {
        files[left + l] = temp[l];
    }

    delete[] temp;  
}

void mergeSort(DynamicArray& files, int left, int right) 
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(files, left, mid);
        mergeSort(files, mid + 1, right);
        merge(files, left, mid, right);
    }
}


// sorting based on size
void bubbleSort(DynamicArray& files) 
{
    int n = files.getSize();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (files[j].sizeKB < files[j + 1].sizeKB) 
            {
                swap(files[j], files[j + 1]);
            }
        }
    }
}

// sorting based on creation date
void insertionSort(DynamicArray& files) {
    for (int i = 1; i < files.getSize(); i++) {
        File key = files[i];
        int j = i - 1;

        while (j >= 0 && files[j].creationDate > key.creationDate) 
        {
            files[j + 1] = files[j];
            j--;
        }
        files[j + 1] = key;
    }
}

// sorting based on date modified meow
int partition(DynamicArray& files, int low, int high) 
{
    string pivot = files[high].lastModifiedDate;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (files[j].lastModifiedDate < pivot) 
        {
            i++;
            swap(files[i], files[j]);
        }
    }
    swap(files[i + 1], files[high]);
    return i + 1;
}

void quickSort(DynamicArray& files, int low, int high) 
{
    if (low < high) 
    {
        int pi = partition(files, low, high);
        quickSort(files, low, pi - 1);
        quickSort(files, pi + 1, high);
    }
}

void addFile(DynamicArray& files, vector<vector<int> >& dependencyGraph, vector<string>& fileNames) {
    // Create a new File object
    File newFile;

    // Take user input for file details
    cout << "Enter the file name: ";
    cin >> newFile.fileName;

    cout << "Enter the file extension: ";
    cin >> newFile.extension;

    cout << "Enter the file size in KB: ";
    cin >> newFile.sizeKB;

    cout << "Enter the file category: ";
    cin >> newFile.category;

    cout << "Enter the file creation date (YYYY-MM-DD): ";
    cin >> newFile.creationDate;

    cout << "Enter the file last modified date (YYYY-MM-DD): ";
    cin >> newFile.lastModifiedDate;

    // Optionally, you can allow the user to enter file dependencies (separated by semicolons)
    cout << "Enter file dependencies (separated by semicolons, leave empty if none): ";
    string dependenciesStr;
    cin.ignore();  // To ignore the newline left in the buffer by previous input
    getline(cin, dependenciesStr);

    // Parse the dependencies string
    stringstream depStream(dependenciesStr);
    string dep;
    while (getline(depStream, dep, ';')) {
        addDependency(newFile.dependenciesHead, dep);
        int depIndex = find(fileNames.begin(), fileNames.end(), dep) - fileNames.begin(); 
        if (depIndex == fileNames.size()) { 
            fileNames.push_back(dep); 
            dependencyGraph.push_back(vector<int>()); 
        } 
        dependencyGraph[depIndex].push_back(files.getSize()); 
    }

    // Add the new file to the DynamicArray
    files.addFile(newFile);
    fileNames.push_back(newFile.fileName); 
    dependencyGraph.push_back(vector<int>()); 
    cout << "File successfully added!" << endl;

    string operation = "ADD," + newFile.fileName;
    undoStack.push(operation);
}



void moveFile(DynamicArray& files) {
    string name;
    cout << "Enter the name of the file to move: ";
    cin >> name;

    // Search for the file in the dynamic array
    File* fileToMove = nullptr;
    for (int i = 0; i < files.getSize(); i++) {
        size_t extPos = files[i].fileName.find_last_of('.');
        string baseFileName = (extPos == string::npos) ? files[i].fileName : files[i].fileName.substr(0, extPos);

        if (baseFileName == name) {
            fileToMove = &files[i];
            break;
        }
    }

    // If file not found, inform the user
    if (fileToMove == nullptr) {
        cout << "File \"" << name << "\" not found." << endl;
        return;
    }

    // Display the current category of the file
    cout << "The file \"" << fileToMove->fileName << "\" is currently in the category: " << fileToMove->category << endl;

    // Ask the user for the new category
    string newCategory;
    cout << "Enter the new category to move the file \"" << fileToMove->fileName << "\": ";
    cin.ignore(); // Ignore leftover newline character
    getline(cin, newCategory);

    // Remove the file from its current category's AVL tree
    categoryTreeMap[fileToMove->category].removeFile(fileToMove);

    string operation = "MOVE," + name + fileToMove->extension+"," + fileToMove->category + "," + newCategory;

    // Change the file's category
    fileToMove->category = newCategory;

    // Add the file to the new category's AVL tree
    categoryTreeMap[newCategory].insertFile(fileToMove);
    
    // pushinh operation to stack
    undoStack.push(operation);


    // Inform the user of the successful move
    cout << "File \"" << fileToMove->fileName << "\" moved to category: " << newCategory << endl;
}

void saveDependencies(ofstream& outFile, DependencyNode* head) {
    while (head != nullptr) {
        outFile << head->dependencyName;
        if (head->next != nullptr) {
            outFile << ";";
        }
        head = head->next;
    }
}

void saveToFile(const DynamicArray& files, const string& filepath) 
{
    ofstream outFile(filepath);  // Open the file in overwrite mode

    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file " << filepath << endl;
        return;
    }

    // Write the header to the file
    outFile << "FileName,Extension,SizeKB,Category,CreationDate,LastModifiedDate,Dependencies" << endl;

    // Write each file's data to the file
    for (int i = 0; i < files.getSize(); i++) {
        const File& file = files[i];

        outFile << file.fileName << ","
                << file.extension << ","
                << file.sizeKB << ","
                << file.category << ","
                << file.creationDate << ","
                << file.lastModifiedDate;
        saveDependencies(outFile, file.dependenciesHead);        

        outFile << endl;  // End of the line for each file
    }

    outFile.close();  // Close the file
    cout << "File data successfully saved to " << filepath << endl;
}

// Function to display all categories and their files
void displayAllCategories() {
    for (auto& pair : categoryTreeMap) {
        cout << "\nCategory: " << pair.first << endl;
        pair.second.displayFiles();
    }
}

// Undo the last operation
void undo(DynamicArray& files) 
{
    if (!undoStack.isEmpty()) 
    {
        string lastOperation = undoStack.top();
        undoStack.pop();

        stringstream ss(lastOperation);
        string operationT;
        getline(ss, operationT, ',');
        if (operationT == "MOVE") 
        {
            stringstream ss(lastOperation);
            string operationType, fileName, oldCategory, newCategory;
            getline(ss, operationType, ',');
            getline(ss, fileName, ',');
            getline(ss, oldCategory, ',');
            getline(ss, newCategory, ',');
            // Move the file back to its old category
            for (int i = 0; i < files.getSize(); i++) 
            {
                if (files[i].fileName == fileName) 
                {
                    // Remove from new category
                    categoryTreeMap[newCategory].removeFile(&files[i]);

                    // Update file's category
                    files[i].category = oldCategory;

                    // Add back to old category
                    categoryTreeMap[oldCategory].insertFile(&files[i]);

                    cout << "Undo MOVE: File \"" << fileName << "\" moved back to category \"" << oldCategory << "\"" << endl;
                    return;
                }
            }
            cout << "Error: File \"" << fileName << "\" not found in the system." << endl;
        } 
        else if (operationT == "DELETE") 
        {
           {
            stringstream ss(lastOperation);
            string operationType, fileName, extension, category, fileSizeStr, creationDate, lastModifiedDate, dependenciesStr;

            getline(ss, operationType, ',');
            getline(ss, fileName, ',');
            getline(ss, extension, ',');
            getline(ss, category, ',');
            getline(ss, fileSizeStr, ',');
            getline(ss, creationDate, ',');
            getline(ss, lastModifiedDate, ',');
            getline(ss, dependenciesStr);
            // Recreate the deleted file object
            File restoredFile;
            restoredFile.fileName = fileName;
            restoredFile.extension = extension;
            restoredFile.category = category;
            restoredFile.sizeKB = stoi(fileSizeStr); // Convert size back to integer
            restoredFile.creationDate = creationDate;
            restoredFile.lastModifiedDate = lastModifiedDate;

            // Reconstruct the dependency list
            if (!dependenciesStr.empty()) 
            {
                stringstream depStream(dependenciesStr);
                string dependency;
                while (getline(depStream, dependency, '|')) 
                {
                    DependencyNode* newDependency = new DependencyNode(dependency);
                    if (restoredFile.dependenciesHead == nullptr) 
                    {
                        restoredFile.dependenciesHead = newDependency;
                    } 
                    else 
                    {
                        DependencyNode* temp = restoredFile.dependenciesHead;
                        while (temp->next != nullptr) 
                        {
                            temp = temp->next;
                        }
                        temp->next = newDependency;
                    }
                }
            }

            // Add the file back to the dynamic array
            files.addFile(restoredFile);

            // Add the file back to its category in the AVL tree
            categoryTreeMap[category].insertFile(&files[files.getSize() - 1]);

            cout << "Undo DELETE: File \"" << fileName << "\" restored to category \"" << category << "\"" << endl;
        } 
        } 
        else if (operationT == "ADD") 
        {
            stringstream ss(lastOperation);
            string operationType, fileName;
            getline(ss, operationType, ',');
            getline(ss, fileName, ',');
            // Remove the added file
            for (int i = 0; i < files.getSize(); i++) 
            {
                if (files[i].fileName == fileName) 
                {
                    // Remove from category
                    categoryTreeMap[files[i].category].removeFile(&files[i]);

                    // Remove from the dynamic array
                    files.removeFile(i);

                    cout << "Undo ADD: File \"" << fileName << "\" removed from the system." << endl;
                    return;
                }
            }
            cout << "Error: File \"" << fileName << "\" not found in the system." << endl;
        } 
        else 
        {
            cout << "Unknown operation type in undo stack: " << operationT << endl;
        }
    } 
    else 
    {
        cout << "No operations to undo." << endl;
    }
}


// Function to handle batch processing of file-related operations
void batchProcess() {
    while (!batchQueue.isEmpty()) {
        string operation = batchQueue.front();
        batchQueue.dequeue();
        // Execute the batch operation here (e.g., move files).
        cout << "Processing: " << operation << endl;
    }
}

// Function to add a file to the batch queue for processing
void addToBatchQueue(const string& operation) {
    batchQueue.enqueue(operation);
}

int main()
{
    string filePath = "MockDataSet.txt";
    DynamicArray files;
    vector<vector<int> > dependencyGraph;
    vector<string> fileNames;
    readFromFile(files, filePath, dependencyGraph, fileNames);
    cout << "Number of files read: " << files.getSize() << endl;


    showMenu(files, dependencyGraph, fileNames);

    saveToFile(files, filePath);

}
