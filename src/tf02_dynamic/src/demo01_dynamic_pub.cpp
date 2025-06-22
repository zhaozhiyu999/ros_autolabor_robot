#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"
/* 
需求描述:

启动 turtlesim_node,该节点中窗体有一个世界坐标系(左下角为坐标系原点)，
乌龟是另一个坐标系，键盘控制乌龟运动，将两个坐标系的相对位置动态发布。

实现分析:

乌龟本身不但可以看作坐标系，也是世界坐标系中的一个坐标点

订阅 topic名称：turtle1/pose,可以获取乌龟在世界坐标系的 x坐标、y坐标、偏移量以及线速度和角速度

将 pose 信息转换成 坐标系相对信息并发布.消息类型是turtlesim/Pose

  实现流程:
        1.包含头文件
        2.初始化 ROS 节点
        3.创建 ROS 句柄
        4.创建订阅对象
        5.回调函数处理订阅到的数据(实现TF广播)关键
            5-1.创建 TF 广播器
            5-2.创建 广播的数据(通过 pose 设置)
            5-3.广播器发布数据
        6.spin（）
*/
void doPose(const turtlesim::Pose::ConstPtr& pose){
    //获取位置和姿态信息，转换成坐标系相对关系，并发布
    //1.创建TF发布对象
    static tf2_ros::TransformBroadcaster pub;
    //2.组织被发布的数据
    geometry_msgs::TransformStamped ts;

    ts.header.frame_id = "world_frame";
    ts.header.stamp = ros::Time::now();
    ts.child_frame_id = "turtle1";

    ts.transform.translation.x = pose->x;
    ts.transform.translation.y = pose->y;
    ts.transform.translation.z = 0;
    //为子信息中没有四元数，只有欧拉角，所以需要转换一下，且乌龟是2D，仅剩一个z轴旋转的theta
    tf2::Quaternion qtn;
    qtn.setRPY(0,0,pose->theta);
    ts.transform.rotation.x = qtn.getX();
    ts.transform.rotation.y = qtn.getY();
    ts.transform.rotation.z = qtn.getZ();
    ts.transform.rotation.w = qtn.getW();
    //3.发布
    pub.sendTransform(ts);
}




int main(int argc, char *argv[])
{
    /* code */
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"dynamic_pub");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/turtle1/pose",100,doPose);

    ros::spin();

    return 0;
}
