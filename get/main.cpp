//  get
//  main.cpp
//  seniordesign2017
//
//  Created by Andy Acosta on 11/7/16.
//  Copyright © 2016 Andy Acosta. All rights reserved.
//  Adapted from Thalmic Myo-Data-Capture
//

#define PI 3.1415926535897832
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <ncurses.h>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <time.h>
#include <Carbon/Carbon.h>
#include <termios.h>

#define NB_ENABLE 0
#define NB_DISABLE 1

using namespace std;

#include <myo/myo.hpp>
#include "../grt-master/GRT/GRT.h"

inline bool file_exists(string name)
{
    ifstream f(name.c_str());
    if(f.good())
    {
        f.close();
        return true;
    }
    else {
        f.close();
        return false;
    }
}

template <typename T> string NumberToString( T Number )
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

/*
 *  Helper function to get asynchronous keyboard hits 
 */

int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void nonblock(int state)
{
    struct termios ttystate;
    
    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);
    
    if (state==NB_ENABLE)
    {
        //turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
    }
    else if (state==NB_DISABLE)
    {
        //turn on canonical mode
        ttystate.c_lflag |= ICANON;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    
}

/*
 *  DataCollector Class that is derived from myo::DeviceListener.
 *  NOTE: override so that it doesn't do nothing
 */
class DataCollector : public myo::DeviceListener {
    
public:
    DataCollector()
    : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    {
        accel = vector<vector<double> >(3, vector<double>(0, 0));
        orient = vector<vector<double> >(3, vector<double>(0, 0));
        emgData = vector<vector<int> >(8, vector<int>(0,0));
        roll = 0; pitch = 0; yaw = 0;
        ax = ay = az = 0;
        currentLabel = 1;
    }
    
