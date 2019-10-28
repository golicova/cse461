#include <iostream>
#include <fstream>
#include <sstream>  // Use strings as our buffer
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Sdisk
{
    public :
        Sdisk (string diskName, int numberOfBlocks, int blockSize);     // Creates SD with initial name, amount of blocks, and sets a size per block
        int getblock (int blockNumber, string& buffer);                 // Gets buffer string from block number
        int putblock (int blockNumber, string buffer);                  // Sets buffer string into block number
        int getNumberOfBlocks();                                        // Accessor function to return numberOfBlocks in Sdisk
        int getBlockSize();                                             // Accessor function to return the blockSize set in Sdisk
    private :
        string diskName;                                                // File name of software-disk
        int numberOfBlocks;                                             // Number of blocks on disk
        int blockSize;                                                  // Block size in bytes
};

// Creates SD with initial name, amount of blocks, and sets a size per block
Sdisk::Sdisk (string diskName, int numberOfBlocks, int blockSize)
{
    // Set attributes (diskname, number of blocks, block size)
    this->diskName = diskName; 
    this->numberOfBlocks = numberOfBlocks; 
    this->blockSize = blockSize; 

    // Find file to read from
    fstream fStream; 
    fStream.open(this->diskName.c_str(), ios::in); 

    // Check if file failed to open to read
    if (fStream.fail())
    {
        // Set fstream to write onto disk
        fStream.open(this->diskName.c_str(), ios::out);

        // Check if file failed to open to write
        if (fStream.fail())
        {
            return; // Cancel SDisk creation
        }
        else 
        {
            // Iterate through the amount of blocks * size of each block
            for (int i = 0; i < this->numberOfBlocks * this->blockSize; i++)
            {
                // Create allocated space for the Sdisk
                fStream.put('#');
            }
        }

        // Close the file after creating it.
        fStream.close();   
    }
}

// Gets buffer string from block number
int Sdisk::getblock (int blockNumber, string& buffer)
{
    // Find file to read from
    fstream iofile;
    iofile.open(this->diskName.c_str(), ios::in);   // Ready for input

    // Check if file failed to open to read
    if(iofile.fail())
    {
        // For failure
        return 0;
    }

    // Start at blocknumber k starts at k * blocksize
    iofile.seekg (blockNumber * this->blockSize);
    buffer = ""; // char c; 

    // Iterate through entire block to initialize buffer
    for (int i = 0; i < this-> blockSize; i++)
    {
        buffer += iofile.get(); 
    }
    
    // Close the file after reading it.
    iofile.close(); 

    // For success
    return 1;
    
}

// Sets buffer string into block number
int Sdisk::putblock (int blockNumber, string buffer)
{
    // Find file to read from or to write to
    fstream iofile(this->diskName.c_str(), ios::in | ios::out);

    // Check if file failed to open to read or to write
    if(iofile.fail())
    {
        // For failure
        return 0; 
    }

    // Start at blocknumber k starts at k * blocksize
    iofile.seekp(blockNumber * this->blockSize);
    //char c; 

    for (int i = 0; i < buffer.length() && i < this->blockSize; i++)
    {
        iofile.put(buffer[i]); 
    }
    
    // Close the file after writing to it.
    iofile.close(); 

    // For success
    return 1;
}

// Accessor function to return numberOfBlocks in Sdisk
int Sdisk::getNumberOfBlocks()
{
    return this->numberOfBlocks;
}

// Accessor function to return the blockSize set in Sdisk
int Sdisk::getBlockSize()
{  
    return this->blockSize; 
}
