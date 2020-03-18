/*************************************** 
* 
* LanXin TECH, All Rights Reserverd. 
* Created at Thu May 16 10:27:58 2019
* Contributor: Ling Shi, Ph.D 
* Email: lshi@robvision.cn 
* 
***************************************/ 

#include "common.h"
#include <iostream>

using namespace lanXin;
using namespace std;

struct SimulationData{
    vector<GeoTransform> vH_e_in_w;
    vector<GeoTransform> vH_g_in_c;
    GeoTransform H_c_in_e;
    GeoTransform H_c_in_w;
    HandEyeType htype;
};

SimulationData generate_data(HandEyeType h_type){
    SimulationData simulation_data;
    vector<GeoTransform> vH_e_in_w; 
	vH_e_in_w.push_back(getTransM(Geo3d(0.56, 0.4, 0.3), Geo3d(0.2, 0.2, 0)));
	vH_e_in_w.push_back(getTransM(Geo3d(0.6, -0.14, 0.3), Geo3d(-0.3, -0.17, 0)));
	vH_e_in_w.push_back(getTransM(Geo3d(0.98, 0.4, 0.40), Geo3d(0.2, 0.15, 0.1)));
	vH_e_in_w.push_back(getTransM(Geo3d(0.85, -0.2, 0.32), Geo3d(-0.19, 0.2, 0))); 
    if (h_type==EyeToHand){
        GeoTransform H_c_in_w = getTransM(Geo3d(0.8, 0.12, 2.4), Geo3d(M_PI*1.02, -0.12*M_PI, M_PI *0.51));
        GeoTransform H_g_in_e = getTransM(Geo3d(0.10, 0.20, -0.401), Geo3d(0.20, -0.20, 0.20));
        vector<GeoTransform> vH_g_in_w,vH_g_in_c;
        for(auto it = vH_e_in_w.begin(); it!= vH_e_in_w.end(); ++it)
        {
            vH_g_in_w.push_back((*it) * H_g_in_e);
        } 

        for (auto it = vH_g_in_w.begin(); it!= vH_g_in_w.end(); ++it)
        {
            vH_g_in_c.push_back(H_c_in_w.inverse() * (*it));
        }
        simulation_data.vH_e_in_w = vH_e_in_w;
        simulation_data.vH_g_in_c = vH_g_in_c;
        simulation_data.H_c_in_w = H_c_in_w;
        simulation_data.htype = EyeToHand;
        return simulation_data;
    }
    else if (h_type==EyeInHand)
    {
        GeoTransform H_g_in_w = getTransM(Geo3d(0.8, 0.12, 2.4), Geo3d(M_PI*1.02, -0.12*M_PI, M_PI *0.51));
        GeoTransform H_c_in_e = getTransM(Geo3d(0.10, 0.20, -0.401), Geo3d(0.20, -0.20, 0.20));
        vector<GeoTransform> vH_c_in_w,vH_g_in_c;
        for(auto it = vH_e_in_w.begin(); it!= vH_e_in_w.end(); ++it)
        {
            vH_c_in_w.push_back((*it) * H_c_in_e);
        } 

        for (auto it = vH_c_in_w.begin(); it!= vH_c_in_w.end(); ++it)
        {
            vH_g_in_c.push_back((*it).inverse()*H_g_in_w);
        }
        simulation_data.vH_e_in_w = vH_e_in_w;
        simulation_data.vH_g_in_c = vH_g_in_c;
        simulation_data.H_c_in_e = H_c_in_e;
        simulation_data.htype = EyeInHand;
        return simulation_data;
    }
	return SimulationData();
}

int main(int argc, char *argv[])
{	
    HandEyeType htype = EyeToHand;
    SimulationData simulation_data = generate_data(htype);
    GeoTransform H = calibrateHandEye(simulation_data.vH_e_in_w,simulation_data.vH_g_in_c,simulation_data.htype);
    if (simulation_data.htype==EyeToHand){
        cout << "Compare to original H : " <<endl<< simulation_data.H_c_in_w.matrix()  << endl;
    }
    else if(simulation_data.htype==EyeInHand)
    {
        cout << "Compare to original H: " <<endl<< simulation_data.H_c_in_e.matrix()  << endl;
    }
	return 0;
}