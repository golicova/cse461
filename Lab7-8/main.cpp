/*
Name
CSE 461
Lab 5 + 6
*/

#include <iostream>
#include <string>
#include "Shell.h"
#include "Table.h"

using namespace std;

int main()
{
    //
    //This main program inputs commands to the shell.
    //It inputs commands as : command op1 op2
    //You should modify it to work for your implementation.
    //
    Sdisk sdisk=Sdisk("sdisk.txt", 256, 128);
    Filesys fsys = Filesys("sdisk.txt", 256, 128);
    Shell* shell=Shell("sdisk.txt", 256, 128);
    Table* table = Table("sdisk.txt", 256, 128);
    
    table.BuildTable("data.txt");

    string s;
    string command="go";
    string op1,op2;
    
    while (command != "quit")
    {
        command.clear();
        op1.clear();
        op2.clear();
        cout << "$";
        getline(cin, s);

        int firstblank = s.find(' ');
        if (firstblank < s.length()) 
            s[firstblank] = '#';

        int secondblank = s.find(' ');
        command = s.substr(0, firstblank);

        if (firstblank < s.length())
            op1 = s.substr(firstblank + 1, secondblank - firstblank - 1);

        if (secondblank < s.length())
            op2 = s.substr(secondblank + 1);

        if (command == "dir")
        {
            // use the ls function
            shell->dir();
        }
        if (command == "add")
        {
            // The variable op1 is the new file
            shell->add(op1);
        }
        if (command == "del")
        {
            // The variable op1 is the file
            shell->del(op1);
        }
        if (command == "type")
        {
            // The variable op1 is the file
            shell->type(op1);
        }
        if (command == "copy")
        {
            // The variable op1 is the source file and the variable op2 is the destination file.
            shell->copy(op1, op2);
        }
        if (command == "clobber")
        {
            shell->clobber(op1);
        }  
    }
    return 0;
}