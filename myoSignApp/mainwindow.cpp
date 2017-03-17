#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include <myo/myo.hpp>

#include <fstream>
#include <sstream>
#include <array>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>

using namespace std;

std::string gestureName;
int testNumber;

bool recording;

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
 *  START CUSTOM DATA COLLECTOR CLASS
 *
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
        //currentLabel = 1;
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
};


/*
 *  END CUSTOM DATA COLLECTOR CLASS
 */

void recordGesture(string gesturename, int t){
    
    
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startTraining_clicked()
{
    
    QString str = ui->gesturename->text();
    testNumber = ui->testNum->text().toInt();
    if(testNumber == 0 || !testNumber){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Must enter the number of times to test!");
        messageBox.setFixedSize(500,200);
    }
    if(str.size() == 0){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Must enter a gesture name!");
        messageBox.setFixedSize(500,200);
    }

    //QLabel strLabel = &str.QString;
    ui->gestureNameOutput->setText(str);
    ui->testsRemainOutput->setText(ui->testNum->text());
    
    string gesturename = str.toStdString();
    int t = ui->testNum->text().toInt();

    if(str.size() != 0 && (testNumber != 0 || testNumber)){
        QMessageBox messageBox;
        messageBox.information(0,"Start training!", "Press 'Start' to the start training a new gesture. Press 'Stop' to end the recording of the gesture.");
    }
    
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

    //recordGesture(str, testNumber);
    
    QString outputDataString = ui->outputData->toPlainText();
    outputDataString.append("Starting the recording module.\n");
    ui->outputData->setPlainText(outputDataString);
    
    
    //test counter
    int fno = 0;
    
    string fext = "../data";
    string filename = fext + "/" + gesturename + ".csv";
    if(file_exists(fext + "/" + gesturename + ".csv")){
        fno++;
        //filename = fext + "/" + gesturename + "_" + NumberToString(fno) + ".csv";
    }
    cout << "FNO: " << fno << endl;
    outputDataString.append("Test Counter: " + QString::fromStdString(std::to_string(fno)) + "\n");
    outputDataString.append("Press the 'Start' button when you are ready to start recording the gesture.\n");
    ui->outputData->setPlainText(outputDataString);
    
    /*
    while(1){
        while(recording){
            hub.run(10);
            if(!collector.onArm){
                cout << "Please sync Myo!!" << endl;
                outputDataString.append("Please sync Myo!!\n");
                ui->outputData->setPlainText(outputDataString);
            }
            collector.print();
            outputDataString.append("recording...");
            ui->outputData->setPlainText(outputDataString);
        }
        outputDataString.append("waiting...\n");
        ui->outputData->setPlainText(outputDataString);
    }
    */
    
    
}

bool stop;

void MainWindow::on_start_clicked()
{
    recording = true;
    QString outputDataString = ui->outputData->toPlainText();
    outputDataString.append("recording...\n");
    ui->outputData->setPlainText(outputDataString);
    myo::Hub hub("com.andyacosta.gesturemyo");
    myo::Myo* myo = hub.waitForMyo(10000);
    
    if(!myo){
        outputDataString.append("Couldn't find myo.\n");
        ui->outputData->setPlainText(outputDataString);
        //throw runtime_error("Couldn't find myo.");
    }
    outputDataString.append("Myo connected!\n");
    ui->outputData->setPlainText(outputDataString);
    
    myo->setStreamEmg(myo::Myo::streamEmgEnabled);
    
    DataCollector collector;
    
    
    hub.addListener(&collector);
    
    /*stop = true;
    while(stop){
        outputDataString.append("recording...");
        ui->outputData->setPlainText(outputDataString);
    }*/
    
    while(1){
        cout << "recording..." << endl;
    }
    
}

void MainWindow::on_stop_clicked()
{
    stop = false;
    QString outputDataString = ui->outputData->toPlainText();
    outputDataString.append("stopped recording...\n");
    ui->outputData->setPlainText(outputDataString);
    
}
