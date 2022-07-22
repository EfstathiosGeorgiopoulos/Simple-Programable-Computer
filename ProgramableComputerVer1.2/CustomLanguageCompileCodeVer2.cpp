#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

//define ports
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
#define FI	0b0000000000000001  // Flag register register out

//CPU decoded instructions
// FLAGS : 00
int data_0[] = {
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0000 - NOP
  MI|CO,  RO|II|CE,  IO|MI,  	RO|AI,  0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0001 - LDA
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|FI,  	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0010 - ADD
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|SU|FI,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0011 - SUB
  MI|CO,  RO|II|CE,  IO|MI,  	AO|RI,  0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0100 - STA
  MI|CO,  RO|II|CE,  IO|AI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0101 - LDI
  MI|CO,  RO|II|CE,  IO|J,   	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0110 - JMP
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0111 - JC
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1000 - JZ
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1001
  MI|CO,  RO|II|CE,  0,    	0, 	0,      	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1010
  MI|CO,  RO|II|CE,  EO|SU|OI,  0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1011 - SUBOUT
  MI|CO,  RO|II|CE,  EO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1100 - ADDOUT
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1101
  MI|CO,  RO|II|CE,  AO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1110 - OUTA
  MI|CO,  RO|II|CE,  HLT,    	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1111 - HLT
};
//FLAGS : 01
int data_1[] = {
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0000 - NOP
  MI|CO,  RO|II|CE,  IO|MI,  	RO|AI,  0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0001 - LDA
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|FI, 	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0010 - ADD
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|SU|FI,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0011 - SUB
  MI|CO,  RO|II|CE,  IO|MI,  	AO|RI,  0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0100 - STA
  MI|CO,  RO|II|CE,  IO|AI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0101 - LDI
  MI|CO,  RO|II|CE,  IO|J,   	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0110 - JMP
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0111 - JC
  MI|CO,  RO|II|CE,  IO|J,     	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1000 - JZ
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1001
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1010
  MI|CO,  RO|II|CE,  EO|SU|OI,  0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1011 - SUBOUT
  MI|CO,  RO|II|CE,  EO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1100 - ADDOUT
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1101
  MI|CO,  RO|II|CE,  AO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1110 - OUTA
  MI|CO,  RO|II|CE,  HLT,    	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1111 - HLT
};
//FLAGS : 10
int data_2[] = {
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0000 - NOP
  MI|CO,  RO|II|CE,  IO|MI,  	RO|AI,  0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0001 - LDA
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|FI,       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0010 - ADD
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|SU|FI,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0011 - SUB
  MI|CO,  RO|II|CE,  IO|MI,  	AO|RI,  0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0100 - STA
  MI|CO,  RO|II|CE,  IO|AI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0101 - LDI
  MI|CO,  RO|II|CE,  IO|J,   	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0110 - JMP
  MI|CO,  RO|II|CE,  IO|J,    	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0111 - JC
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1000 - JZ
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1001
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1010
  MI|CO,  RO|II|CE,  EO|SU|OI,  0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1011 - SUBOUT
  MI|CO,  RO|II|CE,  EO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1100 - ADDOUT
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1101
  MI|CO,  RO|II|CE,  AO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1110 - OUTA
  MI|CO,  RO|II|CE,  HLT,    	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1111 - HLT
};
//FLAGS : 11
int data_3[] = {
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0000 - NOP
  MI|CO,  RO|II|CE,  IO|MI,  	RO|AI,  0,        	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0001 - LDA
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|FI,   	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0010 - ADD
  MI|CO,  RO|II|CE,  IO|MI,  	RO|BI,  EO|AI|SU|FI,	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0011 - SUB
  MI|CO,  RO|II|CE,  IO|MI,  	AO|RI,  0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0100 - STA
  MI|CO,  RO|II|CE,  IO|AI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0101 - LDI
  MI|CO,  RO|II|CE,  IO|J,   	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0110 - JMP
  MI|CO,  RO|II|CE,  IO|J,     	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0111 - JC
  MI|CO,  RO|II|CE,  IO|J,     	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1000 - JZ
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1001
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1010
  MI|CO,  RO|II|CE,  EO|SU|OI,  0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1011 - SUBOUT
  MI|CO,  RO|II|CE,  EO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1100 - ADDOUT
  MI|CO,  RO|II|CE,  0,      	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1101
  MI|CO,  RO|II|CE,  AO|OI,  	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1110 - OUTA
  MI|CO,  RO|II|CE,  HLT,    	0,      0,         	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1111 - HLT
};

//Convert integer to 16 bit binary number
string InToBit (int x, int bit){
	int con = pow(2, bit - 1), y;
	string num("");
	for(int i = 0; i < bit; i++){
		num.append("0");
	}
	for(int i = 0; i < bit; i++){
		y = pow(2, i);
		if(x >= con/y){
			num[i] = '1';
			x = x - con/y;
		}
	}
	return num;
}

int main () {
	int x, h = 0;
	
	//Set up input and output files
	ifstream input ("integer.in");
	ofstream output;
	output.open("memory.list", ios::out | ios::trunc);
	
	//Set up addresses
	char out [10][1024];
	while(input>>x){
		string arr = InToBit(x, 10);
		for(int i = 0; i < 10; i++){
			out[i][h] = arr[i];
		}
		h++;
	}	
	//Output results on consol
	//Output section with flags 00
	for(int i = 0; i <256; i++){
		for(int j = 0; j < 10; j++){
			cout<<out[j][i];
		}
		cout<<" "<<InToBit(data_0[i], 16)<<" "<<data_0[i]<<endl;;
		output<<InToBit(data_0[i], 16)<<endl;
	}
	//Output section with flags 01
	for(int i = 0; i <256; i++){
		for(int j = 0; j < 10; j++){
			cout<<out[j][i + 256];
		}
		cout<<" "<<InToBit(data_1[i], 16)<<" "<<data_1[i]<<endl;
		output<<InToBit(data_1[i], 16)<<endl;
	}
	//Output section with flags 10
	for(int i = 0; i <256; i++){
		for(int j = 0; j < 10; j++){
			cout<<out[j][i + 512];
		}
		cout<<" "<<InToBit(data_2[i], 16)<<" "<<data_1[i]<<endl;
		output<<InToBit(data_2[i], 16)<<endl;
	}
	//Output section with flags 11
	for(int i = 0; i <256; i++){
		for(int j = 0; j < 10; j++){
			cout<<out[j][i + 768];
		}
		cout<<" "<<InToBit(data_3[i], 16)<<" "<<data_1[i]<<endl;
		output<<InToBit(data_3[i], 16)<<endl;
	}
	//close all opened files
	output.close();
	input.close();
	
	return 0;
}
