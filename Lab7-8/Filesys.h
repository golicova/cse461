
#ifndef FILESYS_H
#define FILESYS_H

#include "Sdisk.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Filesys: public Sdisk
{
public:
    Filesys(string diskname, int numberofblocks, int blocksize);
    int fsclose();
    int fssynch();
    int newfile(string file);
    int rmfile (string file);
    int getfirstblock(string file);
    int addblock(string file, string buffer);
    int delblock(string file, int blocknumber);
    int readblock(string file, int blocknumber, string& buffer);
    int writeblock(string file, int blocknumber, string buffer);
    int nextblock(string file, int blocknumber);
    vector<string> ls();
    //testing functions
    void displayRoot();
    void displayFat();
private:
    int rootsize;   //maximum entries in ROOT
    int fatsize;    //maximum entries in FAT
    vector<string> filename;    //filenames in ROOT
    vector<int> firstblock;     //firstblocks of files in ROOT
    vector<int> fat;    //file allocation table
    bool checkblock(string file, int blocknumber);
};

vector<string> block(string buffer, int b);

vector<string> block(string buffer, int b)
{
    // blocks the buffer into a list of blocks of size b

    vector<string> blocks;
    int numberofblocks=0;

    if (buffer.length() % b == 0)
    { 
        numberofblocks= buffer.length()/b;
    }
    else
    { 
        numberofblocks= buffer.length()/b +1;
    }

    string tempblock;

    for (int i=0; i<numberofblocks; i++)
    { 
        tempblock=buffer.substr(b*i,b);
        blocks.push_back(tempblock);
    }

    int lastblock=blocks.size()-1;

    for (int i=blocks[lastblock].length(); i<b; i++)
    { 
        blocks[lastblock]+="#";
    }

    return blocks;

}

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):
         Sdisk(diskname, numberofblocks, blocksize)
{
    string buffer;
    
    getblock(0, buffer);
    cout << buffer << endl;
    if (buffer[1] == '#')
    {
        //no filesystem, need to create
        rootsize = getBlockSize()/12; //was 13
        fatsize = (5*getNumberOfBlocks()/getBlockSize()) + 1; // was 4*getnum()

        //creating the root
        for (int i = 0; i < rootsize; ++i)
        {
            filename.push_back("xxxxxxxx");
            firstblock.push_back(0);
        }

        //creating the FAT
        fat.push_back(2+fatsize); //since first free block at block[2+fatsize]
        fat.push_back(-1); //since next entry in fat is for the root, prevents
                           //root from being overwritten
        
        //allocating blocks for the fat itself
        for (int i = 0; i < fatsize; ++i)
        {
            fat.push_back(-1);
        }

        //initializes locations of next open blocks
        for (int i = fatsize+2; i < getNumberOfBlocks(); ++i)
        {
            fat.push_back(i+1);
        }

        fat[fat.size()-1] = 0; //denotes end of available blocks

        fssynch(); 
    }
    else
    {
        //read in the filesystem
        //remember that root is already in buffer
        cout << "Reading in root..." << endl;
        
        rootsize = getBlockSize()/13;
        fatsize = (4*getNumberOfBlocks()/getBlockSize()) + 1;

        istringstream instream1;
        
        instream1.str(buffer);

        for (int i = 0; i < rootsize; ++i)
        {
            string f;
            int b;
            instream1 >> f >> b;
            filename.push_back(f);
            firstblock.push_back(b);
        }
        cout << "Reading in fat..." << endl;
        //read in the fat
        istringstream instream2;
        buffer.clear();

        for (int i = 0; i < fatsize; ++i)
        {
            string b;
            getblock(2+i, b);
            buffer += b;
        }

        instream2.str(buffer);

        for (int i = 0; i < getNumberOfBlocks(); ++i)
        {
            int b;
            instream2 >> b;
            fat.push_back(b);
        }

        cout << "Disk read in, root and fat created.  System booted!" << endl;
    }
}

int Filesys::fsclose()
{
    return 0;
}

int Filesys::fssynch()
{
    //write the fat to sdisk
    //remembering that fat is in blocks 2 thru [2+fatsize]
    string buffer;

    ostringstream outstream;

    for (int i = 0; i < fat.size(); ++i)
    {
        outstream << fat[i] << " ";
    }

    buffer = outstream.str(); //buffer now contains all fat data

    vector<string> blocks = block(buffer, getBlockSize());

    for (int i = 0; i < blocks.size(); ++i)
    {
        putblock(2+i, blocks[i]);
    }

    //write the root to disk
    ostringstream outstream2;
    buffer.clear();

    for (int i = 0; i < filename.size(); ++i)
    {
        outstream2 << filename[i] << " " << firstblock[i] << " ";
    }

    buffer = outstream2.str();

    //since contents of buffer only occupy one block, and root only occupies
    //one block, no need to run block() function

    putblock(1, buffer);

    return 0;
}

