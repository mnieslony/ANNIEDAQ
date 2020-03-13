#include "HVComs.h"

HVComs::HVComs():Tool(){}


bool HVComs::Initialise(std::string configfile, DataModel &data){

  if(configfile!=""){
    m_variables.Initialise(configfile);
    m_variables.Get("Voltage_Change",VChange);
    m_variables.Get("Voltage_Start",VStart);
    m_variables.Get("Voltage_Stop",VStop);
    m_variables.Get("Voltage_Step",VStep);
    VCurrent=VStart;
  }
  
  //m_variables.Print();

  m_data= &data;

  m_variables.Get("HVIP",HVIP);
  m_variables.Get("HVPort",HVPort);
  
  HV= new zmq::socket_t(*(m_data->context), ZMQ_DEALER);
  std::stringstream address;
  address<<"tcp://"<<HVIP<<":"<<HVPort;
  HV->connect(address.str().c_str());

  HVin[0].socket=*HV;
  HVin[0].fd=0;
  HVin[0].events=ZMQ_POLLIN;
  HVin[0].revents=0;
  HVout[0].socket=*HV;
  HVout[0].fd=0;
  HVout[0].events=ZMQ_POLLOUT;
  HVout[0].revents=0;
  
  // cleaning up output
  bool clean=true;
  while (clean){
    zmq::poll (&HVin[0], 1, 6000);
    if (HVin[0].revents & ZMQ_POLLIN) {
      zmq::message_t message;
      HV->recv(&message);
    }
    else clean=false;
  }
  
  zmq::poll (&HVout[0], 1, 10000);
  if (HVout[0].revents & ZMQ_POLLOUT) {
    Store bb;
    
    bb.Set("msg_id",1);
    bb.Set("msg_type","run");
    bb.Set("run",m_data->RunNumber);
    bb.Set("subrun",m_data->SubRunNumber);
    
    std::string tmp="";
    bb>>tmp;
    zmq::message_t send(tmp.length()+1);
    snprintf ((char *) send.data(), tmp.length()+1 , "%s" ,tmp.c_str()) ;
    HV->send(send);
    bb>>m_data->InfoMessage;
    
    
    zmq::poll (&HVin[0], 1, 10000);
    if (HVin[0].revents & ZMQ_POLLIN) {  
      //std::cout<<"sent "<<tmp<<std::endl;
      zmq::message_t message;
      HV->recv(&message);
      
      //char *test;
      // test = (static_cast<char*>(message.data()));
      std::istringstream iss(static_cast<char*>(message.data()));
      //test=iss.str();                                                                                                                                                                                                  
      //std::cout<<"message received: "<<iss.str()<<std::endl;//<<" size = "<<message.size()<<std::endl;
      //std::cout<<"message received2: "<<test<<std::endl;

      m_data->InfoMessage+=iss.str();

      zmq::poll (&HVin[0], 1, 50000);
      if (HVin[0].revents & ZMQ_POLLIN) {
	
	HV->recv(&message);
	
	//char *test;
	// test = (static_cast<char*>(message.data()));
	std::istringstream iss2(static_cast<char*>(message.data()));
	//test=iss.str();			\
	
	//std::cout<<"message received: "<<iss2.str()<<std::endl;
	
	m_data->InfoTitle="HVComs";
	m_data->InfoMessage+=iss2.str();
	
	m_data->Stores["RunInformation"]->Set("HVComs",m_data->InfoMessage);
	std::cout<<"HVComs"<<m_data->InfoMessage<<std::endl;

	//	m_data->GetTTree("RunInformation")->Fill();
      }
      else {
	std::cout<<"reply from HV time out"<<std::endl;
	//return false;
      }
    }
    else {
      std::cout<<"akn reply from HV time out"<<std::endl;
      //return false;
    }
  }
  else {
    std::cout<<"HV time out for sending message"<<std::endl;
    //    return false;
  }
  
  return true;
}


