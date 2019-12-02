#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Filesys.h"

class Table : public Filesys
{
    public :
        Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile);
        int BuildTable(string input_file);
        int Search(string value);
    private :
        string flatfile;
        string indexfile;
        int numberofrecords;
        Filesys filesystem;
        int IndexSearch(string value);
};

Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile):Filesys(diskname, numberofblocks, blocksize)
{
    ifstream infile;
    infile.open(diskname.c_str);
    
    // Create flatfile in filesys
    newfile(flatfile);

    string buffer1;
	for (int i = 0; i < blocksize; i++)   // blockSize = 128
	{
		buffer1 += '#';
	}
    
    vector<string> blocked = block(buffer1, blocksize);    // blockSize = 128
    
    for (int i = 0; i < blocked.size(); i++) 
    {
        addblock(flatfile, blocked[i]);
    }

    // Create indexfile in filesys
    newfile(indexfile);

    string buffer2;
	for (int i = 0; i < blocksize; i++)   // blockSize = 128
	{
		buffer2 += '#';
	}
    
    vector<string> blocked = block(buffer2, blocksize);    // blockSize = 128
    
    for (int i = 0; i < blocked.size(); i++) 
    {
        addblock(indexfile, blocked[i]);
    }

    cout << "Table created" << endl;
}

int Table::BuildTable(string input_file)
{
    ifstream infile; 
    infile.open(input_file.c_str());

    string record; 
    int count = 0; 
    infile.getline() >> record; 
    vector<string> key; 
    vector<string> iblock; 
    ostringstream outstream;

    while(infile.good())
    {
        string pKey = record.subtr(0,5); 
        vector<string> oblock = block(record, getBlockSize());
        int blockid = addblock (flatfile, oblock[0]);
        outstream << pKey << " " << blockid << " "; 
        count++; 
        
        if(count == 4)
        {
            vector<string> o2block = block(outstream.str(), getBlockSize());
            addblock(indexfile, o2block[0]);
            count = 0; 
            outstream.clear();
        } 
    }
    return 1; 

}

int Table::Search(string value)
{
    // Get blockid
    int blockid = IndexSearch(value);

    // Read and Record blockid
    string buffer;
    return readblock(indexfile, blockid, buffer);
}

int Table::IndexSearch(string value)
{
    istringstream instream;
    int blockid = getfirstblock(indexfile);

    // check if blockid is -1
    while (blockid != 0)
    {
        string buffer;
        readblock(indexfile, blockid, buffer);
        
        string k;
        int b;
        
        instream.str(buffer);
        
        for (int i = 1; i <= 4; i++)
        {
            instream >> k >> b;
            if (k == value)
            {
                return b;
            }
        }

        blockid = nextblock(indexfile, blockid);
    }
    
    return 1;
}