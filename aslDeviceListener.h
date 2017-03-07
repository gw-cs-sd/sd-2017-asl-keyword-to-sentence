//
//  newDeviceListener.hpp
//  sdproject
//
//  Created by Andy Acosta on 11/29/16.
//  Copyright © 2016 Andy Acosta. All rights reserved.
//


#include <sys/types.h>
#include <sys/stat.h>
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

#include <myo/myo.hpp>
#include </Users/aacosta/Documents/seniordesign/update/myoSign/grt-master/GRT/GRT.h>

using namespace GRT;

class aslDeviceListener : public myo::DeviceListener {
    
public:
    
    void onAccelerometerData2(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& acceleration) {}
    void onOrientationData2(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {}
    void onPose2(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {}
    void onUnpair2(myo::Myo* myo, uint64_t timestamp) {}
    void onArmSync2(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState){}
    void onArmUnsync2(myo::Myo* myo, uint64_t timestamp) {}
    void onUnlock2(myo::Myo* myo, uint64_t timestamp) {}
    void onLock2(myo::Myo* myo, uint64_t timestamp) {}
    void onConnect2(myo::Myo *myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {}
    void onDisconnect2(myo::Myo *myo, uint64_t timestamp) {}
    void onGyroscopeData2(myo::Myo *myo, uint64_t timestamp, const myo::Vector3< float > &gyro) {}
    void onRssi2(myo::Myo *myo, uint64_t timestamp, int8_t rssi) {}
    void onBatteryLevelReceived2(myo::Myo *myo, uint64_t timestamp, uint8_t level) {}
    void onEmgData2(myo::Myo *myo, uint64_t timestamp, const int8_t *emg) {}
    void onWarmupCompleted2(myo::Myo *myo, uint64_t timestamp, myo::WarmupResult warmupResult) {}
    
    
    vector<double> emgSamples;
    
    void onConnect(myo::Myo *myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) {
        onConnect2(myo, timestamp, firmwareVersion);
    }
    void onDisonnect(myo::Myo *myo, uint64_t timestamp) {
        onDisconnect2(myo, timestamp);
    }
    void onGyroscopeData(myo::Myo *myo, uint64_t timestamp, const myo::Vector3< float > &gyro) {
        onGyroscopeData2(myo,timestamp, gyro);
    }
    void onRssi(myo::Myo *myo, uint64_t timestamp, int8_t rssi) {
        onRssi2(myo, timestamp, rssi);
    }
    void onBatteryLevelReceived(myo::Myo *myo, uint64_t timestamp, uint8_t level) {
        onBatteryLevelReceived2(myo, timestamp, level);
    }
    void onEmgData(myo::Myo *myo, uint64_t timestamp, const int8_t *emg) {
        for(int i = 0; i < 8; i++){
            emgSamples.push_back(emg[i]);
        }
        onEmgData2(myo, timestamp, emg);
    }
    void onWarmupCompleted(myo::Myo *myo, uint64_t timestamp, myo::WarmupResult warmupResult) {
        onWarmupCompleted2(myo, timestamp, warmupResult);
    }
    
    void onGesture(double confidence, string gesturename) {
        cout << confidence << " " << gesturename << endl;
    }
    
    bool onArm;
    myo::Arm whichArm;
    
    bool isUnlocked;
    
    int roll_w, pitch_w, yaw_w;
    float roll, pitch, yaw;
    
    float ax, ay, az;
    
    int maxBuffSize;
    vector<vector<double > > data;
    
    myo::Pose currentPose;
    
    double gx, gy, gz;
    int totsync = 100;
    int sync; 
    
    DTW dtw;
    
    vector<string> gesturenames;
    
    aslDeviceListener(string datamodel) : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    {
        data = vector<vector<double > >(0, vector<double>(0,0));
        roll = 0;
        pitch = 0;
        yaw = 0;
        maxBuffSize = 100;
        gx = gy = gz = 0;
        ax = ay = az = 0;
        sync = totsync;
        
        gesturenames.push_back("null");
        
        /*ifstream classNames(datalabels);
        int i;
        string s;
        while(classNames >> i){
            classNames >> s;
            gesturenames.push_back(s);
        }*/
        gesturenames.push_back("A");
        gesturenames.push_back("B");
        
        /*
         * Sets if the training data should be trimmed before training the DTW templates.
         * If set to true then any training samples that have very little movement at the start or end of
         *  a file will be trimmed so as to remove the none movement at the start or end of the file.
         * This can be useful if some of your examples have a section were the user paused before
         *  performing a gesture, or were the recording continued for a few seconds after the gesture ended.
         * The trimThreshold and maximumTrimPrecentage parameters control how extreme the
         *  trimming can be.
         * The trimThreshold should be in a range of [0 1] and the maximumTrimPrecentage should be in a range of [0 100].
         * If the amount of energy in the timeseries is below the trimThreshold then it will be trimmed.
         */
        dtw.enableTrimTrainingData(true, 0.1, 90);
        
        //Load DTW model from file
        if(!dtw.loadModelFromFile(datamodel)){
            cerr << "Could not load the classifier model.\n";
            exit(EXIT_FAILURE);
        }
        else{
            cout << "Loaded the classifier model.\n";
        }
        std::cout << "Device Listener created." << endl;
        
    }
    
    void recData(){
        /*vector<double> temp;
        temp.push_back(ax);
        temp.push_back(ay);
        temp.push_back(az);
        temp.push_back(roll);
        temp.push_back(pitch);
        temp.push_back(yaw);
        for(int i = 0; i < 8; i++){
            temp.push_back(emgSamples[i]);
        }
        data.push_back(temp);
        
        int bufferSize = 100;
        if(data.size() > bufferSize+1){
            MatrixDouble window;
            for(int i = 0; i , bufferSize; i++){
                VectorDouble c;
                for(int j = 0; j < 3; j++){
                    c.push_back(data[data.size()-bufferSize+1][j]);
                }
                window.push_back(c);
            }
            if(!dtw.predict(window)){
                cerr << "Could not form a prediction." << endl;
            }
            
            UINT predictedClassLabel = dtw.getPredictedClassLabel();
            double maxLikelihood = dtw.getMaximumLikelihood();
            
            if(predictedClassLabel)
                onGesture(maxLikelihood, gesturenames[predictedClassLabel]);
        }
         */
    }
    
    // Clear current data vectors.
    void clearData() {
        for(int i = 0; i < 6; i++) {
            data[i].clear();
        }
    }
    
    // onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        // We've lost a Myo.
        // Let's clean up some leftover state.
        roll_w = 0;
        pitch_w = 0;
        yaw_w = 0;
        ax = ay = az = 0;
        emgSamples.clear();
        onArm = false;
        isUnlocked = false;
    }
    
    // onAccelerometerData() is called whenever the Myo device provides its current acceleration, which is represented as a 3-vector.
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& acceleration) {
        ax = acceleration[0];
        ay = acceleration[1];
        az = acceleration[2];
        onAccelerometerData2(myo, timestamp, acceleration);
    }
    
    // onOrientationData() is called whenever the Myo device provides its current orientation, which is represented as a unit quaternion.
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
        roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                     1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
        pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
        yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                    1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
        
        // Convert the floating point angles in radians to a scale from 0 to 18.
        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
        onOrientationData2(myo, timestamp, quat);
    }
    
    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example, making a fist, or not making a fist anymore.
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;
        
