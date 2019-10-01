#include <stdlib.h>
#include <iostream> 
#include "PasswordFile.h"

using namespace std; 

int main() 
{
    PasswordFile passfile("password.txt");

    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    cout << endl; 

    passfile.addpw("dbotting","123qwe");
    passfile.addpw("egomez","qwerty");
    passfile.addpw("tongyu","liberty");

    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    cout << endl;

    // right password 
    cout << "Password for egomez : " << passfile.checkpw("egomez", "qwerty");
    cout << endl;
    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    cout << endl; 

    // wrong password 
    cout << "Password for egomez : " << passfile.checkpw("egomez", "qwertyyy"); 
    cout << endl;

    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    cout << endl; 

    passfile.deleteUser("egomez");

    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    cout << endl; 

    passfile.deleteUser("tong");

    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    cout << endl; 
}
