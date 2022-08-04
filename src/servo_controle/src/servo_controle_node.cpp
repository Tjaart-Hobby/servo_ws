/*******************************************************************************
 * rostopic pub /set_tilt_degree std_msgs/Int16 '{data: 0}' -1    (data -30 > 30)
 * ******************************************************************************/


#include <ros/ros.h>
#include <sensor_msgs/Joy.h>         // use to subscribe joy topic
#include <std_msgs/UInt16.h>         //use to publish to servo topic
#include <std_msgs/Int16.h>          //used for int16 required tilt topic
ros::Publisher pub;                  //if publisher object not here  
ros::Publisher pub2;                 //you get "not declared in this scope"

    int tilt_val = 0;
    int tilt_val_new =0;
    int pan_val = 90;
    int pan_val_new =0;


/*void stickCallback (const sensor_msgs::Joy::ConstPtr& msg)
{
  for (unsigned i = 0; i < msg->axes.size(); ++i) {
    ROS_INFO("Axis %d is now at position %f", i, msg->axes[i]);
  }
}*/

/*void buttonCallback (const sensor_msgs::Joy::ConstPtr& msg)
{
    for (unsigned i = 0; i < msg->buttons.size(); ++i) {
    ROS_INFO("button %d is now at position %d", i, msg->buttons[i]);
  }
}*/

void servo_moveCallback(const sensor_msgs::Joy::ConstPtr &msg)  //while loops dont play nice
{
  //int a = msg->buttons[2];
  int b = msg->buttons[1];
  if (msg->buttons[2] == 1)  //msg->buttons can be replaced with b varible
  {
    pan_val_new = pan_val + 10;
    pan_val =  pan_val_new;
    if (pan_val > 180)
      pan_val = 180;

    ROS_INFO("panning servo left %d",pan_val);
    std_msgs::UInt16 new_msg;
    new_msg.data = pan_val;
    pub.publish(new_msg);
  }

  else if ( b == 1  )
  {
    pan_val_new = pan_val - 10;
    pan_val =  pan_val_new;
    if (pan_val < 0)
      pan_val = 0;

    ROS_INFO("panning servo right %d",pan_val);
    std_msgs::UInt16 new_msg;
    new_msg.data = pan_val;
    pub.publish(new_msg);
  }
    /*std_msgs::Int32 new_msg;   //having the publish out here > it publish every event
    new_msg.data = pan_val;      //so pressing tilt > publishes the unchanged value
    pub.publish(new_msg);*/
}
/***********************************************************************************
  created while sorting out left and right > keeping it around for up/down tilt
  **********************************************************************************/
 // moved servo tilt below main because tilt_pub() <not declared?>
 void servo2_moveCallback(const sensor_msgs::Joy::ConstPtr &msg)
{
  if (msg->buttons[3] == 1)
  {

    tilt_val_new = tilt_val + 5;
    tilt_val =  tilt_val_new;
    if (tilt_val > 25)
      tilt_val = 25;
    //else ()
      
   

    ROS_INFO("tilting servo up %d",tilt_val);
    std_msgs::Int16 new_msg;
    new_msg.data = tilt_val;
    pub2.publish(new_msg);
  }

  else if ( msg->buttons[0]== 1  )
  {
    tilt_val_new = tilt_val - 5;
    tilt_val =  tilt_val_new;
    if (tilt_val < -25)
      tilt_val = -25;
    //else ()

    ROS_INFO("tilting servo down %d",tilt_val);
    std_msgs::Int16 new_msg;
    new_msg.data = tilt_val;
    pub2.publish(new_msg);
  }

  
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "servo_controle_node");
  ros::NodeHandle nh;
  ros::Rate loop_rate(50);
  /********************
   * subscribers 1 x fro each function
  *********************/
  // ros::Subscriber sub = nh.subscribe("joy",10, stickCallback);    //test function to access joy sticks
  // ros::Subscriber sub2 = nh.subscribe("joy",10, buttonCallback);  //test function to access buttons
  ros::Subscriber sub3 = nh.subscribe("joy", 10, servo_moveCallback);
  ros::Subscriber sub4 = nh.subscribe("joy",10, servo2_moveCallback);

  /*************************
   * publishers 1 x for each function
   ************************/
  pub = nh.advertise<std_msgs::UInt16>("/servo", 10);
  pub2 = nh.advertise<std_msgs::Int16>("/set_tilt_degree", 10);

  ROS_INFO_ONCE("Servo_controle_node Started");

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
}
