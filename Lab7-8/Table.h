#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Filesys.h"

class Table : public Filesys
{
    public :
        Table(string diskname,int blocksize,int numberofblocks, string flatfile, string indexfile);
        int BuildTable(string input_file);
        int Search(string value);
    private :
        string flatfile;
        string indexfile;
        int numberofrecords;
        Filesys filesystem;
        int IndexSearch(string value);
};

Table::Table(string diskname, int blocksize, int numberofblocks, string flatfile, string indexfile):Filesys(diskname, numberofblocks, blocksize)
{
    cout << "Table created" << endl;
}

int Table::BuildTable(string input_file)
{
    return 1;
}

int Table::Search(string value)
{
    return 1;
}

int Table::IndexSearch(string value)
{
    return 1;
}