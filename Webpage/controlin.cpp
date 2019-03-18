#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  


#include "ServiceDiscovery.h"

#include "zmq.hpp"



#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace cgicc;

int main (){

  
  boost::uuids::uuid m_UUID=boost::uuids::random_generator()();
  long msg_id=0;
    
   
  zmq::context_t *context=new zmq::context_t(3);
   

  std::vector<Store*> RemoteServices;
  

  std::string address("239.192.1.1");
  //std::stringstream tmp ("5000");

  int port=5000;
  //  tmp>>port;

  ServiceDiscovery *SD=new ServiceDiscovery(address,port,context,320);

  bool running=true;

  zmq::socket_t Ireceive (*context, ZMQ_DEALER);
  Ireceive.connect("inproc://ServiceDiscovery");

  sleep(5);

  zmq::message_t send(256);
  snprintf ((char *) send.data(), 256 , "%s" ,"All NULL") ;

  Ireceive.send(send);

      
  zmq::message_t receive;
  Ireceive.recv(&receive);
  std::istringstream iss(static_cast<char*>(receive.data()));
      
  int size;
  iss>>size;

  RemoteServices.clear();


  for(int i=0;i<size;i++){

    Store *service = new Store;

    zmq::message_t servicem;
    Ireceive.recv(&servicem);

    std::istringstream ss(static_cast<char*>(servicem.data()));
    service->JsonParser(ss.str());

    RemoteServices.push_back(service);

  }


  zmq::message_t tmp;
  Ireceive.recv(&tmp);
 
  /*
      for(int i=0;i<RemoteServices.size();i++){

      std::string ip;
      std::string service;
      std::string status;
      
      //*(it->second)>> output;
      ip=*((*(RemoteServices.at(i)))["ip"]);
      service=*((*(RemoteServices.at(i)))["msg_value"]);
      status=*((*(RemoteServices.at(i)))["status"]);

      std::cout<<"["<<i<<"]  "<<ip<<" , "<<service<<" , "<<status<<std::endl;
    
}
  */
 




  


  /// webform
  
  Cgicc formData;

  cout << "Content-type:text/html\r\n\r\n";
  cout<<"<html>";  
  cout<<" <head> <meta http-equiv=\"refresh\" content=\"60; url=./control.cgi\"> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" /> <title>Remote ToolChain Control</title></head>"; 


  cout<<"  <link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/icon?family=Material+Icons\">";                                                    \

  cout<<"  <script defer src=\"https://code.getmdl.io/1.1.2/material.min.js\"></script>";                                                                  \

  cout<<"<link rel=\"stylesheet\" href=\"https://code.getmdl.io/1.1.2/material.indigo-deep_purple.min.css\" />";                                           \

  cout<<"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  cout<<" <link rel=\"stylesheet\" href=\"styles.css\">";                                                                                                  \

  cout<<"<body>";                                                                                                                                          \

                                                                                                                                                           \

  cout<<"<!-- Always shows a header, even in smaller screens. --> ";                                                                                       \

  cout<<"<div class=\"mdl-layout mdl-js-layout mdl-layout--fixed-header\">";                                                                               \

  cout<<" <header class=\"mdl-layout__header\">";                                                                                                          \

  cout<<"<div class=\"mdl-layout__header-row\">";

  cout<<" <!-- Title -->";                                                                                                                                 \

  cout<<"<span class=\"mdl-layout-title\"> ";                                                                                                              \

  cout<<" Remote Control</span>";                                                                                                                          \

  cout<<" <!-- Add spacer, to align navigation to the right -->";                                                                                          \

  cout<<" <div class=\"mdl-layout-spacer\"></div>";                                                                                                        \

  cout<<" <!-- Navigation. We hide it in small screens. -->";                                                                                              \

  cout<<" <nav class=\"mdl-navigation mdl-layout--large-screen-only\"> ";

  cout<<" <a class=\"mdl-navigation__link\" href=\"../index.html\">Home</a>";                                                                              \

  cout<<" <a class=\"mdl-navigation__link\" href=\"/cgi-bin/control.cgi\">Control</a>";                                                                    \

  cout<<" <a class=\"mdl-navigation__link\" href=\"/cgi-bin/logs.cgi\">Logs</a>";                                                                          \

  cout<<"     <a class=\"mdl-navigation__link\" href=\"/cgi-bin/monitoring.cgi\">Monitoring</a> ";                                                         \

  cout<<"<a class=\"mdl-navigation__link\" href=\"/cgi-bin/SQL.cgi\">SQL</a> ";                                                                            \

  cout<<"   <a class=\"mdl-navigation__link\" href=\"/cgi-bin/data.cgi\">Data</a>     ";                                                                          \

  cout<<"</nav> ";
  cout<<"</div> ";

  cout<<"</header>";                                                                                                                                       \

  cout<<"<div class=\"mdl-layout__drawer\">  ";                                                                                                            \

  cout<<"<img src=\"../ANNIE-logo.png\"  width=\"90%\" height=\"10%\">";                                                                                   \

  cout<<"    <span class=\"mdl-layout-title\"></span>";                                                                                                    \

  cout<<"<div class=\"android-drawer-separator\"></div>";                                                                                                  \

  cout<<"<nav class=\"mdl-navigation\">  ";                                                                                                                \

  cout<<"<a class=\"mdl-navigation__link\" href=\"../index.html\">Home</a> ";
  cout<<" <a class=\"mdl-navigation__link\" href=\"/cgi-bin/control.cgi\">Control</a>  ";                                                                  \

  cout<<" <a class=\"mdl-navigation__link\" href=\"/cgi-bin/logs.cgi\">Logs</a>  ";                                                                        \

  cout<<" <a class=\"mdl-navigation__link\" href=\"/cgi-bin/monitoring.cgi\">Monitoring</a>";                                                              \

  cout<<"  <a class=\"mdl-navigation__link\" href=\"/cgi-bin/SQL.cgi\">SQL</a>     ";                                                                      \

  cout<<"  <a class=\"mdl-navigation__link\" href=\"/cgi-bin/data.cgi\">Data</a>";                                                                                \

  cout<<" </nav>";                                                                                                                                         \

  cout<<" </div>";
      
  cout<<"<main class=\"mdl-layout__content\"> ";                                                                                                           \

  cout<<" <div class=\"page-content\"> ";                                                                                                                  \

  cout<<"<!-- Your content goes here -->";                                                                                                                 \

                                                                                                                                                           \




 
  cout<<"<h1 align=\"center\">Remote Control ToolChain</h1>" ;

  /////////////////////////////////////////////////////////////////////
  cout<<"  <p> <table  border='1' align='center'>"; 

  cout<<"<tr> <th scope='col'><div align='center'>Service ID </div></th> ";
  cout<<"<th scope='col'><div align='center'>Service IP</div></th>";
  cout<<"<th scope='col'><div align='center'>Service Name</div></th>"; 
  cout<<"<th scope='col'><div align'center'>Service Status</div></th> </tr>";

  //cout<<"remote services size "<<RemoteServices.size();
  
  for(int i=0;i<RemoteServices.size();i++){

    std::string ip;
    std::string service;
    std::string status;
    std::string colour;

    ip=*((*(RemoteServices.at(i)))["ip"]);
    service=*((*(RemoteServices.at(i)))["msg_value"]);
    status=*((*(RemoteServices.at(i)))["status"]);
    colour="#00FFFF";

    if (status=="Online")colour="#FF00FF";
    else if (status=="Waiting to Initialise ToolChain")colour="#FFFF00";
    else{
      std::stringstream tmpstatus(status);
      tmpstatus>>status;
      if(status=="ToolChain"){
        tmpstatus>>status;
        if(status=="running")colour="#00FF00";
      }
      status=tmpstatus.str();
    }
    
    cout<<"<tr> <td bgcolor=\""<<colour<<"\">["<<i<<"]</td>"; 
    cout<<"<td bgcolor=\""<<colour<<"\">"<<ip<<"</td>";
    cout<<"<td bgcolor=\""<<colour<<"\">"<<service<<"</td> ";
    cout<<"<td bgcolor=\""<<colour<<"\">"<<status<<"</td> </tr> ";
    
    
    //std::cout<<"["<<i<<"]  "<<ip<<" , "<<service<<" , "<<status<<std::endl;
    
  }
  

  cout<<"</table> <p>";


  ///////////////////////////////////////////////////////////////

  cout<<"  <table  border=\"0\" align=\"center\"><tr><td align='center'>";

  cout<<"<form action=\"/cgi-bin/control.cgi\" method=\"post\">"; 
  cout<<"<input type=\"submit\" value=\"Refresh\" align=\"center\" />";
  cout<<"</form></td>";

  cout<<"</tr> </table> <p>";


  /////////////////////////////////////////////////////////


  if(RemoteServices.size()>0){
    cout<<"<form action=\"/cgi-bin/controlin.cgi\" method=\"post\" >"; 


    cout<<"  <table  border=\"0\" align=\"center\"><tr><td align='center'>";
    cout<<"<td><div align='center'>";
    cout<<"<select name=\"UUID\">";

    for(int i=0;i<RemoteServices.size();i++){

      std::string ip;
      std::string uuid;
      std::string service;
      std::string status;
      
      //*(it->second)>> output;
      ip=*((*(RemoteServices.at(i)))["ip"]);
      service=*((*(RemoteServices.at(i)))["msg_value"]);
      status=*((*(RemoteServices.at(i)))["status"]);
      uuid=*((*(RemoteServices.at(i)))["uuid"]);

      if (i==0) cout<<" <option value=\""<<uuid<<"\" selected>["<<i<<"]</option>";  

      else cout<<"<option value=\""<<uuid<<"\">["<<i<<"]</option>"; 

    }
    cout<<"</select>";
    //cout<<"</td><td>";
    cout<<"<input type=\"text\" name=\"command\"> ";
    // cout<<"</td><td>";
    cout<<"<input type=\"submit\" value=\"Send Command\" /></form>";
    cout<<"</td></tr></table></form><p>";
    
  }


  /////////////////////////////////////////////////////////////
    
  cout<<"  <table  border=\"1\" align=\"center\">";
  cout<<"  <td>";

    
    
   
  std::string UUID;
  std::string command;
  std::string var1;
    
  form_iterator fi = formData.getElement("UUID");  
  if( !fi->isEmpty() && fi != (*formData).end()) {  
    UUID= **fi;
    form_iterator ti = formData.getElement("command");  
    if( !ti->isEmpty() && ti != (*formData).end()) {  
      command= **ti; 

      std::stringstream tmp(command);
      tmp >> command>> std::ws;
      getline(tmp, var1);
             
      int ServiceNum=-1;
       
      for(int i=0;i<RemoteServices.size();i++){
	 
	std::string uuid;
	uuid=*((*(RemoteServices.at(i)))["uuid"]);
	if(uuid==UUID)ServiceNum=i;
      }
       
      if(ServiceNum!=-1){
	 
	zmq::socket_t ServiceSend (*context, ZMQ_REQ);
	int a=120000;
	ServiceSend.setsockopt(ZMQ_RCVTIMEO, a);
	ServiceSend.setsockopt(ZMQ_SNDTIMEO, a);
	 
	std::stringstream connection;
	connection<<"tcp://"<<*((*(RemoteServices.at(ServiceNum)))["ip"])<<":"<<*((*(RemoteServices.at(ServiceNum)))["remote_port"]);
	ServiceSend.connect(connection.str().c_str());
	 
	 
	zmq::message_t send(256);
	 
	 
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	std::stringstream isot;
	isot<<boost::posix_time::to_iso_extended_string(t) << "Z";

	msg_id++;
	Store bb;
	 
	bb.Set("uuid",m_UUID);
	bb.Set("msg_id",msg_id);
	*bb["msg_time"]=isot.str();
	*bb["msg_type"]="Command";
	bb.Set("msg_value",command);
	bb.Set("var1",var1);
	 
	std::string tmp="";
	bb>>tmp;
	snprintf ((char *) send.data(), 256 , "%s" ,tmp.c_str()) ;
	 

	cout<<"Sending Command {"<<command<<"} to service ["<<ServiceNum<<"] <br>";
	ServiceSend.send(send);
	 
	 
	zmq::message_t receive;
	ServiceSend.recv(&receive);
	std::istringstream iss(static_cast<char*>(receive.data()));
	 
	std::string answer;
	answer=iss.str();
	Store rr;
	rr.JsonParser(answer);
	if(*rr["msg_type"]=="Command Reply") std::cout<<"Service ["<<ServiceNum<<"] Reply: "<<*rr["msg_value"];
	 
      }
       
      else std::cout<< "Service does not exist"<<std::endl<<std::endl;
       

       
       
    }
    else{
      cout << "No Command Entered " << endl;  
    }
  }
  else{  cout << "Cannot Send Command No Services Found" << endl;  
  }  
   
   
   
  cout<<" <br></td>  </table>";   
  cout << "<br/>\n";

  cout<<"<footer class=\"mdl-mega-footer\">";                                                                                                              \


                                                                                                                                                           \

  cout<<"          <div class=\"mdl-mega-footer--top-section\"> ";
                                                                                                                                                           \

  cout<<"         <div class=\"mdl-mega-footer--right-section\">        ";
  cout<<"     <a class=\"mdl-typography--font-light\" href=\"#top\"> ";                                                                                    \

                                                                                                                                                           \

  cout<<"           Back to Top                                            ";                                                                              \

                                                                                                                                                           \

  cout<<"<i class=\"material-icons\">expand_less</i>    ";
                                                                                                                                                           \

  cout<<"   </a>";                                                                                                                                         \
  cout<<"</div>";
  cout<<" </div>";
  cout<<"       <div class=\"mdl-mega-footer--middle-section\">";                                                                                          \


  cout<<" <p class=\"mdl-typography--font-light\">ANNIE Collaboration © 2016 </p>";
                                                                                                                                                           \

  cout<<" <p class=\"mdl-typography--font-light\">Created by Dr. B.Richards (b.richards@qmul.ac.uk)</p>";
                                                                                                                                                           \

  cout<<" </div>";

  cout<<"     </footer> ";                                                                                                                                 \

  cout<<"</div>";                                                                                                                                          \

  cout<<"  </main>        ";                                                                                                                               \

  cout<<"</div>   ";

   
  cout << "</body>\n";
  cout << "</html>\n";
   
  return 0;
}
