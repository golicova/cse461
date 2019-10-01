#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <algorithm>

using namespace std; 

class PasswordFile
{
    public: 
        PasswordFile(string filename);                      // opens the file and reads the names/passwords in the vectors user and password.
        void addpw (string newuser, string newpassword);    //this adds a new user/password to the vectors and writes the vectors to the file filename
        void deleteUser (string oldUser);                   // deletes existing user and password
        bool checkpw (string user, string passwd);          // returns true if user exists and password matches
        static void newsalt(int ns);
        
    private: 
        bool encryptPasswords;                             // Turn true to encrypt string onto filename
        static int salt;
        string filename;                                    // the file that contains password information
        vector <string> user;                               // the list of usernames
        vector <string> password;                           // the list of passwords
        void synch();                                       // writes the user/password vectors to the password file
        string encrypt(string s);
};

PasswordFile::PasswordFile(string filename)
{   
    this->filename = filename;
    string u, p; 
    ifstream passwordFile;
    passwordFile.open(this->filename.c_str());
    while (passwordFile.good())
    {
        user.push_back(u);
        password.push_back(p);
        passwordFile >> u >> p; 
    }
}

void PasswordFile::addpw (string newuser, string newpassword)
{
    encryptPasswords = true;
    
    // Declaring iterator to a vector 
    vector<string>::iterator it = user.begin(); 
    
    it = find(user.begin(), user.end(), newuser);

    // Check if user already exist, if so update only password
    if(it == user.end()) 
    {
        user.push_back(newuser);            // Add newuser
        password.push_back(newpassword);    // Add newpassword
        synch(); 
    }
    else
    {
        cout << "User already exists." << endl; 
    }    
}

bool PasswordFile::checkpw (string user, string passwd)
{
    // Iterate through each user (should be parallel with password vector)
    for (int i = 0; i < PasswordFile::user.size(); i++)
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
    ofstream outFile;
    outFile.open(filename.c_str());
    for(int i = 0; i < user.size(); i++)
    {
        if (encryptPasswords)
            outFile << user[i] << " " << encrypt(password[i]) << endl;
        else
            outFile << user[i] << " " << password[i] << endl;
    }
    outFile.close();
}

void PasswordFile::deleteUser (string oldUser)
{
    int i = 0; 
    while (i < user.size())
    {
        if(user[i] == oldUser)
        {
            // Overrite user[i] and password[i] values with last element value
            user[i] = user[user.size() - 1];
            password[i] = password[password.size() - 1];

            // Delete the last element of the vectors
            user.pop_back(); 
            password.pop_back();
            
            break;
        }
        else 
        {
             i++;
        }
    }

    if (i >= user.size())
        cout << "Could not find " << oldUser << " in " << filename << endl;
    
    synch();
}

// encryption 

int PasswordFile::salt = 1;

 void PasswordFile::newsalt(int ns)
 { 
    salt = ns;
 }
  
string PasswordFile::encrypt(string s)
{ 
    for (int i=0; i<s.size(); i++)
    { 
      s[i]+=salt;
    }
    return s;
}
