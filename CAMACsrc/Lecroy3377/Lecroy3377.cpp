#include "Lecroy3377"

Lecroy3377::Lecroy3377(int NSlot)	//Subclass constructor, n of Slot given
{
	CamacCrate::Slot.push_back(NSlot);
	ID = CamacCrate::Slot.size()-1;
}

int Lecroy3377::ReadFIFOall(std::vector<long> &vData) //Read FIFO data until end of event, Q = 1 for valid data, Q = 0 at end.
{
	long Data = 0;
	int Q = 0, X = 0, ret;
	do
	{
		ret = CamacCrate::READ(GetID(), 0, 0, Data, Q, X);
		if (ret >= 0) vData.push_back(Data);
	}
	while (Q == 1)
	return ret;
}

//COMMON START ONLY
int Lecroy3377::ReadFIFO();	//F(0)·A(1): Read FIFO data always (Common Start only).
{
}

//COMMON START ONLY
int Lecroy3377::ExFIFOOut(); // F(0)·A(2): Examine FIFO output, do not advance FIFO (Common Start only).
{
}

long Lecroy3377::ReadReg(int R) //Read Control Register R.
{
	long Data = 0;
	int Q = 0, X = 0, ret;
	if (R < 4) ret = CamacCrate::READ(GetID(), R, 1, Data, Q, X);
	if(ret > 0) return Data;
	else return 0;
}

//COMMON START ONLY
int Lecroy3377::ReadTestReg() //F(1)·A(6): Read CAMAC Test Register Common Start only
{
}

int Lecroy3377::TestLAM(); //Test LAM.
{
	long Data = 0;
	int Q = 0, X = 0;
	int ret = CamacCrate::READ(GetID(), 0, 8, Data, Q, X);
	return ret;
}

int Lecroy3377::ClearAll() //Clear all data and LAM. This does NOT affect the control registers.
{	
	long Data = 0;
	int Q = 0, X = 0;
	int ret = CamacCrate::READ(GetID(), 0, 9, Data, Q, X);
	return ret;
}

int Lecroy3377::ClearLAM() //Clear LAM.
{
	long Data = 0;
	int Q = 0, X = 0;
	int ret = CamacCrate::READ(GetID(), 0, 10, Data, Q, X);
	return ret;
}

//COMMON START ONLY
int Lecroy3377::WriteFIFOData() //F(16)·A(0): Write 16-bit data to FIFO (Common Start only).
{
}

//COMMON START ONLY
int Lecroy3377::WriteFIFOtag() // F(16)·A(1): Write FIFO tag bit (Common Start only).
{
}

int Lecroy3377::WriteReg(int R, long &Data); //Write Control Register R.
{
	int Q = 0, X = 0;
	int ret;
	if (R < 4) ret = CamacCrate::WRITE(GetID(), R, 17, Data, Q, X);
	else ret = 0;
	return ret;
}

int Lecroy3377::DisLAM() //Disable LAM.
{
	long Data = 0;
	int Q = 0, X = 0;
	int ret = CamacCrate::READ(GetID(), 0, 24, Data, Q, X);
	return ret;

}

int Lecroy3377::DisAcq() //Disable Acquisition Mode.
{
	long Data = 0;
	int Q = 0, X = 0;
	int ret = CamacCrate::READ(GetID(), 1, 24, Data, Q, X);
	return ret;

}

//COMMON START ONLY
int Lecroy3377::InitTest() //F(25)·A(0): Initiate test cycle (Common Start only).
{
	long Data = 0;
	int Q = 0, X = 0;
	int ret = CamacCrate::READ(GetID(), 0, 25, Data, Q, X);
	return ret;

}

int Lecroy3377::EnLAM() //Enable LAM.
{
	long Data = 0;
	int Q = 0, X = 0;
	int ret = CamacCrate::READ(GetID(), 0, 26, Data, Q, X);
	return ret;

}

int Lecroy3377::EnAcq() //Enable Acquisition Mode.
{
	long Data = 0;
	int Q = 0, Z = 0;
	int ret = CamacCrate::READ(GetID(), 1, 26, Data, Q, X);
	return ret;
}

int Lecroy3377::TestBuff()	//Test buffering in progress (BIP), Q = 1 while BIP.
{
	long Data = 0;
	int Q = 0, Z = 0;
	int ret = CamacCrate::READ(GetID(), 0, 27, Data, Q, X);
	return Q;
}

int Lecroy3377::TestBusy()	//Test busy, Q = 1 while busy.
{
	long Data = 0;
	int Q = 0, Z = 0;
	int ret = CamacCrate::READ(GetID(), 1, 27, Data, Q, X);
	return Q;
}

int Lecroy3377::TestEvent()	//Test event ready, Q = 1 if event ready for readout.
{
	long Data = 0;
	int Q = 0, Z = 0;
	int ret = CamacCrate::READ(GetID(), 2, 27, Data, Q, X);
	return Q;
}

int Lecroy3377::TestFIFO()	//Test FIFO tag bit, Q = 1 if tag bit set for word to be read next.
{
	long Data = 0;
	int Q = 0, Z = 0;
	int ret = CamacCrate::READ(GetID(), 3, 27, Data, Q, X);
	return Q;
}

/*
int Lecroy3377::Reprog(long *Data, int *Q, int *X)	//F(30): Begin the reprogramming sequence.
{
}
*/

int Lecroy3377::READ(int F, int A, long &Data)	//Generic READ
{
	int Q = 0, X = 0, ret;
	if (R < 4) ret = CamacCrate::READ(GetID(), F, A, Data, Q, X);
	if(ret > 0) return Data;
	else return ret;
}

