//Cosas necesarios:
//Principal de ros
#include "ros/ros.h"
//Contiene el tipo de mensaje para Int
#include "std_msgs/Int32.h"
//Contiene el tipo de mensaje para Strings
#include "std_msgs/String.h"

#include "string"

/*
Recuerda: para ejecutar un programa (nodo), primero tienens que ejecutar roscore, en otra 
pestaña diferente dar source devel/setup.bash a tu workspace y dentro de ella usar
rosrun NOMBRE_DEL_PAQUETE NOMBRE_DEL_EJECUTABLE

Eh aquí donde está lo padre de tmux, una sola ventana, multiples terminales.

Si tienes varios ejecutables dentro de un paquete, y necesitas que se ejecuten al
mismo tiempo, busca sobre los archivos launch. El libro Mastering ROS habla de ellos

*/

//Función principal
int main(int argc, char **argv)
{
  //Inicializa el nodo con el nombre publisher_test
  ros::init(argc, argv, "publisher_test");

  //Crea objeto para manejar la comunicación entrante y saliente, de y hacia los topicos
  ros::NodeHandle node_obj;

  //Objeto que publica los mensajes de tipo Int32 al tópico numero_int
  ros::Publisher numero_pub = node_obj.advertise<std_msgs::Int32>("numero_int",1000);
  //Objeto que publica los mensajes de tipo String al tópico palabra_str
  ros::Publisher palabra_pub = node_obj.advertise<std_msgs::String>("palabra_str",1000);
  
  //Indica la velocidad del loop
  ros::Rate loop_rate(10);

  int count = 0;
  //Mientras ROS este funcionando.... (?)
  while(ros::ok())
  {
    //Objeto de tipo std_msgs::Int32. Tiene un miembro llamado data, donde se inserta
    // el número
    std_msgs::Int32 msg;
    msg.data = count;
    //Publica el mensaje al tópico numero_int
    numero_pub.publish(msg);

    //Muestra en consola el contenido del msj
    //ROS_INFO("hey %d",count);
    ROS_INFO("%d",msg.data);
    
    std::string mensajeStr = "Wololo!!!";
    std_msgs::String msg2;
    msg2.data = mensajeStr;
    palabra_pub.publish(msg2);

    std::cout << msg2->data.c_str() << std::endl;

    count++;


    if(count>1000)
    {
      break;
    }
    ros::spinOnce();
    //Hace que el loop se realice a la velocidad que indicamoms
    loop_rate.sleep();
  } 
}