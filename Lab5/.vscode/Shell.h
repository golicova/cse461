#include <iostream>
#include <string>
#include <algorithm>

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

int Shell::dir()
{

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
    
}