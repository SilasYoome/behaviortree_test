#pragma once

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <behaviortree_cpp_v3/action_node.h>
#include <tf/transform_datatypes.h>

using namespace BT;

struct Position2D{
   double x,y,quaternion_z,quaternion_w;
};

/*
 * 若需要寫入其他類型數據，需對數據進行解析，
 * 通過模板函數BT::convertFromString<Position2D>(StringView)，
 * 對數據進行分割（splitString(str, ';')），
 * 然後再調用convertFromString<double>轉換到double類型。
 */
namespace BT{
// 模板函式
template <> inline Position2D convertFromString(StringView str){
   printf("Converting string \"%s\"\n", str.data());  // 終端機顯示現在Position2D的內容，目前的型態是std::string

   // splitString()為字串分割，利用;來分割
   auto parts = splitString(str, ';');
   if(parts.size() != 4){
      throw RuntimeError("invalid input")
   }
   else{
      // 將字串分割完畢之後，就可以使用convertFromeString<資料型態>將字串修改成我們所需要的資料型態
      Position2D output;
      output.x = convertFromString<double>(parts[0]);
      output.y = convertFromString<double>(parts[1]);
      output.quaternion_z = convertFromString<double>(parts[2]);
      output.quaternion_w = convertFromString<double>(parts[3]);
      return output;
   }
}
}// end namespace BT

class MoveBase : public SyncActionNode{
public:
   MoveBase(const std::string& name) : SyncActionNode(name,{}){}

   // 必須靜態成員
   static PortsList providedPorts(){
      return { InputPort<Position2D>("goal") };
   }

   virtual NodeStatus tick() override;

   virtual void halt() override{
      _aborted = true;
   }

private:
   typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
   MoveBaseClient _client;
   bool _aborted;//  中止flag

}