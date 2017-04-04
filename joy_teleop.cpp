/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 * 	 /|_||_\`.	Equipo Autonomos Mini
 * 	(   _    _ _\  	CINVESTAV Saltillo - Robótica y Manufactura Avanzada
 * 	=`-(_)--(_)-'   
 * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 
- MaryPaz Rico Fernández
- Alvaro Paz Anaya
- Johnatan Obregón Flores
- Alejando Maldonado Ramírez
- Noel Cortés Pérez

Nodo para leer el joystick (necesita el paquete "joy") y publicar en el tópico
 
    -  /manual_control/steering 
    -  /manual_control/speed
    
 NOTAS:
 Es necesario instalar el paquete "joy" para poder trabajar con controles USB:

    $ sudo apt-get install ros-indigo-joy

Información de Instalación y Configuración:

    http://wiki.ros.org/joy/Tutorials/ConfiguringALinuxJoystick

EL Launch file para arracar el nodo joy y el publicador en los tópicos los pueden bajar de:

    https://raw.githubusercontent.com/noelcortes/joystick-control/master/controller.launch

 * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
*/

#include "ros/ros.h"
#include "std_msgs/Int16.h"
#include <iostream>
#include <sensor_msgs/Joy.h>	

using namespace std_msgs;

float numbers;

//Callback of the topic /numbers
void number_callback(const std_msgs::Int16::ConstPtr& msg)
{
	//ROS_INFO("Recieved  [%d]",msg->data);
	 numbers=msg->data;
  
}

class TeleopCar
{
    
    /*Creamos el objeto para manejar el nodo.*/
    ros::NodeHandle nh_;
    
    int fwd_vel_linear_, bkw_vel_linear_, angulo_;
    double v_scale, a_scale_;
    
    /*Suscriptor de comandos del joystick y publicador de velocidad.*/
    ros::Publisher steering_pub_;
    ros::Publisher velocity_pub_;
    ros::Subscriber joy_sub_;
    
    /*Constructor de la clase*/
    public:
    TeleopCar():
    fwd_vel_linear_(1),
    bkw_vel_linear_(2),
    angulo_(3)
    {
	nh_.param("axis_fwd_vel_linear_", fwd_vel_linear_, fwd_vel_linear_);
	nh_.param("axis_bkw_vel_linear_", bkw_vel_linear_, bkw_vel_linear_);
	nh_.param("axis_angulo", angulo_, angulo_);
	nh_.param("scale_vel_lineal", v_scale, v_scale);
	nh_.param("scale_angulo", a_scale_, a_scale_);
	
        /*El método va publicar en el tema "out"*/
        steering_pub_ = nh_.advertise<std_msgs::Int16>("manual_control/steering", 1);
	velocity_pub_ = nh_.advertise<std_msgs::Int16>("manual_control/speed", 1);

        /*El método va a tomar imágenes del tema "joy"*/
        joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopCar::joyCallback, this);
	/* Here we take the data from the joystick and manipulate it by scaling it and using
	independent axes to control the linear and angular velocities of the turtle. Finally
	we publish the prepared message.*/
    }
    
        ~TeleopCar()
    {
        //cv::destroyWindow(WINDOW);
    }
    
    
    /*Método callBack para cuando se reciba una comando del control*/
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
    {
      int steering;
      int velocity;
      steering = a_scale_*(joy->axes[angulo_]+1)*0.5;
      velocity = v_scale*(joy->axes[fwd_vel_linear_]-joy->axes[bkw_vel_linear_]);
      std_msgs::Int16 steering_cmd, velocity_cmd;
      steering_cmd.data = steering;
      velocity_cmd.data = velocity;
      steering_pub_.publish(steering_cmd);
      velocity_pub_.publish(velocity_cmd);
    }
};



int main(int argc, char **argv)
{
  ros::init(argc, argv, "Teleop_Car");
  TeleopCar Teleop_Car;
  ros::spin();
}


