//
//  main.cpp
//  myoSign
//
//  Created by Andy Acosta on 1/22/17.
//  Copyright © 2017 Andy Acosta. All rights reserved.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <cmath>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// The only file that needs to be included to use the Myo C++ SDK is myo.hpp.
#include <myo/myo.hpp>
#include "../grt-master/GRT/GRT.h"

#include "../gesturelistener.h"

int main(int argc, char** argv)
{
    // Connect to a Myo.
    myo::Hub hub("com.example.gesturemyo");
    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) {
        throw std::runtime_error("Unable to find a Myo!");
    }
    
    myo->setStreamEmg(myo::Myo::streamEmgEnabled);
    
    // Add a GestureDeviceListener to the hub. See instructions in Readme if you want to extend GestureDeviceListener.
    // The files passed to the GestureDeviceListener are generated by the ProcessRaw and CollectRaw executables in the bin folder.
    // You can also build ProcessRaw and CollectRaw by building processraw.cpp and collectraw.cpp.
    gesturelistener collector("/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/processed/classNames.csv", "/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/processed/DTWModel.txt");
    hub.addListener(&collector);
    hub.setLockingPolicy(myo::Hub::lockingPolicyNone);
    
    
    while(1) {
        // Query every 10 milliseconds.
        hub.run(10);
        
        //collector.print();
        collector.recData();
        
        if(!collector.onArm) {
            cout << "!!!!!!!!!!!!!!Please sync the Myo!!!!!!!!!!!!!!!!!!!!!" << endl;
            return 0;
        }
        // Remember to call collector.recData() each time you call hub.run()!
        //collector.recData();
        
    }
    
    
    cout << "DONE!" << endl;
     
}
