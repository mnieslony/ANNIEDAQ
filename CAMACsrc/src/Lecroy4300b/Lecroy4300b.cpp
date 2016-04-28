#include "Lecroy4300b.h"

Lecroy4300b::Lecroy4300b(int NSlot, int i = 0) : CamacCrate(i)
{
	Slot.push_back(NSlot);
	ID = Slot.size()-1;
}

int Lecroy4300b::ReadReg(long &Data)		//Read status word register. Q = 1 if BUSY = 0.
{
	int Q = 0, X = 0;
	int ret = READ(0, 0, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::ReadPed(int Ch, long &Data)	//Read pedestal memory (8 bits) for the 16 channels. Q = 1 if BUSY = 0.
{
	int Q = 0, X = 0;
	int ret = READ(Ch, 1, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::ReadOut(long &Data, int Ch = 0)	//Random access or sequential readout of the 16 ADC values. Q = 1 if BUSY = 1.
{
	int Q = 0, X = 0;
	int ret = READ(Ch, 2, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::TestLAM()	//Test LAM. Q = 1 if LAM is present.
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(0, 8, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::ClearAll()	//Clear Module (not pedestal?)
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(0, 9, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::ClearLAM()		//Test and clear LAM, if present.
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(0, 10, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::WriteReg(long &Data)	//Write status word register. Q = 1 if BUSY = 0.
{
	int Q = 0, X = 0;
	int ret = WRITE(0, 16, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::WritePed(int Ch, long &Data)	//Write pedestal memory (8 bits) for the 16 channels. Q = 1 if BUSY = 0.
{
	int Q = 0, X = 0;
	int ret = WRITE(Ch, 17, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::TestAll()	//Enable test. Q = 1 if BUSY = 0. 
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(0, 25, Data, Q, X);
	if (ret < 0) return ret;
	else return Q;
}

int Lecroy4300b::READ(int F, int A, long &Data, int &Q, int &X)	//Generic READ
{
	return CamacCrate::READ(GetID(), F, A, Data, Q, X);
}

int Lecroy4300b::WRITE(int F, int A, long &Data, int &Q, int &X)	//Gneric WRITE
{
	return CamacCrate::WRITE(GetID(), F, A, Data, Q, X);
}

int Lecroy4300b::GetData(std::vector<long> &vData, std::vector<long> &vSAD) 
{
	int ret;
	if (ECE || CCE) ret = DumpCompressed(vData, vSAD);
	else ret = DumpAll(vData);
	return ret;
}

int Lecroy4300b::DumpCompressed(std::vector<long> &vData, std::vector<long> &vSAD) 
{
	vData.clear();
	long Data = 0, Chan = 0;
	bool Head;
	int ret = ReadOut(Word);
	ParseCompData(Word, Data, Chan, Head);
	if (Head)
	{
		for (int i = 0; i < Chan; i++)
		{
			ret = ReadOut(Data);
			ParseCompData(Word, Data, Chan, Head);
			vData.push_back(Data);
			vSAD.push_back(Chan);
		}
		return vData.size();
	}
	else return -1;
}

int Lecroy4300b::DumpAll(std::vector<long> &vData)
{
	long Data = 0;
	int ret;
	for (int i = 0; i < 16; i++)
	{
		ret = ReadOut(Data, i);
		vData.push_back(Data);
	}
	return ret * vData.size();
}

void Lecroy4300b::DecRegister()
{
	std::bitset<16> breg(Control);
	VSN = BittoInt(breg, 0, 7);	//b0
	EPS = breg.test(8); 		//b1
	ECE = breg.test(9);		//b2
	EEN = breg.test(10);		//b3
	CPS = breg.test(11);		//b4
	CCE = breg.test(12);		//b5
	CSR = breg.test(13);		//b6
	CLE = breg.test(14);		//b7
	OFS = breg.test(15);		//b8
}

void Lecroy4300b::EncRegister()
{
	std::string sbit = "";
	std::bitset<1> b8(OFS);
	sbit += b8.to_string();
	std::bitset<1> b7(CLE);
	sbit += b7.to_string();
	std::bitset<1> b6(CSR);
	sbit += b6.to_string();
	std::bitset<1> b5(CCE);
	sbit += b5.to_string();
	std::bitset<1> b4(CPS);
	sbit += b4.to_string();
	std::bitset<1> b3(EEN);
	sbit += b3.to_string();
	std::bitset<1> b2(ECE);
	sbit += b2.to_string();
	std::bitset<1> b1(EPS);
	sbit += b1.to_string();
	std::bitset<8> b0(VSN);
	sbit += b0.to_string();

	std::bitset<16> bTOT(sbit);
	Control = bTOT.to_ulong();
}

void Lecroy4300b::GetRegister()
{
	int ret = ReadReg(Control);
}

void Lecroy4300b::SetRegister()
{
	int ret = WriteReg(Control);
}

void Lecroy4300b::PrintRegRaw()
{
	std::cout << "Lecroy 4300b\nReg:\t" << Control << std::endl;
}

void Lecroy4300b::PrintRegister()
{
	DecRegister();
	std::cout << "[" << GetID() << "] :\t";
	std::cout << "Lecroy4300b in slot " << GetSlot() << " register control\n";
	std::cout << "VSN " << VSN << std::endl;
	std::cout << "EPS " << EPS << std::endl;
	std::cout << "ECE " << ECE << std::endl;
	std::cout << "EEN " << EEN << std::endl;
	std::cout << "CPS " << CPS << std::endl;
	std::cout << "CCE " << CCE << std::endl;
	std::cout << "CSR " << CSR << std::endl;
	std::cout << "CLE " << CLE << std::endl;
	std::cout << "OFS " << OFS << std::endl;
	std::cout << std::endl; 
}

void Lecroy4300b::ParseCompData(long Word, long &Stat, long &Num, bool &B1)
{
	std::bitset<16> bWord(Word);
	B1 = bWord.test(15);
	Num = BittoInt(bWord, 11, 15);
	Stat = BittoInt(bWord, 0, 10);
}

int Lecroy4300b::GetPedestal()
{
	int ret;
	for (int i = 0; i < 16; i++)
		ret = ReadPed(i, Ped[i]);
	return ret;
}

int Lecroy4300b::SetPedestal()
{
	int ret;
	for (int i = 0; i < 16; i++)
		ret = WritePed(i, Ped[i]);
	return ret;
}

int Lecroy4300b::GetPedFile(std::string fname)
{
	std::ofstream fout(fname.c_str());
	int ret = GetPedestal();
	std::cout << "#Pedestal saved on ";
	std::time_t now = std::time(NULL);
	std::cout << std::asctime(std::localtime(&now)) << std::endl;	
	for (int i = 0; i < 16; i++)
		fout << Ped[i] << std::endl;
	return ret;
}

int Lecroy4300b::SetPedFile(std::string fname)
{
	std::ifstream fin(fname.c_str());
	std::string line;
	std::stringstream ss;
	int ped;
	std::vector<int> vPed;
	while (getline(fin, line))
	{
		if (line[0] == '#') continue;
		ss.str("");
		ss.clear();
		ss << line;
		ss >> ped;
		vPed.push_back(ped);
	}
	if (vPed.size() == 16) 
		for (int i = 0; i < 16; i++)
			Ped[i] = vPed.at(i);
	return SetPedestal();
}

int Lecroy4300b::GetID()	//Return ID of module
{
	return ID;
}

int Lecroy4300b::GetSlot()	//Return n of Slot of module
{
	return Slot.at(GetID());
}
