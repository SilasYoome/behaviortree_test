/*
input port & output port不同於ch02，端口型態為std::string
ch03主要是教說如何宣告成不同型態的端口，int、long、double、bool、NodeStatus等

除此之外，本節還教了兩種資料寫入方式
一種是用output port來寫入
一種是用xml檔案裡面blackboard的功能來寫入

*/

#include <ros/ros.h>
#include <behaviortree_cpp_v3/bt_factory.h>
#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>
#include <behaviortree_cpp_v3/loggers/bt_zmq_publisher.h>

#include "ch_03_test.h"

using namespace BT;
int main(int argc,char **argv){
    ros::init(argc,argv,"bt_ch03_node");

    ros::NodeHandle nh("~");
    std::string xml_filename;

    nh.param<std::string>("file",xml_filename,"/home/behaviortree_test/src/bt_ch03/bt_ch03_xml/bt_ch03_xml.xml");
    ROS_INFO("LoadingXML : %s",xml_filename.c_str());

    BehaviorTreeFactory factory;
    factory.registerNodeType<CalculateGoal>("CalculateGoal");
    factory.registerNodeType<PrintTarget>("PrintTarget");

    auto tree = factory.createTreeFromFile(xml_filename);

    StdCoutLogger logger_cout(tree);


    NodeStatus status = NodeStatus::RUNNING;//預設節點robot_sequeue狀態目前為RUNNING
    // Keep on ticking until you get either a SUCCESS or FAILURE state
    while (ros::ok() && status == NodeStatus::RUNNING) {  // ROS運行OK以及robot_squeue狀態為RUNNING 就開始運行
        status = tree.rootNode()->executeTick();  // status變成現在樹正在運行的節點，並顯示它目前運行的狀態
        // Sleep 100 milliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 等個0.1秒
    }
  return 0;
}