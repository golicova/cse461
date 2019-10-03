#include <iostream>
#include <string>

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

}

int Sdisk::getblock (int blockNumber, string& buffer)
{

}

int Sdisk::putblock (int blockNumber, string buffer)
{

}

int Sdisk::getNumberOfBlocks()
{

}

int Sdisk::getBlockSize()
{  
    
}