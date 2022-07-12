#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define HLT 0b1000000000000000  // Halt clock
#define MI  0b0100000000000000  // Memory address register in
#define RI  0b0010000000000000  // RAM data in
#define RO  0b0001000000000000  // RAM data out
#define IO  0b0000100000000000  // Instruction register out
#define II  0b0000010000000000  // Instruction register in
#define AI  0b0000001000000000  // A register in
#define AO  0b0000000100000000  // A register out
#define EO  0b0000000010000000  // ALU out
#define SU  0b0000000001000000  // ALU subtract
#define BI  0b0000000000100000  // B register in
#define OI  0b0000000000010000  // Output register in
#define CE  0b0000000000001000  // Program counter enable
#define CO  0b0000000000000100  // Program counter out
#define J  	0b0000000000000010  // Jump (program counter in)
#define BO	0b0000000000000001  // B register out

int data[] = {
  MI|CO,  RO|II|CE,  0,      	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0000 - NOP
  MI|CO,  RO|II|CE,  IO|MI,  	RO|AI,  0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0001 - LDA
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0010 - ADD
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|SU,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0011 - SUB
  MI|CO,  RO|II|CE,  IO|MI,  	AO|RI,  0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0100 - STA
  MI|CO,  RO|II|CE,  IO|AI,  	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0101 - LDI
  MI|CO,  RO|II|CE,  IO|J,   	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0110 - JMP
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0111 - LDB
  MI|CO,  RO|II|CE,  0,      	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1000 
  MI|CO,  RO|II|CE,  0,      	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1001
  MI|CO,  RO|II|CE,  0,      	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1010
  MI|CO,  RO|II|CE,  EO|SU|OI,  0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1011 - SUBOUT
  MI|CO,  RO|II|CE,  EO|OI,  	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1100 - ADDOUT
  MI|CO,  RO|II|CE,  BO|OI,  	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1101 - OUTB
  MI|CO,  RO|II|CE,  AO|OI,  	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1110 - OUTA
  MI|CO,  RO|II|CE,  HLT,    	0,      0,         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1111 - HLT
};

string SixteenBitConverter (int x){
	int con = 32768;
	string num = "0000000000000000";
	while(con>=1 && x > 0){
			if(x>=con){
				if(con >= 32768){
					num[0] = '1';
					x = x - 32768;
				}else if(con >= 16384){
					num[1] = '1';
					x = x - 16384;
				}else if(con >= 8192){
					num[2] = '1';
					x = x - 8192;
				}else if(con >= 4096){
					num[3] = '1';
					x = x - 4096;
				}else if(con >= 2048){
					num[4] = '1';
					x = x - 2048;
				}else if(con >= 1024){
					num[5] = '1';
					x = x - 1024;
				}else if(con >= 512){
					num[6] = '1';
					x = x - 512;
				}else if(con == 256){
					num[7] = '1';
					x = x - 256;
				}else if(con >= 128){
					num[8] = '1';
					x = x - 128;
				}else if(con >= 64){
					num[9] = '1';
					x = x - 64;
				}else if(con >= 32){
					num[10] = '1';
					x = x - 32;
				}else if(con >= 16){
					num[11] = '1';
					x = x - 16;
				}else if(con >= 8){
					num[12] = '1';
					x = x - 8;
				}else if(con >= 4){
					num[13] = '1';
					x = x - 4;
				}else if(con >= 2){
					num[14] = '1';
					x = x - 2;
				}else if(con == 1){
					num[15] = '1';
					x = x - 1;
				}
			}
			con = con/2;
		}
		return num;
}

int main () {
	int x, h = 0;
	ifstream input ("integer.in");
	ofstream output;
	output.open("memory.list", ios::out | ios::trunc);
	int out [8][256];
	while(input>>x){
		int arr [8] = {0, 0, 0, 0, 0, 0, 0, 0};
		int num = x, con = 128;;
		while(con>=1 && x > 0){
			if(x>=con){
				if(con >= 128){
					arr[0] = 1;
					x = x - 128;
				}else if(con >= 64){
					arr[1] = 1;
					x = x - 64;
				}else if(con >= 32){
					arr[2] = 1;
					x = x - 32;
				}else if(con >= 16){
					arr[3] = 1;
					x = x - 16;
				}else if(con >= 8){
					arr[4] = 1;
					x = x - 8;
				}else if(con >= 4){
					arr[5] = 1;
					x = x - 4;
				}else if(con >= 2){
					arr[6] = 1;
					x = x - 2;
				}else if(con == 1){
					arr[7] = 1;
					x = x - 1;
				}
			}
			con = con/2;
		}
		for(int i = 0; i < 8; i++){
			out[i][h] = arr[i];
		}
		h++;
	}	
	//cout<<sizeof(data)/sizeof(data[0]);
	
	for(int i = 0; i <256; i++){
		for(int j = 0; j < 8; j++){
			cout<<out[j][i];
		}
		cout<<" ";
		cout<<SixteenBitConverter(data[i]);
		cout<<" "<<data[i]<<endl;
	}
	for(int i = 0; i <256; i++){
		output<<SixteenBitConverter(data[i])<<endl;
	}
	output.close();
	input.close();
	return 0;
}









