#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<sstream>

using namespace std;

struct DependencyNode {
    string dependencyName;  // Name of the dependency
    DependencyNode* next;   // Pointer to the next node

    DependencyNode(const string& name) : dependencyName(name), next(nullptr) {}
};

struct File {
    string fileName;
    string extension;
    int sizeKB;
    string category;
    string creationDate;
    string lastModifiedDate;
    DependencyNode* dependenciesHead;
    vector<string> dependencies; // we can make a list ?

    File() : dependenciesHead(nullptr) {}
};

// forward declerations
void displayFiles(const class DynamicArray& files);  
void sortFiles(class DynamicArray& files);  
void mergeSort(class DynamicArray& files, int left, int right);
void bubbleSort(class DynamicArray& files);
void insertionSort(class DynamicArray& files);
void quickSort(class DynamicArray& files, int low, int high);

void displayFiles(const DynamicArray& files);
void addFile(class DynamicArray& files);
void deleteFile();
void moveFile();
void searchFiles();
void displayByCategory();
void displayDependencies();

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

};

void readFromFile( DynamicArray& files, const string& filepath) 
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
            fileEntry.dependencies.push_back(dep);
        }

        // Add the File object to the DynamicArray
        files.addFile(fileEntry);
    }

    file.close(); // Close the file
    cout << "File data successfully read into the DynamicArray.\n";
}

void showMenu(DynamicArray& files) {
    int choice;
    do {
        cout << "==========================================" << endl;
        cout << "         File Organization System         " <<endl;
        cout << "==========================================" <<endl;
        cout << "1. View All Files" <<endl;
        cout << "2. Add a New File" <<endl;
        cout << "3. Delete a File" << endl;
        cout << "4. Move a File to a Different Category" <<endl;
        cout << "5. Sort Files" << endl;
        cout << "     a) By Name" << endl;
        cout << "     b) By Size" << endl;
        cout << "     c) By Creation Date" << endl;
        cout << "     d) By Last Modified Date" << endl;
        cout << "6. Search Files" <<endl;
        cout << "     a) By Name" << endl;
        cout << "     b) By File Extension" <<endl;
        cout << "7. Display Files by Category" << endl;
        cout << "8. Display File Dependencies (Graph View)" << endl;
        cout << "9. Exit" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayFiles(files);
                break;
            case 2:
                addFile(files);
                break;
            case 3:
                //deleteFile();
                break;
            case 4:
                //moveFile();
                break;
            case 5:
                sortFiles(files);
                break;
            case 6:
                //searchFiles();
                break;
            case 7:
                //displayByCategory();
                break;
            case 8:
                //displayDependencies();
                break;
            case 9:
                cout << "Exiting File Organization System..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 9);
}

void displayFiles(const DynamicArray& files) {
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
             << ", Last Modified Date: " << file.lastModifiedDate 
             << endl;
    }

    cout << "==========================================" << endl;
}

void sortFiles(DynamicArray& files)
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
            displayFiles(files); 
            break;
        case 2:
            bubbleSort(files); 
            cout << "Files sorted by Size.\n";
            displayFiles(files); 
            break;
        case 3:
            insertionSort(files); 
            cout << "Files sorted by Creation Date.\n";
            displayFiles(files); 
            break;
        case 4:
            quickSort(files, 0, files.getSize() - 1); 
            cout << "Files sorted by Last Modified Date.\n";
            displayFiles(files); 
            break;
        default:
            cout << "Invalid choice. Returning to main menu.\n";
    }
}
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

void addFile( DynamicArray& files) {
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
        newFile.dependencies.push_back(dep);
    }

    // Add the new file to the DynamicArray
    files.addFile(newFile);
    cout << "File successfully added!" << endl;
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

        // Write the dependencies (if any)
        if (!file.dependencies.empty()) {
            for (size_t j = 0; j < file.dependencies.size(); j++) {
                outFile << file.dependencies[j];
                if (j < file.dependencies.size() - 1) {
                    outFile << ";";  // Separate dependencies with a semicolon
                }
            }
        }

        outFile << endl;  // End of the line for each file
    }

    outFile.close();  // Close the file
    cout << "File data successfully saved to " << filepath << endl;
}

int main()
{
    string filePath = "MockDataSet.txt";
    DynamicArray files;
    readFromFile(files, filePath);
    cout << "Number of files read: " << files.getSize() << endl;
    displayFiles(files);

    showMenu(files);

    saveToFile(files, filePath);

}
