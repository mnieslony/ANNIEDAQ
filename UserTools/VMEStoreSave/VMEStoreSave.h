#ifndef VMEStoreSave_H
#define VMEStoreSave_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Tool.h"
#include <zmq.hpp>

//#include "TFile.h"
//#include "TTree.h"
//#include "TThread.h"
//#include "TRandom3.h"

#include <boost/progress.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class VMEStoreSave: public Tool{
 public:
  
  VMEStoreSave();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();
  
 private:
  
  BoostStore* PMTData;
  BoostStore* TrigData;
  //  MRDOut MRDout;
  // static void *RootWriter(void* arg);
  
  zmq::socket_t *sPort;
  zmq::socket_t *strigPort; 
  
  //TTree *tree;
  
  std::string OutPath, OutName, OutNameB;
  
  int TreeCap, FileCap, FileCount, Entries;
  //int ThreadCount;
  
  std::vector<std::string> Lcard;
  std::vector<int> Ncard;
  
  unsigned int OutN, Trigger;
  std::vector<unsigned int> Value, Slot, Channel;
  std::vector<std::string> Type;
  ULong64_t TimeStamp;
  
  std::map<std::string, MRDData::Card>::iterator in;	//Iterator over the Data maps for TDC and ADC
  std::vector<MRDData::Channel>::iterator is;	//Iterator over the Cards, a vec of Channels map 
  std::map<int, int>::iterator it;		//Iterator over the Channel of a Card
  
  boost::posix_time::ptime *Epoch;
  std::string StartTime;
  //  int MonitorFrequency;
  zmq::pollitem_t items[2];

};

#endif