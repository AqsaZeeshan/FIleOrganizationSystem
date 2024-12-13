#include <iostream>
#include "Menu.cpp"
#include "Menu.cpp"
using namespace std;

//class to handle dynamic arrays. :(
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

        int getSize()
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

};
