#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <cmath>

double width;
turtlesim::Pose curr_pos;
turtlesim::Pose waypoint;
geometry_msgs::Twist speed;
int x = 1;

void get_width(){
	ROS_INFO("Enter the width of the grid:\n");
	std::cin >> width;
}

void get_pose(const turtlesim::Pose &data){
	curr_pos.x = data.x;
	curr_pos.y = data.y;
	curr_pos.theta = data.theta;
}

geometry_msgs::Twist speed_gen(turtlesim::Pose des_pose, turtlesim::Pose pose){
        geometry_msgs::Twist speed;

        float kp_x = 2;
        float kd_x = 1;
        float kp_y = 2;

        speed.linear.x = kp_x*(des_pose.x - pose.x) - kd_x * pose.linear_velocity;
        speed.linear.y = kp_y*(des_pose.y - pose.y);

        return speed;
}

bool check_condition(turtlesim::Pose waypoint, turtlesim::Pose curr){
	bool flag = true;
	if(abs(curr.x - waypoint.x) < 0.1 && abs(curr.y - waypoint.y) < 0.1)flag = false;
	return flag;
}

turtlesim::Pose update_waypoint(int *x, turtlesim::Pose waypoint, double width, turtlesim::Pose curr){
	switch(*x){
		case 1:{
		      	waypoint.x = (curr.x == 0)?(curr.x - 9 + 10):curr.x - 9;
			waypoint.y = ( curr.y == 0)?(curr.y + 10):curr.y ; 
			*x = 3;	
	 		break;
		}
		case 2:{

		      	waypoint.x = (curr.x == 0)?(curr.x + 9 + 10):curr.x + 9;

			waypoint.y = ( curr.y == 0)?(curr.y + 10):curr.y; 
			*x = 3;
 			break;
		}
		case 3:{
		      	waypoint.x = (curr.x == 0)?(curr.x + 10):curr.x ;

			waypoint.y = ( curr.y == 0)?(curr.y - width + 10):(curr.y - width) ; 
			*x = (waypoint.x < 5)?2:1;
 			break;
		}
	}
	ROS_INFO("waypoint: x=%f, y=%f ", waypoint.x, waypoint.y);
	ROS_INFO("curr: x=%f, y=%f ", curr.x, curr.y);
	return waypoint;		       
}

int main(int argc, char **argv){

	ros::init(argc, argv, "grid_pattern");

	ros::NodeHandle n;

	ros::Subscriber grid_sub = n.subscribe("turtle1/pose", 1000, get_pose);
	ros::Publisher grid_pub = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);

	ros::Rate rate_loop(10);

	

	get_width();
	waypoint = update_waypoint(&x, waypoint, width, curr_pos);

	while(ros::ok()){
	
		if(check_condition(waypoint, curr_pos)){
			
		
			if(waypoint.x < 0.0001 || waypoint.x > 11.00009 || waypoint.y > 11.0009 || waypoint.y < 0.001)break;
			speed = speed_gen(waypoint, curr_pos);
			grid_pub.publish(speed);

		
			ros::spinOnce();
			rate_loop.sleep();
		}
		else waypoint = update_waypoint(&x, waypoint, width, curr_pos);
	}
	
	return 0;
}	
