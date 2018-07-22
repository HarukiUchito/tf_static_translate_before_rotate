#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <cstdio>
#include <tf/LinearMath/Quaternion.h>
#include <tf/LinearMath/Transform.h>

void out_transform(tf::Transform &q)
{
    ROS_INFO("ox = %10.5f", q.getOrigin().getX());
    ROS_INFO("oy = %10.5f", q.getOrigin().getY());
    ROS_INFO("oz = %10.5f", q.getOrigin().getZ());
    ROS_INFO("rx = %10.5f", q.getRotation().getX());
    ROS_INFO("ry = %10.5f", q.getRotation().getY());
    ROS_INFO("rz = %10.5f", q.getRotation().getZ());
    ROS_INFO("rw = %10.5f", q.getRotation().getW());
}

int main(int argc, char **argv)
{
    ros::init(argc,argv, "static_tf_broadcaster_translate_before_rotate");
    static tf::TransformBroadcaster static_broadcaster;

    if (argc < 9) {
        ROS_INFO("lack of arguments");
        ROS_INFO("args: tf_parent tf_child tr_x tr_y tr_z roll pitch yaw");
        return 0;
    }

    tf::Vector3 trv(atof(argv[3]), atof(argv[4]), atof(argv[5]));
    //-3.80932455, 10.0369571748, 0.0);
    
    tf::Quaternion quat, zero; zero.setRPY(0.0, 0.0, 0.0);
    quat.setRPY(atof(argv[6]), atof(argv[7]), atof(argv[8]));
    //0.0, 0.0, 2.05619);
    tf::Transform ro(quat);
    tf::Transform tr(zero, trv);
    tf::Transform rotr = ro * tr;

/*
    ROS_INFO("ro");
    out_transform(ro);
    ROS_INFO("tr");
    out_transform(tr);
    ROS_INFO("\nrotr");
    out_transform(rotr);
*/

    ros::NodeHandle n;
    ros::Rate r(100);
    while (n.ok()) {
        static_broadcaster.sendTransform(
            tf::StampedTransform(rotr,
                ros::Time::now(), argv[1], argv[2]
            )
        );
        r.sleep();
    }

    return 0;
};