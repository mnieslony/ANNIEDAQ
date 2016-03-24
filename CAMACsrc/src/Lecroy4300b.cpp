#include "Lecroy4300b"

Lecroy4300b::Lecroy4300b()
{
	Slot.push_back(NSlot);
	ID = Slot.size()-1;
}

int Lecroy4300b::ReadReg(long &Data)		//Read status word register. Q = 1 if BUSY = 0.
{
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(GetID(), 0, 0, Data, Q, X);
	if (ret > 0) return Q;
	else return ret;
}

int Lecroy4300b::ReadPed(int Ch, long &Data)	//Read pedestal memory (8 bits) for the 16 channels. Q = 1 if BUSY = 0.
{
	int Q = 0, X = 0;
	int ret = READ(GetID(), Ch, 1, Data, Q, X);
	if(ret > 0) return Q;
	else return ret;
}

void Lecroy4300b::ReadOut(int Ch = 0, long &Data)	//Random access or sequential readout of the 16 ADC values. Q = 1 if BUSY = 1.
{
	int Q = 0, X = 0;
	int ret = READ(GetID(), Ch, 2, Data, Q, X);
	if(ret > 0) return Q;
	else return ret;
}

int Lecroy4300b::TestLAM()	//Test LAM. Q = 1 if LAM is present.
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(GetID(), 0, 8, Data, Q, X);
	return Q;
}

int Lecroy4300b::ClearAll()	//Clear Module (not pedestal?)
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(GetID(), 0, 9, Data, Q, X);
	return ret;
}

int Lecroy4300b::TestClearLAM()		//Test and clear LAM, if present.
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(GetID(), 0, 10, Data, Q, X);
	return Q;
}

int Lecroy4300b::WriteReg(long &Data)	//Write status word register. Q = 1 if BUSY = 0.
{
	int Q = 0, X = 0;
	int ret = WRITE(GetID(), 0, 16, Data, Q, X);
	return Q;
}

void Lecroy4300b::WritePed(int Ch, long &Data)	//Write pedestal memory (8 bits) for the 16 channels. Q = 1 if BUSY = 0.
{
	int Q = 0, X = 0;
	int ret = WRITE(GetID(), Ch, 17, Data, Q, X);
	if(ret > 0) return Q;
	else return ret;
}

int Lecroy4300b::TestAll()	//Enable test. Q = 1 if BUSY = 0. 
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = READ(GetID(), 0, 25, Data, Q, X);
	return Q;
}

int Lecroy4300b::READ(int F, int A, long &Data)	//Generic READ
{
	int Q = 0, X = 0, ret;
	if (R < 4) ret = READ(GetID(), F, A, Data, Q, X);
	if(ret > 0) return Data;
	else return ret;
}

int Lecroy4300b::WRITE(int F, int A, long &Data)	//Gneric WRITE
{
	int Q = 0, X = 0, ret;
	if (R < 4) ret = WRITE(GetID(), F, A, Data, Q, X);
	else ret = 0;
	return ret;
}

int Lecroy4300b::GetData(std::vector<long> &vData) 
{
	int ret;
	if (ECE || CCE) ret = DumpCompressed(vData);
	else ret = DumpAll(vData);
	return ret;
}

int Lecroy4300b::DumpCompressed(std::vector<long> &vData, long &VSN) 
{
	vData.clear();
	long Data = 0, Chan = 0;
	int ret = ReadOut(Word);
	bool Head;
	ParseCompData(VSN, Data, Chan, Head);
	if (Head)
	{
		for (int i = 0; i < Chan; i++)
		{
			ret = ReadOut(Data);
			vData.push_back(Data);
		}
		return ret * vData.size();
	}
	else return -1;
}

int Lecroy4300b::DumpAll(std::vector<long> &vData)
{
	long Data = 0;
	int ret;
	for (int i = 0; i < 16; i++)
	{
		ret = ReadOut(i, Data);
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
	std::bittest<1> b8(OFS);
	sbit += b8.to_string();
	std::bittest<1> b7(CLE);
	sbit += b7.to_string();
	std::bittest<1> b6(CSR);
	sbit += b6.to_string();
	std::bittest<1> b5(CCE);
	sbit += b5.to_string();
	std::bittest<1> b4(CPS);
	sbit += b4.to_string();
	std::bittest<1> b3(EEN);
	sbit += b3.to_string();
	std::bittest<1> b2(ECE);
	sbit += b2.to_string();
	std::bittest<1> b1(EPS);
	sbit += b1.to_string();
	std::bittest<8> b0(VSN);
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

void Lecroy4300b::PrintRegister()
{
	DecRegister();
	std::cout << "[" << GetID << "] :\t"
	std::cout << "Lecroy3377 in slot " << GetSlot() << " register control\n";
	std::cout << "- VSN" << VSN << std::endl;
	std::cout << "- EPS" << EPS << std::endl;
	std::cout << "- ECE" << ECE << std::endl;
	std::cout << "- EEN" << EEN << std::endl;
	std::cout << "- CPS" << CPS << std::endl;
	std::cout << "- CCE" << CCE << std::endl;
	std::cout << "- CSR" << CSR << std::endl;
	std::cout << "- CLE" << CLE << std::endl;
	std::cout << "- OFS" << OFS << std::endl;
	std::cout << std::endl; 
}

void ParseCompData(long Word, long &Stat, long &Num, bool &B1)
{
	std::bitset<16> bWord(Word);
	B1 = bWord.test(15);
	Num = BittoDec(bWord, 11, 15);
	Stat = BittoDec(bWord, 0, 10);
}

int Lecroy4300b::GetID()	//Return ID of module
{
	return ID;
}

int Lecroy4300b::GetSlot()	//Return n of Slot of module
{
	return Slot.at(GetID());
}
