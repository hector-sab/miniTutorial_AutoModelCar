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

Recuerda que este archivo debe encontrarse en la carpeta 

WORKSPACE/src/PAQUETE/src/

y debe estar agregado en el CMakeLists.txt

*/

class SubscriberClass
{
  //Crea objeto que se encargará de la communicación entrante y saliente del nodo
  ros::NodeHandle node_obj;
  // Crea subscriptor que activa la función numero_callback cada que un mensaje nuevo es resivido en el
  // topico numero_int
  ros::Subscriber numero_sub = node_obj.subscribe("numero_int",1000, &SubscriberClass::numero_callback,this);
  // Crea subscriptor que activa la función palabra_callback cada que un mensaje nuevo es resivido en el
  // topico palabra_str
  ros::Subscriber numero_sub = node_obj.subscribe("palabra_str",1000, &SubscriberClass::palabra_callback,this);

private:
  int suma = 0;

public:
  void numero_callback(const std_msgs::Int32::ConstPtr& msg)
  {
    // Obtiene el valor del mensaje resivido y lo almacena en una nueva variable
    int numero_resivido = msg->data;

    // suma los valores resividos
    suma += numero_resivido;

    //Muestra en consola la suma
    std::cout<< "Numero resivido sumado: "<< suma << std::endl;
  }

  void palabra_callback(const std_msgs::String::ConstPtr& msg)
  {
    // Obtiene el valor del mensaje resivido y lo almacena en una nueva variable
    std::string palabra_resivida = msg->data.c_str();

    //Muestra en consola la suma
    std::cout << "Palabra resivida: " << palabra_resivida << std::endl;
  }
};

int main( int argc, char **argv ) 
{
  //Inicializa el nodo con el nombre subscriber_node
  ros::init(argc, argv, "subscriber_node");
  SubscriberClass subs;
  // Hace que se haga un bucle "infinito" en el objeto
  ros::spin();
  return 0;
}
