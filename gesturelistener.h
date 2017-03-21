//
//  gesturelistener.h
//  myoSign
//
//  Created by Andy Acosta on 3/7/17.
//  Copyright © 2017 Andy Acosta. All rights reserved.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <map>

#include <myo/myo.hpp>
#include "grt-master/GRT/GRT.h"

using namespace GRT;

template <typename T> string NumberToString( T Number )
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

class gesturelistener : public myo::DeviceListener {
    
    DTW dtw1;
    
public:
    
    gesturelistener(string classnames, string datamodel) : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose(){
        //emgSamples = vector<double>(0,0);
        emgSamples = array<int, 8>();
        roll = 0; pitch = 0; yaw = 0;
        ax = ay = az = 0;
        //gesturenames.push_back("null");
        gestureIdName[0] = "null";
        //gesturenames.push_back("1");
        //gesturenames.push_back("2");
        
        std::ofstream ofsClear;
        ofsClear.open(classnames, std::ofstream::out | std::ofstream::trunc);
        ofsClear.close();
        
        ifstream infile( classnames );
        std::cout << classnames << std::endl;
        
        infile.clear();
        
        int gestureID;
        while (infile)
        {
            std::string s;
            if (!getline( infile, s )) break;
            
            std::istringstream ss( s );
            int col = 0;
            while (ss)
            {
                
                std::string s;
                
                if (!getline( ss, s, ',' ))
                    break;
                int z;
                if(col == 0){
                    z = std::stoi(s);
                }
                if(col == 1){
                    gestureIdName[z] = s;
                    std::cout << gestureIdName[z] << std::endl;
                }
                col++;
                
            }
        }
        
        if (!infile.eof())
        {
            std::cerr << "Error with file!\n";
        }
        
        dtw1.enableTrimTrainingData(true, 0.1, 90);
        
        //Load DTW model from file
        if(!dtw1.loadModelFromFile(datamodel)){
            cerr << "Could not load the classifier model.\n";
            exit(EXIT_FAILURE);
        }
        else{
            std::cout << "Loaded the classifier model.\n";
        }
        
        std::cout << "Device listener created!!" << std::endl;

    }
    
    void onUnpair(myo::Myo* myo, uint64_t timestamp){
        roll_w = 0;
        pitch_w = 0;
        yaw_w = 0;
        onArm = false;
        isUnlocked = false;
        emgSamples.fill(0);
    }
    
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& acceleration){
        ax = acceleration[0];
        ay = acceleration[1];
        az = acceleration[2];
    }
    
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat){
        //Euler angles (r, p, y) from unit quaternion.
        roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
        pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
        yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
        
        //cout << "[" << roll << " " << pitch << " " << yaw << "]" << endl;
        
        // Convert the floating point angles in radians to a scale from 0 to 18.
        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
    }
    
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg){
        //emgSamples.clear();
        for(int i = 0; i < 8; i++){
            
            int x = static_cast<double>(emg[i]);
            emgSamples[i] = x;
            ostringstream oss;
            oss << static_cast<int>(emg[i]);
            string emgString = oss.str();
            //cout << '[' << emgString << string(4 - emgString.size(), ' ') << ']';
        }
        //cout << endl;
    }
    
    void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
    {
        
    }
    
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
    
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState)
    {
        onArm = true;
        whichArm = arm;
    }
    
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        onArm = false;
    }
    
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = true;
    }
    
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = false;
    }

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
    
    void onGesture(double confidence, string gesturename) {
        cout << gesturename << endl;
    }
    
    void clearData() {
        for(int i = 0; i < 6; i++) {
            data[i].clear();
        }
    }
    
    
    string recData(){
        
        vector<double> temp;
        temp.push_back(ax);
        temp.push_back(ay);
        temp.push_back(az);
        temp.push_back(roll);
        temp.push_back(pitch);
        temp.push_back(yaw);
        
        temp.push_back(emgSamples[0]);
        temp.push_back(emgSamples[1]);
        temp.push_back(emgSamples[2]);
        temp.push_back(emgSamples[3]);
        temp.push_back(emgSamples[4]);
        temp.push_back(emgSamples[5]);
        temp.push_back(emgSamples[6]);
        temp.push_back(emgSamples[7]);
        
        data.push_back(temp);
        
        int bufferSize = 200;
        if(data.size() > bufferSize+1){
            MatrixDouble window;
            for(int i = 0; i < bufferSize; i++){
                VectorDouble c;
                for(int j = 0; j < 14; j++){
                    //c.push_back(data[data.size()-bufferSize+1][j]);
                    c.push_back(data[i][j]);
                }
                window.push_back(c);
            }
            if(!dtw1.predict(window)){
                cerr << "Could not form a prediction." << endl;
            }
            
            UINT predictedClassLabel = dtw1.getPredictedClassLabel();
            double maxLikelihood = dtw1.getMaximumLikelihood();
            
            if(predictedClassLabel){
                cout << "PREDICTED CLASS LABEL:" + gestureIdName[predictedClassLabel] << endl;
                //onGesture(maxLikelihood, gesturenames[predictedClassLabel]);
                temp.clear();
                data.clear();
                return gestureIdName[predictedClassLabel];
            }
        }
        return "";
    }
    
    vector<string> gesturenames;
    std::map<int, string> gestureIdName;
    vector<vector<double > > data;
    bool onArm;
    myo::Arm whichArm;
    bool isUnlocked;
    int roll_w, pitch_w, yaw_w;
    float roll, pitch, yaw, ax, ay, az;
    
    myo::Pose currentPose;
    //vector<double> emgSamples;
    array<int, 8> emgSamples;
    
    
};
