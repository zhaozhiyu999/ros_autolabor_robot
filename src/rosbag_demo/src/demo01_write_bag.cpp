/* 
    1.导包
    2.初始化
    3.创建rosbag对象
    4.打开文件流
    5.写数据进bag（topic+msgs）
    5.关闭文件流
*/

#include "ros/ros.h"
#include "rosbag/bag.h"
#include "std_msgs/String.h"


int main(int argc, char *argv[])
{
    ros::init(argc,argv,"bag_write");
    ros::NodeHandle nh;
    //创建bag对象
    rosbag::Bag bag;
    //打开
    bag.open("test.bag",rosbag::BagMode::Write);//相对路径。与bag.close呼应
    //写
    std_msgs::String msg;
    msg.data = "hello world";
    bag.write("/chatter",ros::Time::now(),msg);//重载：话题名+时间戳+消息
    bag.write("/chatter",ros::Time::now(),msg);
    bag.write("/chatter",ros::Time::now(),msg);
    bag.write("/chatter",ros::Time::now(),msg);
    //关闭
    bag.close();

    return 0;
}