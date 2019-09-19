#include <iostream> 

using namespace std; 

class Sdisk
{
    public: 
        Sdisk (string diskname, int numberOfBlocks, int blockSize);
        int getBlock (int blockNumber, string& buffer); 
        int putBlock (int blockNumber, string buffer); 
        int getNumberOfBlocks(); 
        int getBlockSize(); 

    private: 
        string diskname; 
        int numberOfBlocks; 
        int blockSize; 
}

Sdisk:Sdisk (diskName, numberOfBlocks, blockSize) 
{}

Sdisk::getBlock(blockNumber, buffer)
{}

Sdisk::putBlock (blockNumber, buffer)
{}

