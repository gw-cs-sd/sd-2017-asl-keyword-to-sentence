/*
 *  Andy Acosta
 *  Senior Design 2017
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <myo/myo.hpp>

using namespace std;

/*
 *
 *  Given a filename, the raw gesture data will be added to a file and added to 'rawdata'
 *
 */
void recordGesture(string gesturename, int t)
{
    //myo::
}


//Main Function to
int main(int argc, char** argv)
{
    /* Makes a directory called 'rawdata' to add data to */
    int status;
    status = mkdir("/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/rfiles/rawdata", ACCESSPERMS);
    
    /*
     *  Testing to see if the directory was created.
     *
    if(status){
        cout << "Error making directory or might already exist.\n";
    }
    else
        cout << "Directory Created!\n";
     */
    
    
    if(argc > 1)
    {
        cout << "Data collector to train the Myo Armband new Gestures" << endl;
        cout << " - Adds raw data gesture files to dir 'rawdata'." << endl;
    }
    
    try
    {
        while(1){
            std::cout << "Enter name of gesture: ";
            string gesturename;
            cin >> gesturename;
            if(gesturename.size() == 0)
                return 0;
            std::cout << "Number of times the gesture will be performed?: ";
            int t = 0;
            cin >> t;
            recordGesture(gesturename, t);
        }
    } catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        return 1;
    }
}
