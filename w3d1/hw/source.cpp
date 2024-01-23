// This program counts the number of words in files selected by the user.
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main ( )
{
    ifstream inputfile;
    string filename;
    string words;
    int count;
    count = 0;
    
    //Get the filename from the user.
    cout << "Enter the filename: ";
    cin >> filename;
    
    //Open the input file.
    inputfile.open(filename.c_str());
    
    //If the file successfully opened, process it.
    if (inputfile)
    {
        while (inputfile >> words){
            count++;
        }
            //cout << words << endl;
        
        //Close the file.
        inputfile.close();
        cout << "There are " << count << " words in the file " << filename << "." <<endl;
    }
    else
    {
        //Display an error message.
        cout << "Error opening the file.\n";
    }
    
    return 0;
}