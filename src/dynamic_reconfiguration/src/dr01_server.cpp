#include "ros/ros.h"
#include "dynamic_reconfigure/server.h"
#include "dynamic_reconfiguration/drConfig.h"
 /*  
    动态参数服务端: 参数被修改时直接打印
    实现流程:
        1.包含头文件
        2.初始化 ros 节点
        3.创建服务器对象
        4.创建回调对象(使用回调函数，打印修改后的参数)
        5.服务器对象调用回调对象
        6.spin()
*/

void cb(dynamic_reconfiguration::drConfig& config, uint32_t level){
    ROS_INFO("动态参数解析数据:%d,%.2f,%d,%s,%d",
        config.int_param,
        config.double_param,
        config.bool_param,
        config.string_param.c_str(),
        config.list_param
    );
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    // 2.初始化 ros 节点
    ros::init(argc,argv,"dr");
    // 3.创建服务器对象
    dynamic_reconfigure::Server<dynamic_reconfiguration::drConfig> server;
    // 4.创建回调对象(使用回调函数，打印修改后的参数)
    dynamic_reconfigure::Server<dynamic_reconfiguration::drConfig>::CallbackType cbType;
    cbType = boost::bind(&cb,_1,_2);
    // 5.服务器对象调用回调对象
    server.setCallback(cbType);
    // 6.spin()
    ros::spin();
    return 0;
}