//
//  getraw.cpp
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
#include <curses.h>
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
#include <atomic>
#include <thread>



using namespace std;

#include <myo/myo.hpp>

/*
 *  boolean to continue or end while loop depending on when a user presses a key
 */
atomic_bool keyIsPressed(false);

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
        roll = 0; pitch = 0; yaw = 0;
        ax = ay = az = 0;
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
    }
    
    /*
     *  called to provide the accelerometer data based on three vectors (X, Y, Z)
     */
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& acceleration)
    {
        ax = acceleration[0];
        ay = acceleration[1];
        az = acceleration[2];
    }
    
    /*
     * called to provide current orientation, represented as a unit quaternion
     */
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        //Euler angles (r, p, w) from unit quaternion.
        roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
        pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
        yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
        
        cout << roll << " " << pitch << " " << yaw << endl;
        
        // Convert the floating point angles in radians to a scale from 0 to 18.
        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
    }
    
    /*
     *  called whenever whenever the pose has changed when being worn 
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
        std::cout << '\r';
        
        std::cout   << '[' << std::string(roll_w, '*') << std::string(18-roll_w, ' ') << ']'
                    << '[' << std::string(pitch_w, '*') << std::string(18-pitch_w, ' ') << ']'
                    << '[' << std::string(yaw_w, '*') << std::string(18-yaw_w, ' ') << ']';
        
        if(onArm)
        {
            std::string poseString = currentPose.toString();
            
            std::cout   << '[' << (isUnlocked ? "unlocked" : "locked ") << ']'
                        << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
                        << '[' << poseString << std::string(14-poseString.size(), ' ') << ']';
        } else {
            //if unknown arm and pose
            std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
        }
        
        std::cout << std::flush;
    }
    
    /*
     *  add new data to the already stored vectors
     */
    void recData()
    {
        //Accelerometer data
        accel[0].push_back(ax);
        accel[1].push_back(ay);
        accel[2].push_back(az);
        
        //Orientation data
        orient[0].push_back(roll);
        orient[1].push_back(pitch);
        orient[2].push_back(yaw);
    }
    
    /*
     *  clear the data vectors
     */
    void clearData()
    {
        for(int i = 0; i < 3; i++)
        {
            accel[i].clear();
            orient[i].clear();
        }
    }
    
    /*
     *  add the raw data to the file, given a filename
     */
    void appendData(string filename)
    {
        std::ofstream outfile(filename);
        cerr << "Opened file" << endl;
        outfile << endl;
        outfile << accel[0].size() << endl;
        for(size_t i = 0; i < accel[0].size(); i++)
        {
            for(int j = 0; j < 3; j++)
            {
                outfile << accel[j][i] << " ";
            }
            for(int j = 0; j < 3; j++)
            {
                outfile << orient[j][i] << " ";
            }
            outfile << endl;
        }
        cerr << "Finished writing." << endl;
        outfile.close();
        cerr << "Closed outfile." << endl;
    }
    
    //Values for onArmSync() and onArmUnsync()
    bool onArm;
    myo::Arm whichArm;
    
    bool isUnlocked;
    
    int roll_w, pitch_w, yaw_w;
    float roll, pitch, yaw, ax, ay, az;
    vector<vector<double> > accel;
    vector<vector<double> > orient;
    myo::Pose currentPose;
};

/*
 *  Loop function for keyIsPressed
 */
void recordGestureLoop(myo::Hub hub)
{
    while(!keyIsPressed)
    {
        hub.run(10);
        
        if(!collector.onArm)
        {
            cout << "Please sync Myo!!" << endl;
            return;
        }
        collector.print();
        collector.recData();
    }
}


/*
 *  Given a filename, the raw gesture data will be added to a file and added to 'rawdata'
 */
void recordGesture(string gesturename, int t)
{
    myo::Hub hub("com.example.gesturemyo");
    myo::Myo* myo = hub.waitForMyo(10000);
    if(!myo)
    {
        throw std::runtime_error("Couldn't find Myo.");
    }
    
    DataCollector collector;
    
    hub.addListener(&collector);
    int fno = 0;
    //For 'keyIsPressed'
    thread loopThread = thread(loopFunction);
    
    while(t--)
    {
        while(file_exists("rawdata\\" + gesturename + "_" + NumberToString(fno) + ".rawmyo"))
            fno++;
        string filename = "rawdata\\" + gesturename + "_" + NumberToString(fno) + ".rawmyo";
        std::cout << "Press s when ready to start recording gesture." << endl;
        
        
        
        //while(getch() != 's')
        {
            
        }
        
        std::cout << "Recording gesture. Press 'x' to end recording the gesture." << std::endl;
        
        /*while(!keyIsPressed)
        {
            hub.run(10);
            
            if(!collector.onArm)
            {
                cout << "Please sync Myo!!" << endl;
                return;
            }
            collector.print();
            collector.recData();
            if(getch() == 'x') break;
        }*/
        
        //For 'keyIsPressed'
        thread loopThread = thread(recordGestureLoop(hub));
        
        #ifdef _WIN32 || _WIN64
            system("pause");
        #else
            system("read -n1");
        #endif
        
        keyIsPressed = true;
        loopThread.join();
        
        std::cout << "Use recording? (y/n)" << std::endl;
        char ans;
        while((ans = getch()) != 'n' && ans != 'y');
        
        if( ans == 'y')
        {
            collector.appendData(filename);
            cout << "Written as " << filename << endl;
        }
        collector.clearData();
    }
}


//Main Function to
int main(int argc, char** argv)
{
    
    /* Makes a directory called 'rawdata' to add data to */
    int status;
    status = mkdir("/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/myo/sdk/project/rawdata", ACCESSPERMS);
    
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

