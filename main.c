#include <reg51.h>

//7segment
#define SEGMENT_PORT P2 
#define DIGIT_PORT P0 
unsigned char inp=0;
unsigned char digsel=1;
void timer0(void) interrupt 1 {
unsigned char SEGMENT_LOOKUP[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18};    
//unsigned char i,j;
unsigned char temp,digits[4];
temp=inp;
digits[0]=temp%10;
temp=temp/10;
digits[1]=temp%10;
temp=temp/10;
digits[2]=temp%10;
temp=temp/10;
digits[3]=temp%10;
temp=temp/10;

//for (i = 0; i <4; i++)
//{
//	DIGIT_PORT = ~(0x01 << i);
//SEGMENT_PORT = SEGMENT_LOOKUP[digits[i]];
//for (j = 0; j < 600; j++);
//}
SEGMENT_PORT = SEGMENT_LOOKUP[digits[digsel]];
if (digsel==8){
	digsel=1;
}else{
	digsel=digsel << 1;
}
}


//keypad
sbit DB0 = P1^0;
sbit DB1 = P1^1;
sbit DB2 = P1^2;
sbit DB3 = P1^3;
sbit DB4 = P1^4;
sbit DB5 = P1^5;
sbit DB6 = P1^6;
sbit DB7 = P1^7;


unsigned char temp=0,i=0;
unsigned char keys[4][4] = { 
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

int conc(int inp,char c)
{
	inp=inp*10;
	inp=inp+(int)c;
	return inp;
}

//timer delay

sbit freq_output = P3^7;
unsigned int initial_value;
unsigned int machine_cycles;
unsigned int input_frequency;
unsigned int iterations;
unsigned char c;

unsigned int get_iterations(unsigned int input_frequency)
{
	iterations = (((1.0 / input_frequency) / 2.0) / (0.000001085)) / 65536;
	return iterations;
}
unsigned int get_initial_value(unsigned int input_frequency, unsigned int iterations)
{
	machine_cycles = (((1.0 / input_frequency) / 2.0) / (0.000001085));
	machine_cycles = machine_cycles - ((iterations) * 65536);
	initial_value = 65536 - machine_cycles;
	return initial_value;
}



void T1M1Delay(unsigned int iterations, unsigned int initial_value)
{
	// Setting the timer mode reg.
	TMOD = (TMOD & 0x0F) | 0x10;
	for (c = 0; c <  iterations; c++)
	{
		TH1 = 0;
		TL1 = 0;
		// Start the timer.
		TR1 = 1;
		while (TF1 == 0);
		// Stop the timer.
		TR1 = 0;
		TF1 = 0;
	}
	// Loading the initial value.
	TH1 = (initial_value >> 8) & 0xFF;
  TL1 = initial_value & 0xFF;
	// Start the timer.
	TR1 = 1;
	while (TF1 == 0);
	// Stop the timer.
	TR1 = 0;
	TF1 = 0;
}


unsigned char working=0;
void timer1(void) interrupt 0 {
	if (working==0){
	working=~working;
	input_frequency = inp;
	iterations = get_iterations(input_frequency);
	initial_value = get_initial_value(input_frequency, iterations);
	while(1)
	{
		if (working==0)
		{
			return;
		}
	  freq_output =~ freq_output;
		T1M1Delay(iterations, initial_value);
	}
	}
	else{
		working=~working;
	}
}





void main() {
	TMOD=0x02;
	TH0=0x00;
	TR0=1;
	IE=0x83;
	
	DB1=DB2=DB3=DB4=DB5=DB6=DB7=DB0=1;

while(1)
	{
		DB0 = 0;
    if (DB4 == 0) { while (DB4 == 0); inp=conc(inp,keys[0][0]); }
    if (DB5 == 0) { while (DB5 == 0); inp=conc(inp,keys[0][1]); }
    if (DB6 == 0) { while (DB6 == 0); inp=conc(inp,keys[0][2]); }
//    if (DB7 == 0) { while (DB7 == 0); inp=conc(inp,keys[0][3]); }
		DB0=1;DB1=0;
		if (DB4 == 0) { while (DB4 == 0); inp=conc(inp,keys[1][0]); }
    if (DB5 == 0) { while (DB5 == 0); inp=conc(inp,keys[1][1]); }
    if (DB6 == 0) { while (DB6 == 0); inp=conc(inp,keys[1][2]); }
    if (DB7 == 0) { while (DB7 == 0); inp=conc(inp,keys[1][3]); }
		DB1=1;DB2=0;
		if (DB4 == 0) { while (DB4 == 0); inp=conc(inp,keys[2][0]); }
    if (DB5 == 0) { while (DB5 == 0); inp=conc(inp,keys[2][1]); }
    if (DB6 == 0) { while (DB6 == 0); inp=conc(inp,keys[2][2]); }
    if (DB7 == 0) { while (DB7 == 0); 
	while(1){
			for(i=0;i<4;i++)
	{
		if (i==0){
			P0=0x01;
		}else if(i==1){
			P0=0x02;
		}else if(i==2){
			P0=0x04;
		}else if(i==3){
			P0=0x08;
		}
		temp=(inp%(10^i))/((10^(i-1)));
		if (temp==0)
			P2=0xC0;
		else if (temp==1)
			P2=0xF9;
		else if (temp==2)
			P2=0xA4;
		else if (temp==3)
			P2=0xB0;
		else if (temp==4)
			P2=0x99;
		else if (temp==5)
			P2=0x92;
		else if (temp==6)
			P2=0x82;
		else if (temp==7)
			P2=0xF8;
		else if (temp==8)
			P2=0x80;
		else if (temp==9)
			P2=0x90;
		}
	}
		}
		DB2=1;DB3=0;
//		if (DB4 == 0) { while (DB4 == 0); inp=conc(inp,keys[3][0]); }
//    if (DB5 == 0) { while (DB5 == 0); inp=conc(inp,keys[3][1]); }
//    if (DB6 == 0) { while (DB6 == 0); inp=conc(inp,keys[3][2]); }
//    if (DB7 == 0) { while (DB7 == 0); inp=conc(inp,keys[3][3]); }
		DB3=1;
	 }
	}