int Filesys::newfile(string file)
{
    for (int i = 0; i < filename.size(); ++i)
    {
        if (filename[i] == file)
        {
            cout << "File exists!" << endl;
            return 0; //error code 0
        }
    }

    for (int i = 0; i < filename.size(); ++i)
    {
        if (filename[i] == "xxxxxxxx")
        {
            filename[i] = file;
            
            fssynch();
            cout << "file added to root!" << endl;
            return 1; //successful
        }
    }
    return 0;
}

int Filesys::rmfile(string file)
{
    for (int i = 0; i < rootsize; ++i)
    {
        if (filename[i] == file)
        {
            if (firstblock[i] != 0)
            {
                cout << "File not empty!" << endl;
                return 0; //error code
            }
            else
            {
                filename[i] = "xxxxxxxx";
                fssynch();
                return 1; //successful
            }
        }
    }
    cout << "File not found!" << endl;
    return 0; //error
}

//Returns either the first block used by the file, or -1 if file not found
int Filesys::getfirstblock(string file)
{
    for (int i = 0; i < rootsize; ++i)
    {
        if (filename[i] == file)
        {
            return firstblock[i];
        }
    }
    //cout << "File not found!" << endl;
    return -1; //error code
}

//Returns block that was allocated
int Filesys::addblock(string file, string buffer)
{
    int first = getfirstblock(file);

    if (first == -1)
    {
        return 0; //since file not found in getfirstblock()
    }
    
    int allocate = fat[0];

    if (allocate == 0)
    {
        //no free block
        return 0; //error
    }

    fat[0] = fat[fat[0]]; //updates free list
    fat[allocate] = 0; //updates the fat
    
    if (first == 0)
    {
        for (int i = 0; i < rootsize; ++i)
        {
            if (filename[i] == file)
            {
                firstblock[i] = allocate;
                fssynch();
                putblock(allocate, buffer); //write to disk
                return allocate; //returns block number that was allocated
            }
        }
    }
    else
    {
        //not empty file
        int block = first;

        //traverse the fat until we find the first 0
        while (fat[block] != 0)
        {
            block = fat[block];
        }

        fat[block] = allocate;

        fssynch();

        putblock(allocate, buffer);

        return allocate;
    }
    fssynch();
    return 1;
}

int Filesys::delblock(string file, int blocknumber)
{
    if (!checkblock(file, blocknumber))
    {
        return 0; //error
    }

    if (getfirstblock(file) == blocknumber)
    {
        for (int i = 0; i < filename.size(); ++i)
        {
            if (filename[i] == file)
            {
                firstblock[i] = fat[blocknumber];
                break;
            }
        }
    }
    else
    {
        int iblock = getfirstblock(file);
        while (fat[iblock] != blocknumber)
        {
            iblock = fat[iblock];
        }
        fat[iblock] = fat[blocknumber];
    }
    fat[blocknumber] = fat[0];
    fat[0] = blocknumber;
    fssynch();
    return 1; //success
}

int Filesys::readblock(string file, int blocknumber, string& buffer)
{
    if (checkblock(file, blocknumber))
    {
        getblock(blocknumber, buffer);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
    if (checkblock(file, blocknumber))
    {
        putblock(blocknumber, buffer);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Filesys::nextblock(string file, int blocknumber)
{
    if (checkblock(file, blocknumber))
    {
        return fat[blocknumber]; //success
    }
    return -1; //error
}

vector<string> Filesys::ls()
{
    vector<string> flist;
    
    for (int i = 0; i < filename.size(); ++i)
    {
        if (filename[i] != "xxxxxxxx")
        {
            flist.push_back(filename[i]);
        }
    }

    return flist;
}

void Filesys::displayRoot()
{
    cout << "Displaying root..." << endl;
    
    if (filename.empty())
    {
        cout << "Root directory empty!" << endl;
        return;
    }

    for (int i = 0; i < filename.size(); ++i)
    {
        cout << filename[i] << " " << firstblock[i] << endl;
    }
}

void Filesys::displayFat()
{
    cout << "Displaying fat..." << endl;

    for (int i = 0; i < fat.size(); ++i)
    {
        cout << "Block: " << i << " Points to: " << fat[i] << endl;
    }
}

bool Filesys::checkblock(string file, int blocknumber)
{
    int iblock = getfirstblock(file);

    while (iblock != 0)
    {
        if (iblock == blocknumber)
        {
            return true;
        }
        iblock = fat[iblock];
    }
    
    return false;
}


#endif
