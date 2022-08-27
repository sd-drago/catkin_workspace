#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <cmath>

turtlesim::Pose des;
turtlesim::Pose current_pose;
geometry_msgs::Twist vel;

double deg2rad(double degree){
	double pi=3.14159;
	return (degree * (pi/180));
}

void  currentPose(const turtlesim::Pose &data)
{
	current_pose.x = data.x;
	current_pose.y = data.y;
	current_pose.theta = data.theta;

}

geometry_msgs::Twist speed_gen(turtlesim::Pose des_pose, turtlesim::Pose pose){
	geometry_msgs::Twist speed;

	float kp_x = 1;
	float kd_x = 1;
	float kp_y = 1;

	speed.linear.x = kp_x*(des_pose.x - pose.x) - kd_x * pose.linear_velocity;
	speed.linear.y = kp_y*(des_pose.y - pose.y);

	return speed;
}


void getPose(){
	ROS_INFO("Enter your destination in the following format\nx:");
	std::cin >> des.x;
	ROS_INFO("\ny: ");
	std::cin >> des.y;
	ROS_INFO("\nphi: ");
	double temp;
	
	std::cin >> temp;	
	des.theta = deg2rad(temp);
}

bool check_condition(turtlesim::Pose des, turtlesim::Pose curr){
	bool flag =true;
	if(abs(des.x - curr.x) < 0.1  && abs(des.y - curr.y <0.1)  /*&& des.theta - curr.theta >1*/ )flag = false;

	return flag;
}

bool check_angle(turtlesim::Pose des, turtlesim::Pose curr){
	bool flag = true;
	if(abs(des.theta - curr.theta) < 0.01)flag = false;
	return flag;
}

geometry_msgs::Twist angular_speed(turtlesim::Pose des, turtlesim::Pose curr){
	geometry_msgs::Twist speed;
	speed.angular.z = std::atan2(des.theta - curr.theta, 1);
	return speed;
}	

int main(int argc, char **argv)
{
	ros::init(argc, argv, "controller");

	ros::NodeHandle n;


	ros::Subscriber contoller_sub = n.subscribe("turtle1/pose" , 1000, currentPose);
	ros::Publisher controller_pub = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);

	ros::Rate loop_rate(10);

	getPose();
	while(ros::ok()) 
	{
		while(check_condition(des, current_pose)){
			vel = speed_gen(des, current_pose);
			controller_pub.publish(vel);

			ros::spinOnce();
			loop_rate.sleep();
		}

		while(check_angle(des, current_pose)){
			vel  = angular_speed(des, current_pose);			
			controller_pub.publish(vel);
			ros::spinOnce();
			loop_rate.sleep();
		}
		if(!check_condition(des, current_pose) && !check_angle(des, current_pose) )break;
	}

	return 0;
}
