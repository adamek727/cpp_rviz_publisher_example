#include <sstream>

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Quaternion.h>

static geometry_msgs::Quaternion cube_orient;

visualization_msgs::Marker createCuteCube(float pose) {

    visualization_msgs::Marker marker;
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time();
    marker.ns = "cube";

    marker.id = 0;
    marker.type = visualization_msgs::Marker::CUBE;
    marker.action = visualization_msgs::Marker::ADD;

    marker.pose.position.x = sin(pose);
    marker.pose.position.y = cos(pose);
    marker.pose.position.z = 0.1*sin(5*pose);

    marker.pose.orientation = cube_orient;

    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 0.1;

    marker.color.a = 1.0;
    marker.color.r = 0.0;
    marker.color.g = 1.0;
    marker.color.b = 0.0;

    return marker;
}


void imuCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
    std::cout << msg->orientation.x << " "
              << msg->orientation.y << " "
              << msg->orientation.z << " "
              << msg->orientation.w << std::endl;

    cube_orient = msg->orientation;
}


int main(int argc, char **argv) {

    ros::init(argc, argv, "cpp_rviz_publisher");
    ros::NodeHandle n;

    ros::Publisher vis_pub = n.advertise<visualization_msgs::Marker>( "/cute_cube", 0 );
    ros::Subscriber sub = n.subscribe("/xsens/imu", 0, imuCallback);
    ros::Rate loop_rate(100);

    float pose = 0;
    while (ros::ok()) {
        visualization_msgs::Marker cube = createCuteCube(pose);
        vis_pub.publish( cube );
        pose += 0.01;

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
