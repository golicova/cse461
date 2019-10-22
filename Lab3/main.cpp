#include <iostream> 
#include <string>
#include "Filesys.h"
#include "Sdisk.h"

using namespace std; 

int main()
{
    // Submit Lab 4
    Filesys fsys = Filesys("disk.txt", 128, 256);   // Create or find filesystem in the disk
    Sdisk disk("disk.txt", 128, 256);               // Create or Find disk

    
}