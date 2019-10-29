#include <iostream> 
#include <string>
#include "Filesys.h"

using namespace std; 

bool debugComponent = true;

int main()
{
    Sdisk disk1("disk1",256,128);
    Filesys fsys("disk1",256,128);
    fsys.newfile("file1");
    fsys.newfile("file2");

    string bfile1;
    string bfile2;

    for (int i=1; i<=1024; i++)
    {
        bfile1+="1";
    }

    if (debugComponent)
        cout << "Checkpoint 1!" << endl;

    vector<string> blocks = fsys.block(bfile1, 128); 

    int blocknumber=0;

    for (int i = 0; i < blocks.size(); i++)
    {
        blocknumber=fsys.addblock("file1", blocks[i]);
    }

    if (debugComponent)
        cout << "Checkpoint 2!" << endl;

    fsys.delblock("file1", fsys.getfirstblock("file1"));

    for (int i = 1; i <= 2048; i++)
    {
        bfile2 += "2";
    }

    if (debugComponent)
        cout << "Checkpoint 3!" << endl;

    blocks = fsys.block(bfile2, 128); 

    for (int i = 0; i < blocks.size(); i++)
    {
        blocknumber=fsys.addblock("file2", blocks[i]);
    }

    if (debugComponent)
        cout << "Checkpoint 4!" << endl;

    fsys.delblock("file2", blocknumber);

    if (debugComponent)
        cout << "End of Main.cpp!" << endl;

    return 0;
}