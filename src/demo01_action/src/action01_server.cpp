#include "ros/ros.h"
#include "actionlib/server/simple_action_server.h"
#include "demo01_action/AddIntsAction.h"
/*  
    需求:
        创建两个ROS节点，服务器和客户端，
        客户端可以向服务器发送目标数据N（一个整型数据）
        服务器会计算1到N之间所有整数的和，这是一个循环累加的过程，返回给客户端，
        这是基于请求响应模式的，
        又已知服务器从接收到请求到产生响应是一个耗时操作，每累加一次耗时0.1s，
        为了良好的用户体验，需要服务器在计算过程中，
        每累加一次，就给客户端响应一次百分比格式的执行进度，使用action实现。

    流程:
        1.包含头文件;
        2.初始化ROS节点;
        3.创建NodeHandle;
        4.创建action服务对象;
        5.处理请求(a.解析提交的目标值；b.产生连续反馈；c.最终结果响应)---通过回调函数处理
        6.因为有回调函数，所以还得有spin().

*/
//重命名而已
typedef actionlib::SimpleActionServer<demo01_action::AddIntsAction> Server;


void cb(const demo01_action::AddIntsGoalConstPtr &goal,Server* server){
    //a.获取目标值
    int num = goal->num;
    ROS_INFO("客户端提交的目标值是:%d",num);
    //b.累加并响应连续反馈
    int result = 0;
    ros::Rate rate(10);//通过频率设置休眠时间
    for (int i = 1; i <= num; i++)
    {
        result += i;
        //组织连续数据并发布
        demo01_action::AddIntsFeedback feedback;//连续反馈
        feedback.progress_bar = i / (double)num;//体现num中有多少个数字，作为被出数，整体是百分比即
        server->publishFeedback(feedback);//算作一个调用
        rate.sleep();
    }
    //c.设置最终结果
    demo01_action::AddIntsResult r;
    r.result = result;
    server->setSucceeded(r);
    ROS_INFO("最终结果:%d",r.result);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ROS_INFO("action服务端实现");
    // 2.初始化ROS节点;
    ros::init(argc,argv,"AddInts_server");
    // 3.创建NodeHandle;
    ros::NodeHandle nh;
    // 4.创建action服务对象;
    /*SimpleActionServer(ros::NodeHandle n, 
                        std::string name, 
                        boost::function<void (const demo01_action::AddIntsGoalConstPtr &)> execute_callback, 
                        bool auto_start)四个参数！！！
    */
    // actionlib::SimpleActionServer<demo01_action::AddIntsAction> server(....);
    //bind去绑定回调函数和main，_1是void中第一个参数&goal
    Server server(nh,"addInts",boost::bind(&cb,_1,&server),false);//false：即手动调用该函数来启动函数
    server.start();
    // 5.处理请求,产生反馈与响应;

    // 6.spin().   
    ros::spin();
    return 0;
}
