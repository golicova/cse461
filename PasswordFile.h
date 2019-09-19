#include <iostream> 

using namespace std; 

class PasswordFile
{
    public: 
        PasswordFile (string filename); 
        void addpw (string newuser, string newpassword);
        bool checkpw (string user, string passwd);
        
    private: 
        string filename; 
        vector <string> user; 
        vector <string> password; 
        voin synch(); 
}