#include "../Unity.cpp"

Tool* Factory(std::string tool){
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;

if (tool=="InputVariables") ret=new InputVariables;
if (tool=="Postgress") ret=new Postgress;
//if (tool=="BoardReader") ret=new BoardReader;
//if (tool=="Monitoring") ret=new Monitoring;
//if (tool=="DataRecorder") ret=new DataRecorder;
if (tool=="Trigger") ret=new Trigger;
//if (tool=="NetworkSendData") ret=new NetworkSendData;
if (tool=="NetworkReceiveData") ret=new NetworkReceiveData;

//if (tool=="VMETriggerSend")ret=new VMETriggerSend;
// if (tool=="VMESendData")ret=new VMESendData;
//if (tool=="CardDataRecorder") ret=new CardDataRecorder;
//if (tool=="Logger") ret=new Logger;

//if (tool=="MRD") ret=new MRD;
//if (tool=="RootDataRecorder") ret=new RootDataRecorder;

//if (tool=="SlackBot") ret=new SlackBot;
if (tool=="HVComs") ret=new HVComs;
if (tool=="SlackBot") ret=new SlackBot;
if (tool=="CCTrigger") ret=new CCTrigger;
if (tool=="Lecroy") ret=new Lecroy;
//if (tool=="MRDTreeRecorder") ret=new MRDTreeRecorder;
//if (tool=="MRDRootRecorder") ret=new MRDRootRecorder;

// if (tool=="Monitoring2") ret=new Monitoring2;

//if (tool=="MRD") ret=new MRD;
if (tool=="MRDStoreSave") ret=new MRDStoreSave;
if (tool=="VMEStoreSave") ret=new VMEStoreSave;
if (tool=="MRDMonitoring") ret=new MRDMonitoring;
if (tool=="ThreadedSubToolChain") ret=new ThreadedSubToolChain;
if (tool=="V2Trigger") ret=new V2Trigger;
if (tool=="StoreSave") ret=new StoreSave;
if (tool=="LAPPDTrigger") ret=new LAPPDTrigger;
if (tool=="VMETrigger") ret=new VMETrigger;
if (tool=="TriggerStream") ret=new TriggerStream;
  if (tool=="NetworkReceiveDataStream") ret=new NetworkReceiveDataStream;
return ret;
}