int Lecroy3377::WRITE(int F, int A, long &Data)	//Gneric WRITE
{
	int Q = 0, X = 0, ret;
	if (R < 4) ret = CamacCrate::WRITE(GetID(), F, A, Data, Q, X);
	else ret = 0;
	return ret;
}

int Lecroy3377::GetID()		//Return ID of module
{
	return ID;
}

int Lecroy3377::GetSlot()	//Return n of Slot of module
{
	return CamacCrate::Slot.at(GetID());
}

void Lecroy3377::GetRegister()
{
	for (int i = 0; i < 4; i++)
		Control[i] = ReadReg(i);
}

void Lecroy3377::SetRegister()
{
	int ret;
	for (int i = 0; i < 4; i++)
		ret = WriteReg(i, Control+i);
}
void Lecroy3377::DecRegister()
{
	std::bitset<16> breg0(Control[0]);

	ModIDcode = BittoInt(breg0, 0, 7);	//b0_0
        RecEdges = breg.test(10);		//b0_1
        RedoutMode = breg.test(11);		//b0_2
        BuffMode = breg.test(12);		//b0_3
        HeaderMode = breg.test(13);		//b0_4
        Mode = BittoInt(breg0, 14, 15);		//b0_5

	std::bitset<16> breg1(Control[1]);
	
	TrgWidth = BittoInt(breg0, 0, 3);	//b1_0
	TrgDelay = BittoInt(breg0, 4, 7);	//b1_1
	TrgClock = BittoInt(breg0, 8, 9);	//b1_2
	MPI = BittoInt(breg0, 10, 11);		//b1_3
	FFERAmode = breg.test(12); 		//b1_4
	EvSerNum = BittoInt(breg0, 13, 15);	//b1_5

	std::bitset<16> breg2(Control[2]);

	MaxHITS = BittoInt(breg0, 0, 3);	//b2_0
	FullScale = BittoInt(breg0, 4, 15);	//b2_1
	
	std::bitset<16> breg3(Control[3]);

	ReqDelay = BittoInt(breg0, 0, 3);	//b3_0
}

void Lecroy3377::EncRegister()
{
	string sbit = "";
	std::bitset<2> b0_5(Mode);
	sbit += b0_5.to_string();
	std::bitset<1> b0_4(HeaderMode);
	sbit += b0_4.to_string();
	std::bitset<1> b0_3(BuffMode);
	sbit += b0_3.to_string();
	std::bitset<1> b0_2(RedoutMode);
	sbit += b0_2.to_string();
	std::bitset<1> b0_1(RecEdges);
	sbit += b0_1.to_string();
//	std::bitset<2> b0_emp;
//	sbit += b0_emp.to_string();
	std::bitset<10> b0_0(ModIDcode);
	sbit += b0_0.to_string();
	std::bitset<16> b0_TOT(sbit);
	Control[0] = b0_TOT.to_ulong();

	sbit = "";
	std::bitset<3> b1_5(EvSerNum);	
	sbit += b1_5.to_string();
	std::bitset<1> b1_4(FFERAmode);	
	sbit += b1_4.to_string();
	std::bitset<2> b1_3(MPI);	
	sbit += b1_3.to_string();
	std::bitset<2> b1_2(TrgClock);	
	sbit += b1_2.to_string();
	std::bitset<4> b1_1(TrgDelay);	
	sbit += b1_1.to_string();
	std::bitset<4> b1_0(TrgWidth);	
	sbit += b1_0.to_string();
	std::bitset<16> b1_TOT(sbit);
	Control[1] = b1_TOT.to_ulong();
	
	sbit = "";
	std::bitset<12> b2_1(FullScale);
	sbit += b2_1.to_string();
	std::bitset<4> b2_0(MaxHITS);
	sbit += b2_0.to_string();
	std::bitset<16> b2_TOT(sbit);
	Control[2] = b2_TOT.to_ulong();

	sbit = "";
	std::bitset<16> b3_0(ReqDelay);
	Control[3] = b3_0.to_ulong();
}

void Lecroy3377::PrintRegisterControl();
{	
	DecRegister();
	std::cout << "[" << GetID << "] :\t"
	std::cout << "Lecroy3377 in slot " << GetSlot() << " register control\n";
	std::cout << "- Module ID code\t" << ModIDcode << endl; 
	std::cout << "- Record Edges\t" << RecEdges << endl; 
	std::cout << "- Redout Mode\t" << RedoutMode << endl; 
	std::cout << "- Buffer Mode\t" << BuffMode << endl; 
	std::cout << "- Header Mode\t" << HeaderMode << endl; 
	std::cout << "- Mode in use\t" << Mode << endl; 
	std::cout << "- Trg Width\t"  << TrgWidth << endl; 
	std::cout << "- Trg Delay\t" << TrgDelay << endl; 
	std::cout << "- Trg Clock\t" << TrgClock << endl; 
	std::cout << "- MPI\t\t" << MPI << endl; 
	std::cout << "- Fast FERA\t" << FFERAmode << endl; 
	std::cout << "- Event Serial\t" << EvSerNum << endl; 
	std::cout << "- Max HITS\t" << MaxHITS << endl; 
	std::cout << "- Full Scale\t" << FullScale << endl; 
	std::cout << "- Request Delay\t" << ReqDelay << endl; 
	std::cout << endl; 
}

int Lecroy3377::BittoInt(bitset<16> &bitref, int a, int b)
{
	int tmp, sum = 0;
	if (a > b)
	{
		tmp = a;
		a = b;
		b = tmp;
	}
	for (int i = b; i i>= a: i--)
	{
		sum *= 2;
		sum += bitref(i);
	}
	return sum;
}