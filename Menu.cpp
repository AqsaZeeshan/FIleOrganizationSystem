#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SortingAlgorithms.cpp>
using namespace std;

struct File {
    string fileName;
    string extension;
    int sizeKB;
    string category;
    string creationDate;
    string lastModifiedDate;
};

void viewAllFiles();
void addFile();
void deleteFile();
void moveFile();
void sortFiles();
void searchFiles();
void displayByCategory();
void displayDependencies();

void showMenu() {
    int choice;
    do {
        cout << "==========================================" << endl;
        cout << "         File Organization System         " << endl;
        cout << "==========================================" << endl;
        cout << "1. View All Files" << endl;
        cout << "2. Add a New File" << endl;
        cout << "3. Delete a File" << endl;
        cout << "4. Move a File to a Different Category" << endl;
        cout << "5. Sort Files" << endl;
        cout << "     a) By Name" << endl;
        cout << "     b) By Size" << endl;
        cout << "     c) By Creation Date" << endl;
        cout << "     d) By Last Modified Date" << endl;
        cout << "6. Search Files" << endl;
        cout << "     a) By Name" << endl;
        cout << "     b) By File Extension" << endl;
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
                sortFiles();
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
