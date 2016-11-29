//
//  getpredict.cpp
//  sdproject
//
//  Created by Andy Acosta on 11/26/16.
//  Copyright © 2016 Andy Acosta. All rights reserved.
//

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <fstream>


/*  Adapted from
 *  http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c. 
 */

void func3();
void accessFiles(std::vector<std::string> &out, const std::string directoryName);

std::vector<std::vector<std::vector<double > > > data;

int main()
{
    int status;
    status = mkdir("/Users/aacosta/Documents/seniordesign/sd-2017-asl-keyword-to-sentence/myo/sdk/project/processed", ACCESSPERMS);
    
    /*
     *  Testing to see if the directory was created
     */
    
    if(status){
        std::cout << "Error making directory or might already exist. \n";
    }
    else
        std::cout << "Directory created!\n";
    
    func3();
    
    std::vector<std::string> gestures(0, "");
    accessFiles(gestures, "rawdata");
    sort(gestures.begin(), gestures.end());
    data = std::vector<std::vector<std::vector<double > > >(gestures.size(), std::vector<std::vector<double > >(0, std::vector<double > (0,0)));
    
    for(size_t i = 0; i < gestures.size(); i++){
        std::ifstream fin(gestures[i]);
        int n;
        fin >> n;
        
        data[i] = std::vector<std::vector<double > >(n, std::vector<double>(6, 0));
        for(int j = 0; j < n; j++){
            for(int k = 0; k < 6; k++){
                fin >> data[i][j][k];
            }
        }
        fin.close();
    }
    
    
}

void func3(){
    std::cout << "hello!" << std::endl;
}

void accessFiles(std::vector<std::string> &out, const std::string directoryName)
{
    DIR *dir;
    class dirent *ent;
    class stat st;
    
    dir = opendir(directoryName.c_str());
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
        
    }
    closedir(dir);
}
