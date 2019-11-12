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
        vector<string> block (string buffer, int b);
    private :
        int rootsize;                   // maximum number of entries in ROOT
        int fatsize;                    // number of blocks occupied by FAT
        vector<string> filename;        // filenames in ROOT
        vector<int> firstblock;         // firstblocks in ROOT
        vector<int> fat;                // FAT

        //bool debugComponent = true;
};

Filesys::Filesys()
{}

Filesys::Filesys(string diskname, int numberofblocks, int blocksize):Sdisk(diskname, numberofblocks, blocksize)
{
    rootsize = getBlockSize() / 13; 
    fatsize = (4 * getNumberOfBlocks()) / getBlockSize() + 1; 
    
    //if (debugComponent)
        cout << "Filesys Checkpoint 1!" << endl;

    // Check if Sdisk has a filesystem
    string buffer; 
    getblock (1, buffer); 

    ostringstream outstream;
    
    //if (debugComponent)
        cout << "Filesys Checkpoint 2!" << endl;

    // empty 
    if(buffer[1] == '#') 
    {
        //if (debugComponent)
        cout << "Filesys Checkpoint 3!" << endl;

        // no file, create and store
        rootsize = getBlockSize() / 13;
        //ostringstream outstream;

        //if (debugComponent)
        cout << "Filesys Checkpoint 4!" << endl;

        for (int i = 1; i <= rootsize; i++)
        {
            // set up root 
            filename.push_back("xxxxxxxx");
            firstblock.push_back(0);
            //outstream << "xxxxxxxx" << " " << 0 << " ";
        }

        //if (debugComponent)
            cout << "Filesys Checkpoint 5!" << endl;

        //buffer = outstream.str();
        //vector<string> blocks = block(buffer, getBlockSize()); // getBlockLine()
        //putblock(1, blocks[0]);

        //if (debugComponent)
            cout << "Filesys Checkpoint 6!" << endl;

        // build the FAT
        fatsize = getBlockSize() / 5; // 4 + 1
        fat.push_back(2 + fatsize);
        fat.push_back(-1);

        //if (debugComponent)
            cout << "Filesys Checkpoint 7!" << endl;

        for (int i = 1; i <= fatsize; i++)
        {
            fat.push_back(-1);
        }

        //if (debugComponent)
            cout << "Filesys Checkpoint 8!" << endl;

        for (int i = 2 + fatsize; i < getNumberOfBlocks(); i++)
        {
            fat.push_back(i + 1);
        }
        fat[getNumberOfBlocks() - 1] = 0;  

        //if (debugComponent)
            cout << "Filesys Checkpoint 9!" << endl;

        fssynch();
    }

    else 
    {
        //if (debugComponent)
        //cout << "Filesys Checkpoint 10!" << endl;

        // read in ROOT 
        istringstream instream; 
        getblock(0, buffer);

        //if (debugComponent)
        //cout << "Filesys Checkpoint 11!" << endl;

        instream.str(buffer);

        for (int i = 0; i < rootsize; i++)
        {
            string file; 
            int block;

            instream >> file >> block; 
            filename.push_back(file); 
            firstblock.push_back(block);
        }

        //if (debugComponent)
        //cout << "Filesys Checkpoint 12!" << endl;

        // read in FAT 
        istringstream instream2;
        buffer.clear();

        for (int i = 0; i < fatsize; i++)
        {
            string block;
            getblock(2 + i, block);
            buffer += block;
        }

        instream2.str(buffer);

        //if (debugComponent)
        //cout << "Filesys Checkpoint 13!" << endl;

        // Start from beginning
        for (int i = 0; i < getNumberOfBlocks(); i++)
        {
            instream2 >> i; 
            fat.push_back(i);
        }

        //if (debugComponent)
        //cout << "Filesys Checkpoint 14!" << endl;
    }

    //if (debugComponent)
        //cout << "Filesys Checkpoint 15!" << endl;
}

int Filesys::fsclose()
{
    fssynch();
    cout << "Shutting down the filesystem." << endl; 
    exit(1);  // maybe use a different exit command? 
}

int Filesys::fssynch()
{
    string buffer; 
    ostringstream outstream, outstream2; 

    cout << "fssynch Checkpoint 1!" << endl;

   // Write FAT to the disk
    for (int i = 0; i < getNumberOfBlocks(); i++)
    {
        outstream << fat[i] << " ";
    }

    cout << "fssynch Checkpoint 2!" << endl;

    buffer = outstream.str(); 

    cout << "fssynch Checkpoint 3!" << endl;
    
    vector <string> blocks = block (buffer, getBlockSize());

    cout << "fssynch Checkpoint 4!" << endl;

    for (int i = 1; i <= blocks.size(); i++)
    {
        putblock (i, blocks[i - 1]);
    }

    // Write Root to the disk
    for (int i = 0; i < rootsize; i++)
    {
        outstream2 << filename[i] << " " << firstblock[i] << " ";
    }

    cout << "fssynch Checkpoint 5!" << endl;

    buffer = outstream2.str(); 
    
    blocks.clear(); 
    blocks = block(buffer, getBlockSize());
    
    cout << "fssynch Checkpoint 6!" << endl;

    for (int i = 0; i < blocks.size(); i++)
    {
        putblock (i, blocks[i]);
    }

    cout << "fssynch Checkpoint 7!" << endl;

    cout << "fssynch Checkpoint 8!" << endl;

    return 0;
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
            filename[i] = file;
            fssynch();
            return 1;
        }
    }
    return 0;
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
    }

    cout << "No such file exists!" << endl;
    return -1;
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
                putblock(allocate, block);     // Where is our buffer value coming from?
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
    putblock(allocate, block);     // Where is our buffer value coming from?
    return allocate;
}

int Filesys::delblock(string file, int blocknumber)
{
    cout << "delblock Checkpoint 1!" << endl;
    
    int iblock = getfirstblock(file);

    if (iblock == -1) // block does not exist / is empty 
    {
        return 0; 
    }

    else if (iblock == blocknumber) // block to delete is the first block 
    {
        for (int i = 0; i < filename.size(); i++)
        {
            if(filename[i] == file)
            {
                firstblock[i] = fat[blocknumber];
            }
        }
    }

    else 
    {
        while (fat[iblock] != 0 && fat[iblock] != blocknumber)
        {
            cout << "iblock: " << iblock  << ", fat[iblock]: " << fat[iblock] << ", blocknumber: " << blocknumber << endl;

            iblock = fat[iblock];
        }

        if (fat[iblock] != 0)
        {
            fat[iblock] = fat[blocknumber];
        }

        else 
        {
            return 0; 
        }
    }

    //cout << "delblock Checkpoint 6!" << endl;

    fat[blocknumber] = fat[0];
    fat[0] = blocknumber;
    //cout << "delblock Checkpoint 7!" << endl;
    
    fssynch();
    
    //cout << "delblock Checkpoint 8!" << endl;

    return 0; 
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