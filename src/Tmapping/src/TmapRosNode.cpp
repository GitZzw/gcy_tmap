//
// Created by stumbo on 2019/12/19.
//

#include "TmapRosNode.h"
#include <fstream>
#include <chrono>
using namespace tmap;
using namespace std;

tmap::TmapRosNode::TmapRosNode() :
        n(),
        srvAruco(n.advertiseService(GETID,&TmapRosNode::cbSrvAruco, this)),
        //done
        srvNewExp(n.advertiseService(TMAP_STD_SERVICE_NAME_NEW_EXP,
                                     &TmapRosNode::cbSrvNewExp, this)),
        //done
        srvGateMovement(n.advertiseService(TMAP_STD_SERVICE_NAME_GATE_MOVE,
                                           &TmapRosNode::cbSrvGateMovement, this)),
        //done
        srvGetmaps(n.advertiseService(TMAP_STD_SERVICE_NAME_GET_MAPS,
                                      &TmapRosNode::cbSrvGetMaps, this)),
        //done
        srvSetSurviver(n.advertiseService(TMAP_STD_SERVICE_NAME_SET_SURVIVERS,
                                      &TmapRosNode::cbSrvSetSuriviers, this)),
        srvReset(n.advertiseService(TMAP_STD_SERVICE_NAME_RESET,
                                      &TmapRosNode::cbSrvReset, this)),
        srvChHis(n.advertiseService(TMAP_STD_SERVICE_GET_CHAMPION_HISTORY,
                                      &TmapRosNode::cbSrvGetEvoOfChampion, this)),
        //ignore
        srvSaveChampion(n.advertiseService(TMAP_STD_SERVICE_SAVE_CHAMPION,
                                      &TmapRosNode::cbSrvSaveChampion, this)),
        mTmappingCore(new TopoMapping)
{

}


static double temp;

//void TmapRosNode::ModifyLineData(const char* fileName, int lineNum, const char* lineData)
//{
//    ifstream in;
//    in.open(fileName);
//    string strFileData = "";
//    int line = 1;
//    char tmpLineData[1024] = {0};
//    while(in.getline(tmpLineData, sizeof(tmpLineData)))
//    {
//        if (line == lineNum)
//        {
//            strFileData += lineData;
//            strFileData += "\n";
//        }
//        else
//        {
//            strFileData += tmpLineData;
//            strFileData += "\n";
//        }
//        line++;
//    }
//    in.close();
//    //写入文件
//    ofstream out;
//    out.open(fileName);
//    out.flush();
//    out<<strFileData;
//    out.close();
//}


bool TmapRosNode::cbSrvAruco(tmapping::arucoIdRequest& req, tmapping::arucoIdResponse& res) {
    cout<<"get aruco ID"<<endl;
    ofstream fout("/home/uav/id.txt",ios_base::app); //创建待写入数据文件
    int aruID = req.num;
    int indoorID = req.indoor;
    int leavedoorID = req.leavedoor;
    float disx = req.disx;
    float disy = req.disy;
    fout<<aruID;
    fout<<"  ";
    fout<<disx;
    fout<<"  ";
    fout<<disy;
    fout<<"  ";
    fout<<indoorID;
    fout<<"  ";
    fout<<leavedoorID;
    fout<<"\n";
    fout.close();

//    string disxstr = to_string(disx)+",";
//    string disystr = to_string(disy);
//    const char* xstr = disxstr.data();
//    const char* ystr = disystr.data();
//
////    cout<<disx<<"  "<<disy<<"   "<<aruID<<endl;
//
//
//    stringstream fout2;
//    fout2<<req;
//    string m;
//    while(fout2>>m){
//        if(m=="num:"){
//            continue;
//        }
//        else if(m == "2"){
//            //修改里程计信息
//            string filename =  "/home/zzw/tmappingMaps/test"+m+".json";
//
//            const char* p = filename.data();
//
//            TmapRosNode::ModifyLineData(p,11,xstr);
//            TmapRosNode::ModifyLineData(p,12,ystr);
//        }
//        else if(m == "4"){
//            //修改里程计信息
//            string filename =  "/home/zzw/tmappingMaps/test"+m+".json";
//            const char* p = filename.data();
//            TmapRosNode::ModifyLineData(p,11,xstr);
//            TmapRosNode::ModifyLineData(p,12,ystr);
//        }
//    }


}


