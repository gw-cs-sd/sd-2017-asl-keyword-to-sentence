//
//  main.cpp
//  analyze
//
//  Created by Andy Acosta on 1/22/17.
//  Copyright © 2017 Andy Acosta. All rights reserved.
//

#include <iostream>
#include <sys/stat.h>
#include <vector>

#include <map>
#include <cstdlib>
#include "../grt-master/GRT/GRT.h"

using namespace GRT;
using namespace std;

/*  accessFiles adapted from
 *  http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c.
 */
void accessFiles(std::vector<std::string> &out, const std::string directoryName)
{
    DIR *dir;
    class dirent *ent;
    class stat st;
    
    dir = opendir(directoryName.c_str());
    if(dir == NULL)
        std::cout << "Error opening directory." << std::endl;
    while( (ent = readdir(dir)) != NULL){
        const char *filename = ent->d_name;
        const std::string fullname = directoryName + "/" + filename;
        
        std::cout << fullname << std::endl;
        
        if(filename[0] == '.'){
            continue;
        }
        if(stat(fullname.c_str(), &st) == -1){
            continue;
        }
        
        const bool isDirectory = (st.st_mode & S_IFDIR) != 0;
        
        if(isDirectory)
            continue;
        
        out.push_back(fullname);
        
        std::cout << "vector stores : " << int(out.size()) << " files\n";
        
    }
    
    closedir(dir);
    std::cout << "Directory closed." << std::endl;
}


int main(int argc, const char * argv[]) {
    int status;
    status = mkdir("/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/processed", ACCESSPERMS);
    
    if(status){
        std::cout << "Error making directory or might already exist. \n";
    }
    else
        std::cout << "Directory created!\n";
    
    std::vector<std::string> gestures(0, "");
    accessFiles(gestures, "/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/data");
    sort(gestures.begin(), gestures.end());
    
    UINT gestureLabel = 1;
    
    TimeSeriesClassificationData training;
    
    training.setNumDimensions(14);
    MatrixDouble trainingSample;
    
    training.setDatasetName("GestureTraining");
    training.setInfoText("Gesture dataset that is to be used by the hub");
    
    std::string::size_type sz;
    
    for(UINT i = 0; i < gestures.size(); i++){
        gestureLabel = i+1;
        
        trainingSample.clear();
        vector <vector <double> > data;
        
        ifstream infile( gestures[i] );
        std::cout << gestures[i] << std::endl;
        
        int testnum = 0;
        while (infile)
        {
            string s;
            if (!getline( infile, s )) break;
            
            istringstream ss( s );
            vector <double> record;
            int col = 0;
            int checkedtestnum;
            while (ss)
            {
                
                string s;
                if (!getline( ss, s, ',' ))
                    break;
                if(col == 0){
                    std::cout << "Col is 0" << s << std::endl;
                    checkedtestnum = std::stod(s, &sz);
                    if(checkedtestnum != testnum){
                        std::cout << "changing testnum " << std::endl;
                        data.push_back( record );
                        trainingSample.push_back(record);
                        
                        training.addSample(gestureLabel, trainingSample);
                        trainingSample.clear();
                        testnum++;
                    }
                }
                else{
                    std::cout << "Col is not 0" << std::endl;
                    double z = std::stod(s, &sz);
                    record.push_back( z );
                }
                col++;
                
            }
            
            data.push_back( record );
            trainingSample.push_back(record);
        }
        std::cout << data.size() << std::endl;
        training.addSample(gestureLabel, trainingSample);

        if (!infile.eof())
        {
            cerr << "Error with file!\n";
        }
        
    }
    
    if( !training.saveDatasetToFile( "/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/processed/trainingdata.txt" ) ){
        std::cout << "Failed to save dataset to file!\n";
    }
    
    DTW dtw1;
    
    if( !dtw1.train( training ) ){
        std::cout << "Couldn't train the classifier" << std::endl;
    }else{
        std::cout << "Dataset trained!" << std::endl;
    }
    
    
    dtw1.enableNullRejection(true);
    dtw1.enableTrimTrainingData(true, 0.1, 90);
    if(!dtw1.saveModelToFile( "/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/processed/DTWModel.txt" )){
        std::cout << "Failed to save the classifier model" << std::endl;
    }
    else{
        std::cout << "Saved the classifier model!" << std::endl;
    }
    
    //This can then be loaded later
    if( !training.loadDatasetFromFile( "/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/processed/trainingdata.txt" ) ){
        std::cout << "Failed to load dataset from file!\n";
    }
    
    //This is how you can get some stats from the training data
    std::string datasetName = training.getDatasetName();
    std::string infoText = training.getInfoText();
    UINT numSamples = training.getNumSamples();
    UINT numDimensions = training.getNumDimensions();
    UINT numClasses = training.getNumClasses();
    
    std::cout << "Dataset Name: " << datasetName << std::endl;
    std::cout << "InfoText: " << infoText << std::endl;
    std::cout << "NumberOfSamples: " << numSamples << std::endl;
    std::cout << "NumberOfDimensions: " << numDimensions << std::endl;
    std::cout << "NumberOfClasses: " << numClasses << std::endl;
    
    //You can also get the minimum and maximum ranges of the data
    std::vector< MinMax > ranges = training.getRanges();
    
    std::cout << "The ranges of the dataset are: \n";
    for(UINT r=0; r<ranges.size(); r++){
        std::cout << "Dimension: " << r << " Min: " << ranges[r].minValue << " Max: " << ranges[r].maxValue << std::endl;
    }
    
    TimeSeriesClassificationData testData = training.partition( 80 );
    //Use the test dataset to test the DTW model
    double accuracy = 0;
    for(UINT i=0; i<testData.getNumSamples(); i++){
        //Get the i'th test sample - this is a timeseries
        UINT classLabel = testData[i].getClassLabel();
        MatrixDouble timeseries = testData[i].getData();
        
        //Perform a prediction using the classifier
        if( !dtw1.predict( timeseries ) ){
            std::cout << "Failed to perform prediction for test sampel: " << i <<"\n";
            return EXIT_FAILURE;
        }
        
        //Get the predicted class label
        UINT predictedClassLabel = dtw1.getPredictedClassLabel();
        double maximumLikelihood = dtw1.getMaximumLikelihood();
        VectorDouble classLikelihoods = dtw1.getClassLikelihoods();
        VectorDouble classDistances = dtw1.getClassDistances();
        
        //Update the accuracy
        if( classLabel == predictedClassLabel ) accuracy++;
        
        std::cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << std::endl;
    }
    
    std::cout << "Test Accuracy: " << accuracy/double(testData.getNumSamples())*100.0 << "%" << std::endl;
    
}
