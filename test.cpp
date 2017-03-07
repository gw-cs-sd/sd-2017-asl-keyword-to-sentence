//
//  test.cpp
//  
//
//  Created by Andy Acosta on 1/22/17.
//
//
/*
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
int main( int argc, char* argv[] )
{
    
 
    ofstream myfile;
    myfile.open ("/Users/aacosta/Documents/seniordesign/update/example.csv");
    if(myfile.is_open())
        cout << "is open!" << endl;
    else
        cout << "not open" << endl;
    myfile << "This is the first cell in the first column, second, third, fourth\n";
    myfile << "a,b,c,\n";
    myfile << "c,s,v,\n";
    myfile << "1,2,3.456\n";
    myfile << "semi;colon";
    myfile.close();
    
 
    
    cout << "Start" << endl;
    
    vector<vector<int> > emgSamples = vector<vector<int> >(8, vector<int>(0,0));
    
    for(int i = 0; i < emgSamples.size(); i++){
        for(int j = 0; j < 8; j++){
            emgSamples[i].push_back(i);
            cout << emgSamples[i][j] << ",";
        }
        cout << endl;
    }
    
    return 0;
}*/

#include "/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/myo/sdk/project/grt-master/GRT/GRT.h"
using namespace GRT;

int main (int argc, const char * argv[])
{
    //Create a new instance of the TimeSeriesClassificationData
    TimeSeriesClassificationData trainingData;
    
    //Set the dimensionality of the data (you need to do this before you can add any samples)
    trainingData.setNumDimensions( 14 );
    
    //You can also give the dataset a name (the name should have no spaces)
    trainingData.setDatasetName("DummyData");
    
    //You can also add some info text about the data
    trainingData.setInfoText("This data contains some dummy timeseries data");
    
    //Here you would record a time series, when you have finished recording the time series then add the training sample to the training data
    UINT gestureLabel = 1;
    MatrixDouble trainingSample;
    
    //For now we will just add 10 x 20 random walk data timeseries
    Random random;
    for(UINT k=0; k<10; k++){//For the number of classes
        gestureLabel = k+1;
        
        //Get the init random walk position for this gesture
        VectorDouble startPos( trainingData.getNumDimensions() );
        for(UINT j=0; j<startPos.size(); j++){
            startPos[j] = random.getRandomNumberUniform(-1.0,1.0);
        }
        
        //Generate the 20 time series
        for(UINT x=0; x<20; x++){
            
            //Clear any previous timeseries
            trainingSample.clear();
            
            //Generate the random walk
            UINT randomWalkLength = random.getRandomNumberInt(90, 110);
            VectorDouble sample = startPos;
            for(UINT i=0; i<randomWalkLength; i++){
                for(UINT j=0; j<startPos.size(); j++){
                    sample[j] += random.getRandomNumberUniform(-0.1,0.1);
                }
                
                //Add the sample to the training sample
                trainingSample.push_back( sample );
            }
            
            //Add the training sample to the dataset
            trainingData.addSample( gestureLabel, trainingSample );
            
        }
    }
    
    //After recording your training data you can then save it to a file
    if( !trainingData.saveDatasetToFile( "TrainingData.txt" ) ){
        std::cout << "Failed to save dataset to file!\n";
        return EXIT_FAILURE;
    }
    
    //This can then be loaded later
    if( !trainingData.loadDatasetFromFile( "TrainingData.txt" ) ){
        std::cout << "Failed to load dataset from file!\n";
        return EXIT_FAILURE;
    }
    
    //This is how you can get some stats from the training data
    std::string datasetName = trainingData.getDatasetName();
    std::string infoText = trainingData.getInfoText();
    UINT numSamples = trainingData.getNumSamples();
    UINT numDimensions = trainingData.getNumDimensions();
    UINT numClasses = trainingData.getNumClasses();
    
    std::cout << "Dataset Name: " << datasetName << std::endl;
    std::cout << "InfoText: " << infoText << std::endl;
    std::cout << "NumberOfSamples: " << numSamples << std::endl;
    std::cout << "NumberOfDimensions: " << numDimensions << std::endl;
    std::cout << "NumberOfClasses: " << numClasses << std::endl;
    
    //You can also get the minimum and maximum ranges of the data
    std::vector< MinMax > ranges = trainingData.getRanges();
    
    std::cout << "The ranges of the dataset are: \n";
    for(UINT j=0; j<ranges.size(); j++){
        std::cout << "Dimension: " << j << " Min: " << ranges[j].minValue << " Max: " << ranges[j].maxValue << std::endl;
    }
    
    //If you want to partition the dataset into a training dataset and a test dataset then you can use the partition function
    //A value of 80 means that 80% of the original data will remain in the training dataset and 20% will be returned as the test dataset
    TimeSeriesClassificationData testData = trainingData.partition( 80 );
    
    //If you have multiple datasets that you want to merge together then use the merge function
    if( !trainingData.merge( testData ) ){
        std::cout << "Failed to merge datasets!\n";
        return EXIT_FAILURE;
    }
    
    //If you want to run K-Fold cross validation using the dataset then you should first spilt the dataset into K-Folds
    //A value of 10 splits the dataset into 10 folds and the true parameter signals that stratified sampling should be used
    if( !trainingData.spiltDataIntoKFolds( 10, true ) ){
        std::cout << "Failed to spiltDataIntoKFolds!\n";
        return EXIT_FAILURE;
    }
    
    //After you have called the spilt function you can then get the training and test sets for each fold
    for(UINT foldIndex=0; foldIndex<10; foldIndex++){
        TimeSeriesClassificationData foldTrainingData = trainingData.getTrainingFoldData( foldIndex );
        TimeSeriesClassificationData foldTestingData = trainingData.getTestFoldData( foldIndex );
    }
    
    //If need you can clear any training data that you have recorded
    //trainingData.clear();
    
    DTW dtw1;
    
    if( !dtw1.train( trainingData ) ){
        std::cout << "Couldn't train the classifier" << std::endl;
    }else{
        std::cout << "Dataset trained!" << std::endl;
    }
    
    
    dtw1.enableNullRejection(true);
    dtw1.enableTrimTrainingData(true, 0.1, 90);
    if(!dtw1.saveModelToFile( "/Users/aacosta/Documents/seniordesign/update/myoSign/DTWModel.txt" )){
        std::cout << "Failed to save the classifier model" << std::endl;
    }
    
    return EXIT_SUCCESS;
}
