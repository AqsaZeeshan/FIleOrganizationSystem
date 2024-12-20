#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<sstream>
#include "SortingAlgorithms.cpp"
#include "DataStructures.cpp"
using namespace std;
/* should the dynamic array of files be 
global or created in main?? confusion. I think we can make a global 
array so that it can be globally accessed. Do give thoughts. 
BEcause werna we will have to pass everything to the show meny function ?*/

struct File {
    string fileName;
    string extension;
    int sizeKB;
    string category;
    string creationDate;
    string lastModifiedDate;
    vector<string> dependencies;
};


void displayFiles(const DynamicArray& files);
void addFile();
void deleteFile();
void moveFile();
void sortFiles(DynamicArray& files);
void searchFiles();
void displayByCategory();
void displayDependencies();

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
                viewAllFiles();
                break;
            case 2:
                addFile();
                break;
            case 3:
                deleteFile();
                break;
            case 4:
                moveFile();
                break;
            case 5:
                sortFiles(files);
                break;
            case 6:
                searchFiles();
                break;
            case 7:
                displayByCategory();
                break;
            case 8:
                displayDependencies();
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
            mergeSort(files, 0, files.getSize() - 1); // Call merge sort
            cout << "Files sorted by Name.\n";
            displayFiles(files); // Display sorted files
            break;
        case 2:
            bubbleSort(files); // Call bubble sort
            cout << "Files sorted by Size.\n";
            displayFiles(files); // Display sorted files
            break;
        case 3:
            insertionSort(files); // Call insertion sort
            cout << "Files sorted by Creation Date.\n";
            displayFiles(files); // Display sorted files
            break;
        case 4:
            quickSort(files, 0, files.getSize() - 1); // Call quick sort
            cout << "Files sorted by Last Modified Date.\n";
            displayFiles(files); // Display sorted files
            break;
        default:
            cout << "Invalid choice. Returning to main menu.\n";
    }
}

int main()
{
    string filePath = "/Users/aqsazeeshan/Desktop/FileOrganizationSystem/MockDataSet.txt";
    DynamicArray files;
    readFromFile(files, filepath);
    cout << "Number of files read: " << files.getSize() << endl;
    displayFiles(files);

}
