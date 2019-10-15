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

Filesys::Filesys() 
{
    
}

Filesys::Filesys(string diskname, int numberofblock, int blocksize)
{
    Sdisk (disk, int numberofblocks, int blocksize)
    string buffer; 
    getblock (1, buffer); 
    if(buffer[0] == "#")               // empty 
    {
        // no file, create and store
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