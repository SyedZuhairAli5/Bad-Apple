#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib");

//  ".\run.bat" to run the animation in the windows terminal

using namespace std;

const int numOfFrames = 6569;
const double FPS = 30.0;
const double frameDurationMs = 1000.0/FPS;

COORD cursorPos = {0, 0}; //the point from where anything is written on the console

void setup(ifstream &currentFrame){
    system("cls");
    if(!currentFrame.is_open()){
        cout << "Error loading frame." << endl;
    }
    else if(currentFrame.is_open()){
        string line;
        cout << "Press the 'Maximize Panel' button at the top right of your VS Code terminal to avoid animation flicker issues." << endl << endl;

        while(getline(currentFrame, line)){
            cout << line << endl;
        }

        cout << endl << "Scroll up if the whole image isn't visible" << endl << "Press ENTER to continue..";
        cin.get();
    }
}

string filePathName(int num) {return "res/BA" + to_string(num) + ".txt";}

void playAnim(ifstream &currentFrame, int num, HANDLE hConsole){
    SetConsoleCursorPosition(hConsole, cursorPos); //sets cursor to 0,0 to draw the next frame without using cls

    string fileName = filePathName(num);
    currentFrame.open(fileName);

    if(!currentFrame.is_open()){return;}

    string line;
    while(getline(currentFrame, line)) {cout << line << endl;}
    //close the file after the frame is drawn   
    currentFrame.close();
}

long getMusicPosMs(){
    char mciBuffer[128];
    mciSendStringA("status music position", mciBuffer, sizeof(mciBuffer), NULL);
    return atol(mciBuffer); //milliseconds
}


int main()
{
    //initializing and stuff
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ifstream currentFrame;
    int frameNum = 407; //initially setting the frame number to frame number 407
    string fileName = filePathName(frameNum);
    mciSendStringA("open \"BA_WAV.wav\" type waveaudio alias music", NULL, 0, NULL);
    mciSendStringA("set music time format milliseconds", NULL, 0, NULL);

    //setup
    currentFrame.open(fileName);
    setup(currentFrame);
    system("cls");
    currentFrame.close();

    //actual animation
    mciSendStringA("play music", NULL, 0, NULL);

    int lastFrame = -1;

    while(true){
        long musicMs = getMusicPosMs();
        frameNum = static_cast<int>(musicMs/frameDurationMs);

        if(frameNum >= numOfFrames) {break;}

        if(frameNum != lastFrame){
            playAnim(currentFrame, frameNum+1, hConsole);
            lastFrame = frameNum;
        }

        this_thread::sleep_for(chrono::milliseconds(1));
    }

    return 0;
}