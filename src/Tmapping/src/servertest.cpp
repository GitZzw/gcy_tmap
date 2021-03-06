#include "ros/ros.h"
#include <tmapping/arucoId.h>
#include <cstdlib>
#include <stdio.h>
using namespace std;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "send_mimic_data_client");

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<tmapping::arucoId>("arucoID");
    tmapping::arucoId srv;

    srv.request.num = 1;
    srv.request.disx = -3.0;
    srv.request.disy = 6.0;
    srv.request.indoor = 3;
    srv.request.leavedoor = 1;
    if (client.call(srv))
    {
        ROS_INFO("Ok");
    }
    else
    {
        ROS_ERROR("Failed");
        return 1;
    }
    srv.request.num = 2;
    srv.request.disx = 3.0;
    srv.request.disy = 0.0;
    srv.request.indoor = 0;
    srv.request.leavedoor = 1;
    if (client.call(srv))
    {
        ROS_INFO("Ok");
    }
    else
    {
        ROS_ERROR("Failed");
        return 1;
    }
    srv.request.num = 1;
    srv.request.disx = -3.0;
    srv.request.disy = 6.0;
    srv.request.indoor = 3;
    srv.request.leavedoor = 2;
    if (client.call(srv))
    {
        ROS_INFO("Ok");
    }
    else
    {
        ROS_ERROR("Failed");
        return 1;
    }
    srv.request.num = 4;
    srv.request.disx = 0.0;
    srv.request.disy = -3.0;
    srv.request.indoor = 0;
    srv.request.leavedoor = 1;
    if (client.call(srv))
    {
        ROS_INFO("Ok");
    }
    else
    {
        ROS_ERROR("Failed");
        return 1;
    }
    srv.request.num = 1;
    srv.request.disx = 3.0;
    srv.request.disy = 0.0;
    srv.request.indoor = 0;
    srv.request.leavedoor = 3;
    if (client.call(srv))
    {
        ROS_INFO("Ok");
    }
    else
    {
        ROS_ERROR("Failed");
        return 1;
    }
    srv.request.num = 2;
    srv.request.disx = 0.0;
    srv.request.disy = -3.0;
    srv.request.indoor = 1;
    srv.request.leavedoor = 0;
    if (client.call(srv))
    {
        ROS_INFO("Ok");
    }
    else
    {
        ROS_ERROR("Failed");
        return 1;
    }

    return 0;
}