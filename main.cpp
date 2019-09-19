#include <iostream> 
#include 'PasswordFile.h' 

using namespace std; 

int main() 
{
    PasswordFile passfile("password.txt");
    passfile.addpw("dbotting","123qwe");
    passfile.addpw("egomez","qwerty");
    passfile.addpw("tongyu","liberty");
    
    //check if passwords match users
}