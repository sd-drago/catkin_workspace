#include <ros/ros.h>
#include <std_msgs/string.h>
#include <turtlesim/Pose.h>

void poseCallback(const turtlesim::Pose &data){
	x = data.x;
	y = data.y;
	theta = data.theta;
	ROS_INFO("[the location of the robot is %f, %f] and the theta is %f", x, y, theta);
	std::cout << x << " is x " << y << " is y " << theta << " is theta " << std::endl;
}


int main(int argc, char **argv){

	ros::init(argc, argv, "poseDetector")
	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("pose", 1000, poseCallback);

	ros::spin();

	return 0;
}
