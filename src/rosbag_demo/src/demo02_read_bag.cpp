/*  
    读取 磁盘上的 bag 文件：
        
    1.导包
    2.初始化
    3.创建rosbag对象
    4.打开文件流（不再是write，而是read）
    5.写数据进bag（topic+msgs）
    5.关闭文件流
*/



#include "ros/ros.h"
#include "rosbag/bag.h"
#include "rosbag/view.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"

int main(int argc, char *argv[])
{

    setlocale(LC_ALL,"");

    ros::init(argc,argv,"bag_read");
    ros::NodeHandle nh;

    //创建 bag 对象
    rosbag::Bag bag;
    //打开 bag 文件
    bag.open("test.bag",rosbag::BagMode::Read);
    //读数据：取出话题+时间戳+消息内容。县获取消息的集合然后再迭代取出消息的字段
    // for (rosbag::MessageInstance const m : rosbag::View(bag))
    // {
    //     std_msgs::String::ConstPtr p = m.instantiate<std_msgs::String>();
    //     if(p != nullptr){
    //         ROS_INFO("读取的数据:%s",p->data.c_str());
    //     }
    // }
    //video的solution
    for (auto &&m : rosbag::View(bag))
    {
        //解析
        std::string topic = m.getTopic();
        ros::Time time = m.getTime();
        std_msgs::StringPtr p = m.instantiate<std_msgs::String>();
        ROS_INFO("·解析的内容，话题：%s,时间戳:%.2f,消息值：%s",
                    topic.c_str(),
                    time.toSec(),
                    p->data.c_str()
        );

    }
    
    
    //关闭文件流
    bag.close();
    return 0;
}