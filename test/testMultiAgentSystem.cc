#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include"../include/MultiAgentSystem.h"

using namespace std;

int main(int argc, char **argv) {

    // Constructor
    std::string pathToVoc("../Vocabulary/ORBvoc.txt");
    MAM3SLAM::MultiAgentSystem mas(pathToVoc);

    // void MultiAgentSystem::addAgent(const string &strSettingsFile)
    std::string strSettingsFile1("./settingsForTest_00.yaml");
    std::string strSettingsFile2("./settingsForTest_01.yaml");
    mas.addAgent(strSettingsFile1);
    mas.addAgent(strSettingsFile2);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    return 0;
}