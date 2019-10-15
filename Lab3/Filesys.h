#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "Sdisk.h"

class Filesys: public Sdisk
{
    public :
        Filesys();
        Filesys(string diskname, int numberofblocks, int blocksize);
        int fsclose();
        int fssynch();
        int newfile(string file);
        int rmfile(string file);
        int getfirstblock(string file);
        int addblock(string file, string block);
        int delblock(string file, int blocknumber);
        int readblock(string file, int blocknumber, string& buffer);
        int writeblock(string file, int blocknumber, string buffer);
        int nextblock(string file, int blocknumber);
    private :
        int rootsize;                   // maximum number of entries in ROOT
        int fatsize;                    // number of blocks occupied by FAT
        vector<string> filename;        // filenames in ROOT
        vector<int> firstblock;         // firstblocks in ROOT
        vector<int> fat;                // FAT
};

Filesys::Filesys() { }

Filesys::Filesys(string diskname, int numberofblock, int blocksize)
{

    Sdisk  (disk, int numberofblocks, int blocksize)

    // Check if Sdisk has a filesystem
    string buffer; 
    getblock (1, buffer); 

    if(buffer[1] == "#")               // empty 
    {
        // no file, create and store
        rootsize = getBlockSize() / 13;
        ostringstream outstream;

        for (int i = 1; i <= rootsize; i++)
        {
            // set up root 
            filename.push_back("xxxxxxxx");
            firstblock.push_back(0);
            outstream << "xxxxxxxx" << " " << 0 << " ";
        }

        buffer = outstream.str();
        vector<string> blocks = block(buffer, getBlockSize()); // getBlockLine()
        putblock(1, blocks[0]);

        // build the FAT
        fatsize = getBlockSize() / 5; // 4 + 1
        fat.push_back(2 + fatsize);
        fat.push_back(-1);

        for (int i = 1; i <= fatsize; i++)
        {
            fat.push_back(i + 1);
            fat[fatsize() - 1] = 0;
        }

        for (int i = 2 + fatsize(); i < getNumberOfBlocks(); i++)
        {
            outstream << fat[i] << " ";
        }
    }

    else 
    {
        // read in file system, root is in buffer 
    }
}

int Filesys::fsclose()
{
    
}

int Filesys::fssynch()
{
    // Write Root to the disk

    // Write FAT to the disk
}

int Filesys::newfile(string file)
{
    // search root directory for xxxxxxxx
    // change it to filename 
    // change 0 to 4 
    // change 4 in FAT to 5 (next free block)
}

int Filesys::rmfile(string file)
{
    
}

int Filesys::getfirstblock(string file)
{
    
}

int Filesys::addblock(string file, string block)
{

}

int Filesys::delblock(string file, int blocknumber)
{
    
}  

int Filesys::readblock(string file, int blocknumber, string& buffer)
{

}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
    
}

int Filesys::nextblock(string file, int blocknumber)
{
    
}