bool HVComs::Execute(){
  std::cout<<"y1"<<std::endl;
  if(m_data->Restart==1){
    std::cout<<"y2"<<std::endl;
    Finalise();
    std::cout<<"y2.1"<<std::endl;
  }
  else if(m_data->Restart==2){
    std::cout<<"y3"<<std::endl;
    Initialise("",*m_data);
    std::cout<<"y4"<<std::endl;
    if(VChange){
      std::cout<<"y5"<<std::endl;
      // cleaning up output
      bool clean=true;
      while (clean){
	zmq::poll (&HVin[0], 1, 6000);
	if (HVin[0].revents & ZMQ_POLLIN) {
	  zmq::message_t message;
	HV->recv(&message);
	}
	else clean=false;
      }
      std::cout<<"y6"<<std::endl;
      zmq::poll (&HVout[0], 1, 10000);
      std::cout<<"y7"<<std::endl;
      if (HVout[0].revents & ZMQ_POLLOUT) {
	Store bb;
	std::cout<<"y8"<<std::endl;
	bb.Set("msg_id",1);
	bb.Set("msg_type","write");
	//bb.Set("msg_property","vset");
	//bb.Set("00-02-15","1450");
	//"msg_type":"write", "msg_property":"imax", "ACTIVE":"250"
	bb.Set("msg_property","vset");
	if(VCurrent<VStop)VCurrent+=VStep;
	bb.Set("TANK",VCurrent);
	std::string tmp="";
	bb>>tmp;
	zmq::message_t send(tmp.length()+1);
	snprintf ((char *) send.data(), tmp.length()+1 , "%s" ,tmp.c_str()) ;
	HV->send(send);
	bb>>m_data->InfoMessage;
	
	std::cout<<"y9"<<std::endl;
	zmq::poll (&HVin[0], 1, 10000);
	std::cout<<"y10"<<std::endl;
	if (HVin[0].revents & ZMQ_POLLIN) { 
	  std::cout<<"y11"<<std::endl; 
	  //std::cout<<"sent "<<tmp<<std::endl;
	  zmq::message_t message;
	  HV->recv(&message);
	  
	  //char *test;
	  // test = (static_cast<char*>(message.data()));
	  std::istringstream iss(static_cast<char*>(message.data()));
	  //test=iss.str();                                                                                                                                                                                                  
	  std::cout<<"message received: "<<iss.str()<<std::endl;//<<" size = "<<message.size()<<std::endl;
	  //std::cout<<"message received2: "<<test<<std::endl;
	  
	  m_data->InfoMessage+=iss.str();
	  
	  std::cout<<"y12"<<std::endl;
	  zmq::poll (&HVin[0], 1, 50000);
	  std::cout<<"y13"<<std::endl;
	  if (HVin[0].revents & ZMQ_POLLIN) {
	    std::cout<<"y14"<<std::endl;
	    HV->recv(&message);
	    
	    //char *test;
	    // test = (static_cast<char*>(message.data()));
	  std::istringstream iss2(static_cast<char*>(message.data()));
	  //test=iss.str();			\
	  
	  //std::cout<<"message received: "<<iss2.str()<<std::endl;
	  
	  m_data->InfoTitle="HVComs";
	  m_data->InfoMessage+=iss2.str();
	  
	  m_data->Stores["RunInformation"]->Set("HVComs",m_data->InfoMessage);
	  std::cout<<"HVComs"<<m_data->InfoMessage<<std::endl;
	  
	  //	m_data->GetTTree("RunInformation")->Fill();
	  }
	  else {
	    std::cout<<"reply from HV time out"<<std::endl;
	    //return false;
	  }
	}
	else {
	  std::cout<<"akn reply from HV time out"<<std::endl;
	  //return false;
	}
      }
      else {
	std::cout<<"HV time out for sending message"<<std::endl;
	//    return false;
      }
    }
  }
  
  return true;
}