    /*
     *  called whenever the Myo disconnects from Myo Connect
     */
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        //Myo unconnected, reinitialize to 0/false
        roll_w = 0;
        pitch_w = 0;
        yaw_w = 0;
        onArm = false;
        isUnlocked = false;
        emgSamples.fill(0);
    }
    
    /*
     *  called to provide the accelerometer data based on three vectors (X, Y, Z)
     */
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& acceleration)
    {
        ax = acceleration[0];
        ay = acceleration[1];
        az = acceleration[2];
        
        //Accelerometer data
        accel[0].push_back(ax);
        accel[1].push_back(ay);
        accel[2].push_back(az);
    }
    
    /*
     * called to provide current orientation, represented as a unit quaternion
     */
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        
        //Euler angles (r, p, y) from unit quaternion.
        roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
        pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
        yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
        
        //cout << "[" << roll << " " << pitch << " " << yaw << "]" << endl;
        
        // Convert the floating point angles in radians to a scale from 0 to 18.
        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
        
        //Orientation data
        orient[0].push_back(roll);
        orient[1].push_back(pitch);
        orient[2].push_back(yaw);
        
    }
    
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg){
        
        for (int i = 0; i < 8; i++) {
            emgSamples[i] = emg[i];
            ostringstream oss;
            oss << static_cast<int>(emgSamples[i]);
            string emgString = oss.str();
            emgData[i].push_back(emg[i]);
            
            
            //cout << '[' << emgString << string(4 - emgString.size(), ' ') << ']';
        }
        //cout << endl;
        
    }
    
    void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
    {
        // Print out the MAC address of the armband we paired with.
        
        // The pointer address we get for a Myo is unique - in other words, it's safe to compare two Myo pointers to
        // see if they're referring to the same Myo.
        
        // Add the Myo pointer to our list of known Myo devices. This list is used to implement identifyMyo() below so
        // that we can give each Myo a nice short identifier.
        knownMyos.push_back(myo);
        
        // Now that we've added it to our list, get our short ID for it and print it out.
        std::cout << "Paired with " << identifyMyo(myo) << "." << std::endl;
    }

    
    /*
     *  called whenever the pose has changed when being worn
     *  (i.e. making fist to not making a fist anymore)
     */
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;
        
        if(pose != myo::Pose::unknown && pose != myo::Pose::rest)
        {
            myo->unlock(myo::Myo::unlockHold);
            myo->notifyUserAction();
        } else {
            myo->unlock(myo::Myo::unlockTimed);
        }
    }
    
    
    
    /*
     *  called whenever a sync gesture (wave out) has been recognized after putting Myo on arm.
     *  lets gesture[i].substring(0, gestures[i].find_first_of('_')Myo know which arm it's on and which
     *  way it's facing.
     */
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState)
    {
        onArm = true;
        whichArm = arm;
    }
    
    /*
     *  called whenever Myo is moved on forearm and no longer synced
     */
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        onArm = false;
    }
    
    /*
     *  called whenever Myo is unlock and pose events can start delivering
     */
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = true;
    }
    
    /*
     *  called whenever Myo is locked and no pose events can be sent
     */
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = false;
    }
    
    /*
     *
     *  Functions that override functions in DeviceListener
     *
     */
    
    /*
     *  called to print the current values updated by the function above.
     */
    void print()
    {
        //Clear current line
        cout << '\r';
        
        cout   << '[' << string(roll_w, '*') << string(18-roll_w, ' ') << ']'
        << '[' << string(pitch_w, '*') << string(18-pitch_w, ' ') << ']'
        << '[' << string(yaw_w, '*') << string(18-yaw_w, ' ') << ']';
        
        if(onArm)
        {
            string poseString = currentPose.toString();
            
            cout   << '[' << (isUnlocked ? "unlocked" : "locked ") << ']'
            << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
            << '[' << poseString << string(14-poseString.size(), ' ') << ']';
        } else {
            //if unknown arm and pose
            cout << '[' << string(8, ' ') << ']' << "[?]" << '[' << string(14, ' ') << ']';
        }
        
        cout << flush;
    }
    
    /*
     *  add new data to the already stored vectors
     *
    void recData()
    {
        //Accelerometer data
        accel[0].push_back(ax);
        accel[1].push_back(ay);
        accel[2].push_back(az);
        
        cout << accel.size() << endl;
        
        //Orientation data
        orient[0].push_back(roll);
        orient[1].push_back(pitch);
        orient[2].push_back(yaw);
    }
     
     */
    
    /*
     *  clear the data vectors
     */
    void clearData()
    {
        for(int i = 0; i < 3; i++)
        {
            accel[i].clear();
            orient[i].clear();
            emgData[i].clear();
        }
    }
    
    /*
     *  add the raw data to the file, given a filename
     */
    void appendData(string filename, int fno)
    {
        
        ofstream outfile;
        outfile.open(filename, ios::out | ios::app);
        cerr << "\nOpened file." << endl;
        //outfile << "Test Size," << accel[0].size() << endl;
        //outfile << "Accel X,Accel Y,Accel Z,Orient X,Orient Y,Orient Z,Emg1,Emg2,Emg3,Emg4,Emg5,Emg6,Emg7,Emg8" << endl;
        outfile << endl;
        
        for(size_t i = 0; i < accel[0].size(); i++)
        {
            int k = 0;
            outfile << fno << ",";
            for(int j = 0; j < 3; j++)
            {
                outfile << accel[j][i] << ",";
            }
            for(int j = 0; j < 3; j++)
            {
                outfile << orient[j][i] << ",";
            }
            for(int j = 0; j < 8; j++){
                outfile << emgData[j][i] << ",";
            }
            outfile << endl;
        }
        cerr << "Finished writing." << endl;
        outfile.close();
        cerr << "Closed outfile." << endl;
        clearData();
    }
    
    size_t identifyMyo(myo::Myo* myo) {
        // Walk through the list of Myo devices that we've seen pairing events for.
        for (size_t i = 0; i < knownMyos.size(); ++i) {
            // If two Myo pointers compare equal, they refer to the same Myo device.
            if (knownMyos[i] == myo) {
                return i + 1;
            }
        }
        return 0;
    }
    
    //Values for onArmSync() and onArmUnsync()
    bool onArm;
    myo::Arm whichArm;
    
    bool isUnlocked;
    
    int roll_w, pitch_w, yaw_w;
    float roll, pitch, yaw, ax, ay, az;
    vector<vector<double> > accel;
    vector<vector<double> > orient;
    vector<vector<int> > emgData;
    array<int8_t, 8> emgSamples;
    myo::Pose currentPose;
    
    std::vector<myo::Myo*> knownMyos;
    GRT::UINT currentLabel;
};



