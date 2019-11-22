/*
Name
CSE 461
Lab 5 + 6
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Filesys.h"

class Shell: public Filesys
{
    public :
        Shell(string filename, int blocksize, int numberofblocks);
        int dir();// lists all files
        int add(string file);// add a new file using input from the keyboard
        int del(string file);// deletes the file
        int type(string file);//lists the contents of file
        int copy(string file1, string file2);//copies file1 to file2
        string getFile(string filename);
        int clobber(string file);
};

Shell::Shell(string diskname, int numberofblocks, int blocksize): Filesys(diskname, numberofblocks, blocksize)
{
    cout << "Shell created succesfully." << '\n';
}

int Shell::dir()
{ 
    vector<string> flist = ls();
    for (int i = 0; i < flist.size(); i++)
    {
        cout << flist[i] << endl;
    }

    return 1;
}

int Shell::add(string file)
{
    newfile(file);

    string buffer;
	for (int i = 0; i < 128; i++)   // blockSize = 128
	{
		buffer += '#';
	}

    char x;
    cin.get(x);
    
    while (x != '~') 
    {
        buffer += x;
        cin.get(x);
    }
    
    vector<string> blocked = block(buffer, 128);    // blockSize = 128
    
    for (int i = 0; i < blocked.size(); i++) 
    {
        addblock(file, blocked[i]);
    }

    return 1;
}

int Shell::del(string file)
{
    while (getfirstblock(file) > 0) 
    {
        delblock(file, getfirstblock(file));
    }

    rmfile(file);
    return 1; 
}

int Shell::type(string file)
{
	int block = getfirstblock(file);
	string buffer;
	while (block > 0)
	{
		string t;
		int error = readblock(file, block, t);
		buffer += t;
		block = nextblock(file, block);
	}
	cout << buffer << endl;
	cout << buffer.length() << endl;
	return 1;
}

int Shell::copy(string file1, string file2)
{
    int code = getfirstblock(file1);
    if (code == -1)
    {
        cout << "file does not exist";
        return -1;
    }
    int code2 = getfirstblock(file2);
    if (code2 != -1)
    {
        cout << "file2 already exists";
        return -1;
    }
    int code3 = newfile(file2);
    if (code3 == 0)
    {
        cout << "no space on root";
        return -1;
    }
    
    int iblock = code; //iblock is firstblock
    while (iblock != 0)
    {
       string b;
       readblock(file1, iblock, b);
       int code4 = addblock(file2, b);
       if (code4 == -1)
       {
           cout << "no space left";
           del(file2);
           return -1;
       }
       iblock = nextblock(file1, iblock);
       
    }

    return 1;
}

string Shell::getFile(string filename)
{
    string buffer; 
    int iblock = getfirstblock(filename);

    if (iblock == -1)
    {
        cout << "No such file!" << endl;
        return 0;
    }

    while (iblock != 0)
    {
        int b;
        readblock(filename, b, buffer);
        buffer += b;
        iblock = nextblock(filename, iblock);
    }

    return buffer;
}

int Shell::clobber(string file)
{
    int iblock = getfirstblock(file);

    // Non-recursive
    /*
    if (iblock == -1)
    {
        cout << "no such file";
        return 0;
    }

    while(iblock != 0)
    {
        delblock(file, iblock);
        int iblock2 = nextblock(file, iblock);
        iblock = iblock2;
    }
    */

    // Recursive
    if (iblock == -1)
    {
        cout << "no such file";
        return 0;
    }
    else
    {
        if (iblock != 0)
        {
            delblock(file, iblock);
            int iblock2 = nextblock(file, iblock);
            iblock = iblock2;
            clobber(file);
        }
    }
    
    rmfile(file);   

    return 1;
}