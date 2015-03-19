#include <ros/ros.h>
#include <geometry_msgs/Quaternion.h>
#include <keyboard/Key.h>

class Coordinate
{
public:
	Coordinate();

private:
	void callback(const keyboard::Key::ConstPtr& key);

	ros::NodeHandle nh;
	ros::Publisher pub;
	ros::Subscriber sub;

	int i;
};

Coordinate::Coordinate():
i(1)
{
	pub = nh.advertise<geometry_msgs::Quaternion>("coord", 1);
	sub = nh.subscribe<keyboard::Key>("/keyboard/keyup", 10, &Coordinate::callback, this);
}


geometry_msgs::Quaternion quat;

int counter_for_enter = 0;

void Coordinate::callback(const keyboard::Key::ConstPtr& key)
{
    int code = key->code;
    if(code == 271 || code == 13)
    {
        counter_for_enter++;
    }
    if(counter_for_enter==2)
    {
        pub.publish(quat);
        quat.x = 0;
        quat.y = 0;
        
        counter_for_enter = 0;
    }
    if((code != 271 && code != 13) && counter_for_enter==0)
    {
        quat.x = quat.x*10 + (code-48); //48 is the code for 0 in the keyboard above alphabets
    }
    if(code != 271 && code != 13 && counter_for_enter==1)
    {
        quat.y = quat.y*10 + (code-48);
    }
}

 int main(int argc, char** argv)
 {
    ros::init(argc, argv, "coord_node");
    Coordinate coordinate;

    ros::spin();
 }
