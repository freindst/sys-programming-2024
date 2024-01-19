#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
using namespace std;

int main ( )
{
    ifstream inputfile;
    string filename;
    string words;
    
    //Get the filename from the user.
    cout << "Enter the filename: ";
    cin >> filename;
    
    //Open the input file.
    inputfile.open(filename.c_str());
    
    //If the file succesfully opened, process it.
    if (inputfile)
    {
        while (inputfile >> words)
            cout << words.length() << endl;
        
        //Clost the file.
        inputfile.close();
    }
    else
    {
        //Display an error message.
        cout << "Error opening the file.\n";
    }
    
    return 0;
}