#include <iostream> 
#include <fstream> 
#include <vector>

using namespace std; 

class PasswordFile
{
    public: 
        PasswordFile(string filename);                     // opens the file and reads the names/passwords in the vectors user and password.
        void addpw (string newuser, string newpassword);    //this adds a new user/password to the vectors and writes the vectors to the file filename
        void deleteUser (string oldUser);                     // deletes existing user and password
        bool checkpw (string user, string passwd);          // returns true if user exists and password matches
        
    private: 
        string filename;                                    // the file that contains password information
        vector <string> user;                               // the list of usernames
        vector <string> password;                           // the list of passwords
        void synch();                                       // writes the user/password vectors to the password file
};

PasswordFile::PasswordFile(string filename)
{
    ifstream infile; 
    string u, p; 
    infile >> u >> p; 
    while (infile.good())
    {
        user.push_back(u);
        password.push_back(p);
        infile >> u >> p; 
    }
    infile.close();   
}

void PasswordFile::addpw (string newuser, string newpassword)
{
    user.push_back(newuser);            // Add newuser
    password.push_back(newpassword);    // Add newpassword
}

bool PasswordFile::checkpw (string user, string passwd)
{
    // Iterate through each user (should be parallel with password vector)
    for (int i = 0; i < PasswordFile::user.size; i++)
    {
        if (PasswordFile::user[i] == user)
        {
            if (PasswordFile::password[i] == passwd)
            {
                return true;
            }
        }
    }

    return false;
}

void PasswordFile::synch()
{
    
}

void PasswordFile::deleteUser (string oldUser)
{
    
}
