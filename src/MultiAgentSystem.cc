#include "MultiAgentSystem.h"

namespace MAM3SLAM
{

Verbose::eLevel Verbose::th = Verbose::VERBOSITY_NORMAL;

MultiAgentSystem::MultiAgentSystem(const string &strVocFile, bool bActiveLC, bool bUseViewer) : mbShutDown(false), mbUseViewer(bUseViewer) 
{
    // Clear files
    std::ofstream ofs;
    ofs.open("/home/ju/Copie_de_travail_ORBSLAM3/MAM3SLAM/output/MapLogs.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ofs.open("/home/ju/Copie_de_travail_ORBSLAM3/MAM3SLAM/output/reloc.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    mStrVocabularyFilePath = strVocFile;

    //Load ORB Vocabulary
    cout << endl << "Loading ORB Vocabulary. This could take a while..." << endl;

    mpVocabulary = new ORBVocabulary();
    bool bVocLoad = mpVocabulary->loadFromTextFile(strVocFile);
    if(!bVocLoad)
    {
        cerr << "Wrong path to vocabulary. " << endl;
        cerr << "Failed to open at: " << strVocFile << endl;
        exit(-1);
    }
    cout << "Vocabulary loaded!" << endl << endl;

    //Create KeyFrame Database
    mpKeyFrameDatabase = new KeyFrameDatabase(*mpVocabulary);

    //Create the Atlas
    cout << "Initialization of Atlas from scratch " << endl;
    mpAtlas = new Atlas();

    //Initialize the Loop Closing thread and launch
    // mSensor!=MONOCULAR && mSensor!=IMU_MONOCULAR
    bool bFixScale = false;
    mpLoopCloser = new LoopClosing(mpAtlas, mpKeyFrameDatabase, mpVocabulary, bFixScale, bActiveLC);
    mpLoopCloser->SetMultiAgentSystem(this);
    mptLoopClosing = new thread(&MAM3SLAM::LoopClosing::Run, mpLoopCloser);

    // // Fix verbosity
    // Verbose::SetTh(Verbose::VERBOSITY_QUIET);

}

MultiAgentSystem::~MultiAgentSystem() {
    Shutdown();
    // SaveKFTrajectory();
}

void MultiAgentSystem::addAgent(const string &strSettingsFile) {
    Agent* pNewAgent = new Agent(strSettingsFile, this, mbUseViewer);
    mvpAgents.push_back(pNewAgent);
    usleep(3000);
}

void MultiAgentSystem::addAgent(Agent* pNewAgent) {
    mvpAgents.push_back(pNewAgent);
    usleep(3000);
}

bool MultiAgentSystem::MapChanged() {
    static int n=0;
    int curn = mpAtlas->GetLastBigChangeIdx();
    if(n<curn)
    {
        n=curn;
        return true;
    }
    else
        return false;
}

void MultiAgentSystem::Shutdown() {
    {
        unique_lock<mutex> lock(mMutexReset);
        mbShutDown = true;
    }
    mpLoopCloser->RequestFinish();
    cout << "Shutdown MultiAgentsystem" << endl;
    for (int i = 0 ; i < mvpAgents.size() ; i++) {
        mvpAgents[i] -> Shutdown();
    }
}


bool MultiAgentSystem::isShutDown() {
    return true;
}

ORBVocabulary* MultiAgentSystem::getVocabulary() {
    return mpVocabulary;
}

KeyFrameDatabase* MultiAgentSystem::getKeyFrameDatabase() {
    return mpKeyFrameDatabase;
}

Atlas* MultiAgentSystem::getAtlas() {
    return mpAtlas;
}

LoopClosing* MultiAgentSystem::getLoopCloser() {
    return mpLoopCloser;
}

// FIXME !!! Pratique à risque.
Agent* MultiAgentSystem::getAgent(int i) {
    return mvpAgents[i];
}

void MultiAgentSystem::StartViewer() {
       // MultiAgentViewer
    mpMultiAgentViewer = new MultiAgentViewer();
    for (int i = 0 ; i < mvpAgents.size() ; i++) {
        mpMultiAgentViewer -> AddAgentViewer(mvpAgents[i] -> getAgentViewer());
    }
    mptMultiAgentViewer = new thread(&MAM3SLAM::MultiAgentViewer::Run, mpMultiAgentViewer);
}

std::vector<Agent*> MultiAgentSystem::GetAgentsInMap(long unsigned int nMapId)
{
    std::vector<Agent*> vpAgentsInMap;
    for(Agent* pAgent : mvpAgents)
    {
        if (pAgent->GetCurrentMap()->GetId() == nMapId)
        {
            vpAgentsInMap.push_back(pAgent);
        }
    }
    return vpAgentsInMap;
}

void MultiAgentSystem::SaveKFTrajectory() {
    std::string filename("/home/ju/Copie_de_travail_ORBSLAM3/MAM3SLAM/output/KF_traj.txt");
    std::cout << "Saving KF trajectory to " << filename << " ..." << std:: endl;
    
    ofstream f;
    f.open(filename.c_str());
    f << fixed;

    f << setprecision(6) << "ts" << setprecision(7) << " tx ty tz qx qy qz qw agent map" << endl;

    vector<Map*> vpMaps = mpAtlas->GetAllMaps();
    for(int j=0; j<vpMaps.size(); j++)
    {
        vector<KeyFrame*> vpKFs = vpMaps[j]->GetAllKeyFrames();
        for(size_t i=0; i<vpKFs.size(); i++)
        {
            KeyFrame* pKF = vpKFs[i];

        // pKF->SetPose(pKF->GetPose()*Two);

            if(pKF->isBad())
                continue;

            Sophus::SE3f Twc = pKF->GetPoseInverse();
            Eigen::Quaternionf q = Twc.unit_quaternion();
            Eigen::Vector3f t = Twc.translation();
            f << setprecision(6) << pKF->mTimeStamp << setprecision(7) << " " << t(0) << " " << t(1) << " " << t(2) << " " << q.x() << " " << q.y() << " " << q.z() << " " << q.w() << " " << pKF->getAgent()->mnId << " " << pKF->GetMap()->GetId() << endl;

        }
    }

    f.close();
    std::cout << "Trajectory saved!" << std::endl;

    for (int i = 0 ; i < mvpAgents.size() ; i++) {
        mvpAgents[i] -> SaveTrackingStates();
        mvpAgents[i] -> SaveTrajectory();
    }
}

void MultiAgentSystem::SaveTimes()
{
    // Place recognition
    std::stringstream ss;
    ss << "/home/ju/Copie_de_travail_ORBSLAM3/MAM3SLAM/output/TimesPR.txt";
    std::string filename = ss.str();
    cout << endl << "Saving LM durations to " << filename << " ..." << endl;
    ofstream f;
    f.open(filename.c_str());
    f << fixed;
    f << "PR_duration(ms)" << std::endl;
    for(int i = 0 ; i < mpLoopCloser->mvdPR_ms.size() ; i++)
    {
        f << setprecision(6) << mpLoopCloser->mvdPR_ms[i] << std::endl;
    }
    f.close();
    cout << endl << "PR durations saved!" << endl;

    // LC
    std::stringstream ss2;
    ss2 << "/home/ju/Copie_de_travail_ORBSLAM3/MAM3SLAM/output/TimesLC.txt";
    filename = ss2.str();
    cout << endl << "Saving LC durations to " << filename << " ..." << endl;
    f.open(filename.c_str());
    f << fixed;
    f << "LC_duration(ms)" << std::endl;
    for(int i = 0 ; i < mpLoopCloser->mvdLC_ms.size() ; i++)
    {
        f << setprecision(6) << mpLoopCloser->mvdLC_ms[i] << std::endl;
    }
    f.close();
    cout << endl << "LC durations saved!" << endl;

    // MM
    std::stringstream ss3;
    ss3 << "/home/ju/Copie_de_travail_ORBSLAM3/MAM3SLAM/output/TimesMM.txt";
    filename = ss3.str();
    cout << endl << "Saving MM durations to " << filename << " ..." << endl;
    f.open(filename.c_str());
    f << fixed;
    f << "MM_duration(ms)" << std::endl;
    for(int i = 0 ; i < mpLoopCloser->mvdMM_ms.size() ; i++)
    {
        f << setprecision(6) << mpLoopCloser->mvdMM_ms[i] << std::endl;
    }
    f.close();
    cout << endl << "MM durations saved!" << endl;

    for (int i = 0 ; i < mvpAgents.size() ; i++) {
        mvpAgents[i] -> SaveTimes();
    }
}

}