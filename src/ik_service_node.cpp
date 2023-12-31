#include <cstdlib>
#include <iostream>
#include "ros/ros.h"
#include "ik_service/PoseIK.h"
#include "ur_kinematics/ur_kin.h"


bool pose_ik(ik_service::PoseIK::Request &req, ik_service::PoseIK::Response &res) {
  double q[] = {3.14, -1.13, 1.51, 3.77, -1.51, 0};
  double T[4][4];
  int num_sols;
  double q_sols[8][6];
  ur_kinematics::forward((double *)&q[0], (double *)&T[0][0]);
  
  T[0][3] = req.part_pose.position.x;
  T[1][3] = req.part_pose.position.y;
  T[2][3] = req.part_pose.position.z;
  

  num_sols = ur_kinematics::inverse(&T[0][0], &q_sols[0][0], 0.0);
  
  ROS_INFO("By ur_kinematics, %i solutions were returned", num_sols);
  ROS_INFO("Send out ,%i solutions", num_sols);
  res.num_sols = num_sols;
  for(int i=0;i<8;++i){
  	for(int j=0;j<6;++j){
  		res.joint_solutions[i].joint_angles[j] = q_sols[i][j];
  	}
  }
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "Process_start");
  ros::NodeHandle n;


  ros::ServiceServer service = n.advertiseService("pose_ik", pose_ik);
  ROS_INFO("Start Service:");
  ros::spin();

  return 0;
}
