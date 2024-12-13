#include <iostream>
#include "Menu.cpp"
#include "DataStructures.cpp"
using namespace std;

// ssorting based on name
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
            if (files[j].sizeKB > files[j + 1].sizeKB) 
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
