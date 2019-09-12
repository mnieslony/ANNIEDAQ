#ifndef VMETrigger_H
#define VMETrigger_H

#include <string>
#include <iostream>

#include "Tool.h"

class VMETrigger: public Tool {


 public:

  VMETrigger();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:

  zmq::socket_t* TriggerSend;
  zmq::pollitem_t items[1];
  unsigned long reftriggernum;



};


#endif