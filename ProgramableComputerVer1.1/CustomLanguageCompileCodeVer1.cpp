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
#define BO	0b0000000000000001  // B register out

//CPU decoded instructions
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

//Convert integer to 16 bit binary number
string SixteenBitConverter (int x){
	int con = 32768, y;
	string num = "0000000000000000";
	for(int i = 0; i <= 15; i++){
		y = pow(2, i);
		if(x >= con/y){
			num[i] = '1';
			x -= con/y;
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
	char out [8][256];
	while(input>>x && h <= 255){
		string arr = "00000000";
		int con = 128, y;
		for(int i = 0; i < 8; i++){
			y = pow(2, i);
			if(x >= con/y){
				arr[i] = '1';
				x = x - con/y;
			}
		}
		for(int i = 0; i < 8; i++){
			out[i][h] = arr[i];
		}
		h++;
	}	

	//Output results on consol
	for(int i = 0; i <256; i++){
		for(int j = 0; j < 8; j++){
			cout<<out[j][i];
		}
		cout<<" "<<SixteenBitConverter(data[i])<<" "<<data[i]<<endl;;
	}
	//Output result on list file
	for(int i = 0; i <256; i++){
		output<<SixteenBitConverter(data[i])<<endl;
	}
	//close all opened files
	output.close();
	input.close();
	
	return 0;
}
