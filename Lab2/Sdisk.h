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
        Sdisk (string diskName, int numberOfBlocks, int blockSize);
        int getblock (int blockNumber, string& buffer);
        int putblock (int blockNumber, string buffer);
        int getNumberOfBlocks();                                        // accessor function
        int getBlockSize();                                             // accessor function
    private :
        string diskName;                                                // file name of software-disk
        int numberOfBlocks;                                             // number of blocks on disk
        int blockSize;                                                  // block size in bytes
};


Sdisk::Sdisk (string diskName, int numberOfBlocks, int blockSize)
{
    // set attributes (diskname, number of blocks, block size)
    this->diskName = diskName; 
    this->numberOfBlocks = numberOfBlocks; 
    this->blockSize = blockSize; 

    fstream fStream; 

    fStream.open(this->diskName.c_str(), ios::in); 
    if (fStream.fail())
    {
        fStream.open(this->diskName.c_str(), ios::out);

        if (fStream.fail())
        {
            return; 
        }
        else 
        {
            for (int i = 0; i < this->numberOfBlocks * this->blockSize; i++)
            {
                fStream.put('#');                                               // create allocated space for the disk
            }
        }

        fStream.close();   
    }
}

int Sdisk::getblock (int blockNumber, string& buffer)
{
    fstream iofile;
    iofile.open(this->diskName.c_str(), ios::in);

    if(iofile.fail())
    {
        return 0; 
    }

    iofile.seekg (blockNumber * this->blockSize);    // blocknumber k starts at k * blocksize
    buffer = ""; // char c; 

    for (int i = 0; i < this-> blockSize; i++)
    {
        buffer += iofile.get(); 
    }
    
    iofile.close(); 

    return 1;
    
}

int Sdisk::putblock (int blockNumber, string buffer)
{
    fstream iofile;
    iofile.open(this->diskName.c_str(), ios::in | ios::out);

    if(iofile.fail())
    {
        return 0; 
    }

    iofile.seekp(blockNumber * this->blockSize);    // blocknumber k starts at k * blocksize
    char c; 

    for (int i = 0; i < buffer.length() && i < this->blockSize; i++)
    {
        iofile.put(buffer[i]); 
    }
    
    iofile.close(); 

    return 1;
}

int Sdisk::getNumberOfBlocks()
{
    return this->numberOfBlocks;
}

int Sdisk::getBlockSize()
{  
    return this->blockSize; 
}
