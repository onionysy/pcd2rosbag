#include <ros/ros.h>
#include <rosbag/bag.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>//which contains the required definitions to load and store point clouds to PCD and other file formats.
#include <string>     
#include <iostream>
#include <fstream>
using namespace std;
 
sensor_msgs::PointCloud2 read_pcd_path(string path, int num)
{
  pcl::PointCloud<pcl::PointXYZ> cloud;
  sensor_msgs::PointCloud2 output;
  string path_final = path + to_string(num) + ".pcd";
  cout << "Read file:" << path_final << endl; 
  pcl::io::loadPCDFile (path_final, cloud); //修改自己pcd文件所在路径
  //Convert the cloud to ROS message
  
  pcl::toROSMsg(cloud, output);
  output.header.frame_id = "vlp16";//this has been done in order to be able to visualize our PointCloud2 message on the RViz visualizer    
//！！！这一步需要注意，是后面rviz的 fixed_frame  !!!敲黑板，画重点。
  return output;
}




int main (int argc, char **argv)
{
  ros::init (argc, argv, "UandBdetect");
  ros::NodeHandle nh;
  ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2> ("pcl_output", 10);
  pcl::PointCloud<pcl::PointXYZ> cloud;
  sensor_msgs::PointCloud2 output;
  string path = "/home/ysy/pcd_mod/vlp16_";
  string bagname = "/home/ysy/mod.bag";

  ofstream fileout(bagname,ios::trunc);//ios::trunc是清除原文件内容,可不写,默认就是它
  if(!fileout){
      cout << "Create file failure...\n";
      exit(0);
  }
  fileout.close();

  rosbag::Bag bag;
  double time = ros::Time::now().toSec();
  bag.open(bagname,rosbag::bagmode::Write);


  for (float num = 1. ; num <= 792 ; num++)
  {
    output = read_pcd_path(path,num);
    output.header.frame_id = "vlp16" ;
    output.header.stamp = ros::Time().fromSec(float(num/10)+time);
    // cout << "scan time:" << float(num/10)+time << endl; 
    cout << "scan time:" << output.header.stamp << endl; 
    bag.write("/velodyne_points",output.header.stamp,output);
  }
  bag.close();
  cout<<"Bag file write finish"<<endl;

  // ros::Rate loop_rate(1);
  // while (ros::ok())
  // {
  //   pcl_pub.publish(output);
  //   ros::spinOnce();
  //   loop_rate.sleep();
  // }
  return 0;
}


