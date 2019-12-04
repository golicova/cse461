
#ifndef TABLE_H
#define TABLE_H

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
        //Filesys filesystem;
        int IndexSearch(string value);
};

Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile):Filesys(diskname, numberofblocks, blocksize)
{
    //ifstream infile;
    //infile.open(diskname.c_str);
    
    // Create flatfile in filesys
    newfile(flatfile);
    this->flatfile = flatfile;
    

    // Create indexfile in filesys
    newfile(indexfile);
    this->indexfile = indexfile;
    

    cout << "Table created" << endl;
}

int Table::BuildTable(string input_file)
{
    ifstream infile; 
    infile.open(input_file.c_str());

    string record; 
    int count = 0; 
    getline(infile, record); 
    vector<string> key; 
    vector<string> iblock; 
    ostringstream outstream;

    while(getline(infile, record))
    {
        cout << "Table Checkpoint 0!" << endl;
        //cout << "record: " << record << endl;
        string pKey = record.substr(0,5); 
        vector<string> oblock = block(record, getBlockSize());
        cout << "oblock[0]: " << oblock[0] << endl;
        int blockid = addblock (flatfile, oblock[0]); // Fuck you!!!
        cout << "blockid: " << blockid << endl;
        outstream << pKey << " " << blockid << " "; 
        count++; 

        if(count == 4)
        {
            cout << "Table Checkpoint 1!" << endl;
            vector<string> o2block = block(outstream.str(), getBlockSize());
            addblock(indexfile, o2block[0]);
            count = 0; 
            outstream.str("");
            cout << "Table Checkpoint 2!" << endl;
        } 
        //getline(infile, record); 
        cout << "record: " << record << endl;
        cout << "Table Checkpoint 3!" << endl;
    }
    return 1; 

}

int Table::Search(string value)
{
    // Get blockid
    int blockid = IndexSearch(value);

    if (blockid == -1)
    {
        return 0;
    }

    // Read and Record blockid
    string buffer;
    readblock(indexfile, blockid, buffer);
    cout << buffer;

    return 1;
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

#endif