bool HVComs::Finalise(){
    
  bool clean=true;
  while (clean){
    zmq::poll (&HVin[0], 1, 6000);
    if (HVin[0].revents & ZMQ_POLLIN) {
      zmq::message_t message;
      HV->recv(&message);
    }
    else clean=false;
  }
  
  zmq::poll (&HVout[0], 1, 10000);
  if (HVout[0].revents & ZMQ_POLLOUT) {
    
    Store bb;
    
    bb.Set("msg_id",2);
    bb.Set("msg_type","get");
    bb.Set("msg_property","event");
    bb.Set("XXX","XXX");
    
    std::string tmp="";
    bb>>tmp;
    zmq::message_t send(tmp.length()+1);
    snprintf ((char *) send.data(), tmp.length()+1 , "%s" ,tmp.c_str()) ;
    HV->send(send);
    
    //std::cout<<"sent "<<tmp<<std::endl;
    bb>>m_data->InfoMessage;
    
    zmq::poll (&HVin[0], 1, 10000);
    if (HVin[0].revents & ZMQ_POLLIN) {
      
      zmq::message_t message;
      HV->recv(&message);
      // char *test;
      //  test = (static_cast<char*>(message.data()));
      std::istringstream iss(static_cast<char*>(message.data()));
      //test=iss.str();                                                                                                                                         
      
      //std::cout<<"message received: "<<iss.str()<<std::endl;//<<" size = "<<message.size()<<std::endl;
      //  std::cout<<"message received2: "<<test<<std::endl;
    
      m_data->InfoMessage+=iss.str();      

      zmq::poll (&HVin[0], 1, 10000);
      if (HVin[0].revents & ZMQ_POLLIN) {
	
	HV->recv(&message);
	// char *test;
	//  test = (static_cast<char*>(message.data()));
	std::istringstream issb(static_cast<char*>(message.data()));
	//test=iss.str();
	
	//std::cout<<"message received: "<<issb.str()<<std::endl;
	
	m_data->InfoMessage+=issb.str();

	m_data->InfoTitle="HV event";
	m_data->Stores["RunInformation"]->Set("HVevent", m_data->InfoMessage);
	std::cout<<"HVevent"<<m_data->InfoMessage<<std::endl;

	//	m_data->GetTTree("RunInformation")->Fill();

	
	zmq::poll (&HVout[0], 1, 10000);
	if (HVout[0].revents & ZMQ_POLLOUT) {
	  
	  bb.Set("msg_id",3);
	  bb.Set("msg_property","error");
	  
	  tmp="";
	  bb>>tmp;
	  zmq::message_t send2(tmp.length()+1);
	  snprintf ((char *) send2.data(), tmp.length()+1 , "%s" ,tmp.c_str()) ;
	  HV->send(send2);
	  //std::cout<<"sent "<<tmp<<std::endl;
	  
	  bb>>m_data->InfoMessage;
	  
	  zmq::poll (&HVin[0], 1, 10000);
	  if (HVin[0].revents & ZMQ_POLLIN) {
	    
	    zmq::message_t message2;
	    HV->recv(&message2);
	    //char *test;
	    //test = (static_cast<char*>(message2.data()));
	    std::istringstream iss2(static_cast<char*>(message2.data()));
	    //test=iss.str();                                                                                                                                         
	    
	    // std::cout<<"message received: "<<iss2.str()<<std::endl;//<<" size = "<<message2.size()<<std::endl;
	    //std::cout<<"message received2: "<<test<<std::endl;
	    
	    m_data->InfoMessage+=iss2.str();
	    
	    zmq::poll (&HVin[0], 1, 10000);
	    if (HVin[0].revents & ZMQ_POLLIN) {
	      
	      HV->recv(&message2);
	      //char *test;
	      //test = (static_cast<char*>(message2.data()));
	      std::istringstream iss2b(static_cast<char*>(message2.data()));
	      //test=iss.str();
	      
	      //std::cout<<"message received: "<<iss2b.str()<<std::endl;

	      m_data->InfoTitle="HV error";
	      m_data->InfoMessage+=iss2b.str();
	      //      m_data->GetTTree("RunInformation")->Fill();
	      m_data->Stores["RunInformation"]->Set("HVerror", m_data->InfoMessage);
	      std::cout<<"HVerror"<<m_data->InfoMessage<<std::endl;
	    
}
	    else std::cout<<"HV reply timeout"<<std::endl;
	  } 
	  else std::cout<<"HV akn reply timeout"<<std::endl;
	}
	else std::cout<<"HV send timeout"<<std::endl;
      }
      else std::cout<<"HV reply timeout"<<std::endl;
    }
    else std::cout<<"HV akn reply timeout"<<std::endl;
  }
  else std::cout<<"HV send timeout"<<std::endl;
  
  delete HV;
  HV=0;

  return true;
}
