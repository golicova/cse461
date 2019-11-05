#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Filesys.h"

class Shell: public Filesys
{
    public :
        Shell(string filename, int blocksize, int numberofblocks);
        int dir();                                                              // lists all files
        int add(string file);// add a new file using input from the keyboard
        int del(string file);// deletes the file
        int type(string file);//lists the contents of file
        int copy(string file1, string file2);                                   //copies file1 to file2
};

Shell::Shell() {}

Shell::Shell(string filename, int blocksize, int numberofblocks) 
{

}

// dir lists files in the class Shell
// Prototype: int Shell::dir() 

int Shell::dir()
{ 
    vector<string> flist = ls();
    for (i = 0; i < flist.size(); i++)
    {
        cout << flist[i] << endl;
    }
}

int Shell::add(string file)
{

}

int Shell::del(string file)
{

}

int Shell::type(string file)
{

}

int Shell::copy(string file1, string file2)
{
<<<<<<< Updated upstream
    int getFirstBlock (file1); 
    if (code == 1) 
    {
        cout << "No such file." << endl; 
        return 0; 
    }
    
    int code2 = getFirstBlock(file2); 
    if (code2 != 1)
    {
        cout << "File 2 already exists." << endl; 
        return 0; 
    }
    
    int code3 = newFile(file2); 
    if (code3 == 0)
    {
        cout << "No space in ROOT." << endl; 
        return 0; 
    }
    
    int iblock = code;
    while (iblock != 0)
    {
        sting b; 
        getBlock(file1, b);
        int code4 = addBlock(file2, b); 
        if (code4 == -1)
        {
            cout << "No space left." << endl; 
            delete(file2); 
            return 0; 
        }
        
        iblock = nextBlock (file1, iblock); 
    }
    return 1; 
}
=======

}
>>>>>>> Stashed changes
