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
        bool checkblock(string file, int blocknumber);
    private :
        int rootsize;                   // maximum number of entries in ROOT
        int fatsize;                    // number of blocks occupied by FAT
        vector<string> filename;        // filenames in ROOT
        vector<int> firstblock;         // firstblocks in ROOT
        vector<int> fat;                // FAT
        vector<string> block(string buffer, int b);
};

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname, numberofblocks, blocksize) {}

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname, numberofblocks, blocksize)
{
    //Sdisk sdisk (diskname, numberofblocks, blocksize);  // How do we utilize this Sdisk object in this constructor?
    
    // Check if Sdisk has a filesystem
    string buffer; 
    getblock (1, buffer); 

    // empty 
    if(buffer[1] == '#') 
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
            fat[fatsize - 1] = 0;
        }

        for (int i = 2 + fatsize; i < getNumberOfBlocks(); i++)
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
    string buffer; 
    ostringstream outstream; 

    // Write Root to the disk
    for (int i = 0; i < filename.size(); i++)
    {
        outstream << filename[i] << " " << firstblock[i] << " ";
    }
    
    // Write FAT to the disk
    for (int i = 0; i < fat.size(); i++)
    {
        outstream << fat[i] << " ";
    }

    buffer = outstream.str(); 

    vector <string> blocks = block (buffer, getBlockSize());
    for (int i = 0; i < blocks.size; i++)
    {
        putblock (2 + i, blocks[i]);
    }
}

int Filesys::newfile(string file)
{
    // search root directory for xxxxxxxx
    // change it to filename 
    // change 0 to 4 
    // change 4 in FAT to 5 (next free block)

    for (int i = 0; i < rootsize; i++)
    {
        if (filename[i] == file)
        {
            cout << "file name" << endl;
            return 0;
        }
    }
    for (int i = 0; i < rootsize; i++)
    {
        if (filename[i] == "xxxxxxxx")
        {
            fssynch();
            return 1;
        }
    }
}

int Filesys::rmfile(string file)
{
    for (int i = 0; i < rootsize; i++)
    {
        if(filename[i] ==  file)
        {
            if(firstblock[0] != 0)
            {
                cout << "File not empty.";
                return 0; 
            }
        }
        else 
        {
            filename[i] = "xxxxxxxx";
            fssynch(); 
            return 1; 
        }
    }
    return 0; 
}

int Filesys::getfirstblock(string file)
{
    for (int i = 0; i < rootsize; i++)
    {
        if (filename[i] == file)
        {
            return firstblock[i];
        }
        else 
        {
            return -1;
        }
    }

    cout << "No such file exists!" << endl;
    return 0;
}

int Filesys::addblock(string file, string block)
{
    string buffer; 
    ostringstream outstream;

    int first = getfirstblock(file);

    buffer = outstream.str();

    if (first == -1)
    {
        return 0;
    }

    int allocate = fat[0];
    if (allocate == 0)
    {
        // no free blocks
        return 0; 
    }
    
    else 
    {
        fat[0] = fat[fat[0]];
        fat[allocate] = 0; 
    }
    
    if (first == 0)
    {
        for (int i = 0; i < rootsize; i++)
        {
            if (filename[i] == file)
            {
                firstblock[i] = allocate;
                fssynch();
                putblock(allocate, buffer);     // Where is our buffer value coming from?
                return allocate;
            }
        }
    }
    else
    {
        cout << "Not Empty" << endl;
    }
    
    int iblock = first;
    
    while (fat[iblock] != 0)
    {
        iblock = fat[iblock];     // points to first 0 in FAT
    }
    fat[iblock] = allocate; 
    
    fssynch();
    putblock(allocate, buffer);     // Where is our buffer value coming from?
    return allocate;
}

int Filesys::delblock(string file, int blocknumber)
{
    if (!checkblock(file, blocknumber))
    {
        for (int i = 0; i < filename.size(); i++)
        {
            if (file == filename[i])
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
}  

int Filesys::readblock(string file, int blocknumber, string& buffer)
{
    if (checkblock (file, blocknumber))
	{
		getblock (blocknumber, buffer); 
		return 1; 
	}
	
	else 
	{
		return 0; 
    }
}

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
    if (checkblock (file, blocknumber))
	{
		putblock (blocknumber, buffer); 
		return 1; 
	}
	
	else 
	{
		return 0; 
    }
}

int Filesys::nextblock(string file, int blocknumber)
{
    if (checkblock (file, blocknumber))
	{
		return fat[blocknumber];
	}
	
	else 
	{
		return -1; // fail
    }
}

bool Filesys::checkblock(string file, int blocknumber)
{
    int iblock = getfirstblock (file); 
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

vector<string> Filesys::block(string buffer, int b)
{
    // blocks the buffer into a list of blocks of size b

    vector<string> blocks;
    int numberofblocks = 0;

    if (buffer.length() % b == 0) 
    { 
        numberofblocks= buffer.length() / b;
    }
    else 
    { 
        numberofblocks= buffer.length() / b +1;
    }

    string tempblock;

    for (int i=0; i<numberofblocks; i++)
    { 
        tempblock=buffer.substr(b * i, b);
        blocks.push_back(tempblock);
    }

    int lastblock=blocks.size()-1;

    for (int i = blocks[lastblock].length(); i < b; i++)
    {  
        blocks[lastblock] += "#";
    }

    return blocks;
}