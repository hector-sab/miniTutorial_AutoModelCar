//Librerias de ros necesarias:
#include "ros/ros.h"
#include "sensor_msgs/Image.h"

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

//Librerias de opencv necesarias:
#include "opencv2/opencv.hpp" 
#include "opencv2/core/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"

//Librerias estandar necesarias
#include <iostream>
#include <stdio.h> /*printf, scanf, puts, NULL*/
#include <stdlib.h> /* srand, rand*/
#include <cmath>

#include <vector>

namespace enc = sensor_msgs::image_encodings;

/*
Este programa modifica la imagen RGB a escala de grises, y edita pixeles tanto en la 
imagen RGB como en la que está en escala de grises
*/


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



//Leer primero funcion main (abajo)... luego leer esto
class Read
{
  //crea objeto para manejar los mensajes de entrada y salida
  ros::NodeHandle node_obj;
  //crea objeto para manejar las imagenes de entrada y salida
  image_transport::ImageTransport it;

  //crea un subscriptor para las imagenes
  image_transport::Subscriber rgb_sub;
  //crea dos publicador de imagenes
  image_transport::Publisher rgb_pub;
  image_transport::Publisher rgb_pub2;
  image_transport::Publisher rgb_pub3;

public:
  /*
  Innicializa el subscriptor para que lea los mensajes 
  del topico /app/camera/rgb/image_raw . Cuando detecta un mensaje nuevo,
  ejecuta la funcion rgb_callback
  
  publica las imagenes modificadas en los tópicos rgb/salida, rgb/salida2 
  y rgb/salida3
  salida = escala de grises
  salida2 = binarizada
  salida3 = color
  */

  Read(): it(node_obj)
  {
    //Cuando hay una nueva imagen disponible, la funcion rgb_callback se ejecuta
    rgb_sub = it.subscribe("/app/camera/rgb/image_raw", 1, &Read::rgb_callback, this);
    
    //Publica las imagenes modificadas, puedes verlas en rviz:
    rgb_pub = it.advertise("/rgb/salida",1);
    rgb_pub2 = it.advertise("/rgb/salida2",1);
    rgb_pub3 = it.advertise("/rgb/salida3",1);
  }

  //Crea la funcion rgb_callback
  void rgb_callback(const sensor_msgs::ImageConstPtr& msg)
  {
    //Objeto para leer la imagen
    cv_bridge::CvImagePtr cv_ptr;

    //Si no hay error la imagen se lee y se peude accesar a ella a traves de cv_ptr
    try 
    {
      cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    //creamos un objeto del tipo cv::Mat para usar mas facilmente la imagen
    cv::Mat image = cv_ptr->image;

    //lee cuantas columnas y cuantas hileras tiene la imagen
    int rows = image.rows; // image.size[0];
    int cols = image.cols; // image.size[1];

    //crea contenedor para almacenar la imagen en escala de grises
    cv::Mat gray;
    //convierte la imagen de color a grises
    cv::cvtColor(image, gray, CV_BGR2GRAY);

    //crea contenedor
    cv::Mat thresh;
    //convierte a una imagen binarizada
    cv::adaptiveThreshold(gray,thresh,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,15,-5);

    //vamos a editar un poco los pixeles de las imagenes de color y la de escala de grises
    for(int i=50; i<70; i++)
    {
      for(int j=0; j<cols; j++)
      {
        //accesa al pixel en escala de grises
        gray.at<uchar>(i,j) = 0;
        //accesa al pixel en rgb (colors)
        image.at<cv::Vec3b>(i,j) = 255;
      }
    }

    //crea mensage para almacenar imagen para enviar al topico
    sensor_msgs::ImagePtr out_msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", gray).toImageMsg();
    //envia al topico
    rgb_pub.publish(out_msg);

    sensor_msgs::ImagePtr out_msg2 = cv_bridge::CvImage(std_msgs::Header(), "mono8", thresh).toImageMsg();
    rgb_pub2.publish(out_msg2);

    sensor_msgs::ImagePtr out_msg3 = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
    rgb_pub3.publish(out_msg3);
  }

};

int main( int argc, char **argv ) 
{
  //Inicializa nodo llamandolo read_cam
  ros::init(argc, argv, "read_cam");
  //crea objeto de tipo Read, llamandolo reader
  Read reader;
  //Crea un bucle en el objeto
  ros::spin();
  return 0;
}