/*
 *  Given a filename, the raw gesture data will be added to a file and added to 'rawdata'
 */
void recordGesture(string gesturename, int t)
{
    cout << "Starting the recording module." << endl;
    myo::Hub hub("com.example.gesturemyo");
    myo::Myo* myo = hub.waitForMyo(10000);
    if(!myo)
    {
        throw runtime_error("Couldn't find Myo.");
    }
    
    cout << "Myo connected." << endl;
    
    myo->setStreamEmg(myo::Myo::streamEmgEnabled);
    
    DataCollector collector;
    
    
    hub.addListener(&collector);
    
    //filenumber
    int fno = 0;
    
    char c;
    char k;
    
    nonblock(NB_ENABLE);
    while(t--)
    {
        int i=0;
        int j=0;
        
        cout << "Start while." << endl;
        string fext = "../data";
        //+ "_" + NumberToString(fno)
        string filename = fext + "/" + gesturename + ".csv";
        if(file_exists(fext + "/" + gesturename + ".csv")){
            fno++;
            //filename = fext + "/" + gesturename + "_" + NumberToString(fno) + ".csv";
        }
        cout << "FNO: " << fno << endl;
        
        
        
        cout << "Press 's' when ready to start recording gesture." << endl;
        
        while(!j){
            usleep(1);
            j = kbhit();
            if(j!=0){
                k = fgetc(stdin);
                if(k=='s')
                    j = 1;
                else
                    j = 0;
            }
        }
        
        cout << "Recording gesture. Press 'q' to end recording the gesture." << endl;
        
        while(!i)
        {
            usleep(1);
            hub.run(10);
            i = kbhit();
            
            if(i!=0){
                c = fgetc(stdin);
                if(c=='q')
                    i = 1;
                else{
                    if(!collector.onArm)
                    {
                        cout << "Please sync Myo!!" << endl;
                        return;
                    }
                    collector.print();
                }
            }
        }
        
        
        cout << "Use recording? (y/n)" << endl;
        char ans;
        cin.get(ans);
        
        
        if( ans == 'y')
        {
            collector.appendData(filename, fno);
            cout << "Written as " << filename << endl;
        }
        if(ans == 'n')
        {
            t++;
        }
        collector.clearData();
    }
    nonblock(NB_DISABLE);
}

string getcwd(){
    char *buffer = new char[MAXPATHLEN];
    getcwd(buffer,MAXPATHLEN);
    if(buffer != NULL){
        string ret(buffer);
        delete[] buffer;
        return ret;
    }else{
        return string();
    }
}


//Main Function to
int main(int argc, char** argv)
{
    /* Makes a directory called 'rawdata' to add data to */
    int status;
    cout << getcwd() << endl;
    status = mkdir("../data", ACCESSPERMS);
    
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
        cout << " - Adds raw data gesture files to dir 'data'." << endl;
    }
    
    try
    {
        while(1){
            cout << "Enter name of gesture: ";
            string gesturename;
            cin >> gesturename;
            if(gesturename.size() == 0)
                return 0;
            cout << "Number of times the gesture will be performed?: ";
            int t = 0;
            cin >> t;
            recordGesture(gesturename, t);
        }
    } catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        cerr << "Press enter to continue.";
        cin.ignore();
        return 1;
    }
}

