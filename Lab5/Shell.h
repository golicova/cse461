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
};

Shell::Shell(string filename, int blocksize, int numberofblocks) 
{
    //this->filename.push_back(filename); 
    this->blockSize = blockSize;
    //this->numberofblocks = numberofblocks;
    cout << "Shell created succesfully." << '\n';
}

int Shell::dir()
{ 
    vector<string> flist = ls();
    for (int i = 0; i < flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
}

int Shell::add(string file)
{
    string buffer;
	for (int i = 0; i < this->blockSize; i++)   // blockSize = 128
	{
		buffer += '#';
	}

    /*
    char x;
    cin.get(x);
    
    while (x != '~') 
    {
        buffer += x;
        cin.get(x);
    }
    */
   
    newfile(file);
    vector<string> blocked = block(buffer, this->blockSize);    // blockSize = 128
    
    for (int i = 0; i < blocked.size(); i++) 
    {
        addblock(file, blocked[i]);
    }
}

int Shell::del(string file)
{
    while (getfirstblock(file) > 0) 
    {
        delblock(file, getfirstblock(file));
    }

    rmfile(file);
}

int Shell::type(string file)
{
    string temp1;
    string temp2;
    int next = getfirstblock(file);
    readblock(file, next, temp1);
    int last = nextblock(file, next);

    while (last != 0) 
    {
        readblock(file, last, temp2);
        temp1 += temp2;
        last = nextblock(file, last);
    }
    cout << temp1 << '\n';
}

int Shell::copy(string file1, string file2)   // does not accept files 
{
    int code1 = getfirstblock (file1);
    int iblock = code1;

    if (code1 == 1) 
    {
        cout << "No such file." << endl; 
        return 0; 
    }
    
    int code2 = getfirstblock(file2); 
    if (code2 != 1)
    {
        cout << "File 2 already exists." << endl; 
        return 0; 
    }
    
    int code3 = newfile(file2); 
    if (code3 == 0)
    {
        cout << "No space in ROOT." << endl; 
        return 0; 
    }

    while (iblock != 0)
    {
        string b; 
        getblock(b, file1);               // fix buffer parameter
        int code4 = addblock(file2, b); 
        if (code4 == -1)
        {
            cout << "No space left." << endl; 
            delete(file2); 
            return 0; 
        }
        
        iblock = nextblock (file1, iblock); 
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