        if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
            // Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
            // Myo becoming locked.
            myo->unlock(myo::Myo::unlockHold);
            
            // Notify the Myo that the pose has resulted in an action, in this case changing
            // the text on the screen. The Myo will vibrate.
            myo->notifyUserAction();
        } else {
            // Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
            // are being performed, but lock after inactivity.
            myo->unlock(myo::Myo::unlockTimed);
        }
        onPose2(myo, timestamp, pose);
    }
    
    // onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their arm. This lets gestures[i].substring(0,gestures[i].find_first_of('_') Myo know which arm it's on and which way it's facing.
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                   myo::WarmupState warmupState)
    {
        onArm = true;
        whichArm = arm;
        onArmSync2(myo, timestamp, arm, xDirection, rotation, warmupState);
    }
    
    // onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen when Myo is moved around on the arm.
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        onArm = false;
        onArmUnsync2(myo, timestamp);
    }
    
    // onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = true;
        onUnlock2(myo, timestamp);
    }
    
    // onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = false;
        onLock2(myo, timestamp);
    }
    
private:
    // We define this function to print the current values that were updated by the on...() functions above.
    void print()
    {
        // Clear the current line
        std::cout << '\r';
        
        // Print out the orientation. Orientation data is always available, even if no arm is currently recognized.
        std::cout << '[' << std::string(roll_w, '*') << std::string(18 - roll_w, ' ') << ']'
        << '[' << std::string(pitch_w, '*') << std::string(18 - pitch_w, ' ') << ']'
        << '[' << std::string(yaw_w, '*') << std::string(18 - yaw_w, ' ') << ']';
        
        if (onArm) {
            // Print out the lock state, the currently recognized pose, and which arm Myo is being worn on.
            
            /* Pose::toString() provides the human-readable name of a pose. We can also output a Pose directly to an
             * output stream (e.g. std::cout << currentPose;). In this case we want to get the pose name's length so
             * that we can fill the rest of the field with spaces below, so we obtain it as a string using toString().
             * std::string poseString = currentPose.toString();
             */
            std::string poseString = currentPose.toString();
            
            std::cout << '[' << (isUnlocked ? "unlocked" : "locked  ") << ']'
            << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
            << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
        } else {
            // Print out a placeholder for the arm and pose when Myo doesn't currently know which arm it's on.
            std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
        }
        
        std::cout << std::flush;
    }
    
};