bool TmapRosNode::cbSrvNewExp(tmapping::NewExpRequest& req,
                              tmapping::NewExpResponse& res)
{
    cout << "get new exp info" << endl;
#if TMAPPING_CONFIG_LOG_TIME
    auto startTime = std::chrono::system_clock::now();
    static bool once = false;
    if (!once) {
        once = true;
        cout << "nMap\t newExpTime \t gateTime \t All" << endl;
    }
#endif
    const auto& newExp = make_shared<Exp>(JsonHelper::Str2JS(req.jNewExp));
    mTmappingCore->arriveNewExp(newExp);
#if TMAPPING_CONFIG_LOG_TIME
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> diff(endTime - startTime);
    temp = diff.count();
//    cout << "\nTIME newExp: " << diff.count() << endl;
    cout << diff.count() << '\t';
#endif
    res.jChampionStatus = JsonHelper::JS2Str(mTmappingCore->getChampionDefendedCount());
    return true;
}

bool TmapRosNode::cbSrvGateMovement(tmapping::GateMovementRequest& req,
                                    tmapping::GateMovementResponse& res)
{
//    cout << "get gate movement info" << endl;
    auto startTime = std::chrono::system_clock::now();
//    cout<<"jGateMove is "<<req.jGateMove<<endl;
    auto gateID = JsonHelper::Str2JS(req.jGateMove).asInt();
    mTmappingCore->setLeftGate(gateID);
#if TMAPPING_CONFIG_LOG_TIME
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> diff(endTime - startTime);
//    cout << diff.count() << '\t' << diff.count() + temp << endl;
#endif
    return true;
}

bool
TmapRosNode::cbSrvGetMaps(tmapping::GetMapsRequest& req,
                          tmapping::GetMapsResponse& res)
{
    cout << "Processing [GetMaps] request..." << endl;
    cout<<"nMapRequired is "<<req.nMapRequired<<endl;
    auto topoMaps = mTmappingCore->getTopMaps(req.nMapRequired);
    res.jMaps = JsonHelper::JS2Str(topoMaps);
//    cout<<res.jMaps<<endl;
    cout << "Returned " << topoMaps["maps"].size() << " maps" << endl;
    return true;
}

bool TmapRosNode::cbSrvSetSuriviers(tmapping::SetSurviverMapsNumRequest& req,
                                    tmapping::SetSurviverMapsNumResponse& res)
{
    mTmappingCore->setNSurviverMaps(req.nMaps);
    cout<<"nMaps is "<<req.nMaps<<endl;
    cout << "The survivor count is set to " << req.nMaps << endl;
    return true;
}

bool TmapRosNode::cbSrvReset(std_srvs::EmptyRequest& req, std_srvs::EmptyResponse& res)
{
    mTmappingCore.reset(new TopoMapping);
    cout << "Mapping core is reseted" << endl;
    return true;
}

bool
TmapRosNode::cbSrvGetEvoOfChampion(tmapping::GetMapsRequest& req, tmapping::GetMapsResponse& res)
{
    res.jMaps = JsonHelper::JS2Str(mTmappingCore->getChampionHistory());
    return true;
}

bool TmapRosNode::cbSrvSaveChampion(std_srvs::EmptyRequest& req, std_srvs::EmptyResponse& res)
{
    int r = JsonHelper::saveJson(mTmappingCore->getTopMaps(1)["maps"][0], "currentChampion", true);
    if ( r>=0 ) {
        cout << "save Champion map success!";
    } else {
        cerr << "save Champion map failure!";
    }
    return r >= 0;
}

