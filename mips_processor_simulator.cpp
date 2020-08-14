#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#define DEBUG

using namespace std;

void getCode();


//instruction and data memory as a 2-D array
string memory[128][4];

//number of instructions in program
int instCount = 0;

int main() {

	//registers

	int PC = 0;    //program counter
	int MAR;       //memory access register
	int ACC;       //accumulator
	int ALU;       //airthemetic logic unit
	string MDR[4]; //memory data register, this can hold a single instruction
	string CIR[4]; //current instruction register, this can hold a single instruction

	//temporary and saved registers, first half temporary regisiters, second half saved regisiters
	int reg[10] = { 1, 1, 1, 0, 100, 0, 0, 0, 0, 0 };

	//opcodes 
	enum opcodes { lw = 1, sw, ADD, SUB, ADDI, MULT, MOD, beq, j };

	//temporary varaibales used to pass decoded information for correct execution 
	int regAddr1, regAddr2, regAddr3, constant, opcode;

	//read assembly code from text file and place into memory
	getCode();

	//delete content in Log.txt file before writing to it
	ofstream file;
	file.open("Log.txt", ofstream::out | ofstream::trunc);
	file.close();

	for (PC; PC < instCount;) {

		//FETCH

		//RAM address of PC is stored in memory address register (MAR)
		MAR = PC;

		//contents of the data stored in MAR is moved to memory data register (MDR)
		MDR[0] = memory[MAR][0];
		MDR[1] = memory[MAR][1];
		MDR[2] = memory[MAR][2];
		MDR[3] = memory[MAR][3];

		//contents of MDR is copied to current instruction register (CIR)
		CIR[0] = MDR[0];
		CIR[1] = MDR[1];
		CIR[2] = MDR[2];
		CIR[3] = MDR[3];

		//increment program counter once instructions fetched ready for next instruction
		PC++;


		//DECODE

		if (CIR[0] == "lw") {
			opcode = lw;
			constant = stoi(CIR[2].substr(0));
			if (CIR[1].find_first_of("t") == 0)
				regAddr1 = stoi(CIR[1].substr(1));
			else if (CIR[1].find_first_of("s") == 0)
				regAddr1 = stoi(CIR[1].substr(1)) + 5;
		}

		else if (CIR[0] == "sw") {
			opcode = sw;
			regAddr2 = stoi(CIR[2].substr(0));
			if (CIR[1].find_first_of("t") == 0)
				regAddr1 = stoi(CIR[1].substr(1));
			else if (CIR[1].find_first_of("s") == 0)
				regAddr1 = stoi(CIR[1].substr(1)) + 5;
		}


		else if (CIR[0] == "ADD") {
			opcode = ADD;
			if (CIR[1].find_first_of("t") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}

				else if (CIR[2].find_first_of("s") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}
			}

			else if (CIR[1].find_first_of("s") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
					else if (CIR[2].find_first_of("s") == 0) {
						if (CIR[3].find_first_of("t") == 0) {
							regAddr1 = stoi(CIR[1].substr(1)) + 5;
							regAddr2 = stoi(CIR[2].substr(1)) + 5;
							regAddr3 = stoi(CIR[3].substr(1));
						}
						else if (CIR[3].find_first_of("t") == 0) {
							regAddr1 = stoi(CIR[1].substr(1)) + 5;
							regAddr2 = stoi(CIR[2].substr(1)) + 5;
							regAddr3 = stoi(CIR[3].substr(1)) + 5;
						}
					}
				}
			}
		}

		else if (CIR[0] == "SUB") {
			opcode = SUB;
			if (CIR[1].find_first_of("t") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}

				else if (CIR[2].find_first_of("s") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}
			}

			else if (CIR[1].find_first_of("s") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
					else if (CIR[2].find_first_of("s") == 0) {
						if (CIR[3].find_first_of("t") == 0) {
							regAddr1 = stoi(CIR[1].substr(1)) + 5;
							regAddr2 = stoi(CIR[2].substr(1)) + 5;
							regAddr3 = stoi(CIR[3].substr(1));
						}
						else if (CIR[3].find_first_of("t") == 0) {
							regAddr1 = stoi(CIR[1].substr(1)) + 5;
							regAddr2 = stoi(CIR[2].substr(1)) + 5;
							regAddr3 = stoi(CIR[3].substr(1)) + 5;
						}
					}
				}
			}
		}

		if (CIR[0] == "ADDI") {
			opcode = ADDI;
			if (CIR[1].find_first_of("t") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					regAddr1 = stoi(CIR[1].substr(1));
					regAddr2 = stoi(CIR[2].substr(1));
					constant = stoi(CIR[3].substr(0));
				}
				else if (CIR[2].find_first_of("s") == 0) {
					regAddr1 = stoi(CIR[1].substr(1));
					regAddr2 = stoi(CIR[2].substr(1)) + 5;
					constant = stoi(CIR[3].substr(0));
				}
			}
			else if (CIR[1].find_first_of("s") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					regAddr1 = stoi(CIR[1].substr(1)) + 5;
					regAddr2 = stoi(CIR[2].substr(1));
					constant = stoi(CIR[3].substr(0));
				}
				else if (CIR[2].find_first_of("s") == 0) {
					regAddr1 = stoi(CIR[1].substr(1)) + 5;
					regAddr2 = stoi(CIR[2].substr(1)) + 5;
					constant = stoi(CIR[3].substr(0));
				}
			}
		}


		else if (CIR[0] == "MULT") {
			opcode = MULT;
			if (CIR[1].find_first_of("t") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}

				else if (CIR[2].find_first_of("s") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}
			}

			else if (CIR[1].find_first_of("s") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
					else if (CIR[2].find_first_of("s") == 0) {
						if (CIR[3].find_first_of("t") == 0) {
							regAddr1 = stoi(CIR[1].substr(1)) + 5;
							regAddr2 = stoi(CIR[2].substr(1)) + 5;
							regAddr3 = stoi(CIR[3].substr(1));
						}
						else if (CIR[3].find_first_of("t") == 0) {
							regAddr1 = stoi(CIR[1].substr(1)) + 5;
							regAddr2 = stoi(CIR[2].substr(1)) + 5;
							regAddr3 = stoi(CIR[3].substr(1)) + 5;
						}
					}
				}
			}
		}


		else if (CIR[0] == "MOD") {
			opcode = MOD;
			if (CIR[1].find_first_of("t") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}

				else if (CIR[2].find_first_of("s") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1));
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}
			}

			else if (CIR[1].find_first_of("s") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("s") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1));
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				else if (CIR[2].find_first_of("s") == 0) {
					if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1));
					}
					else if (CIR[3].find_first_of("t") == 0) {
						regAddr1 = stoi(CIR[1].substr(1)) + 5;
						regAddr2 = stoi(CIR[2].substr(1)) + 5;
						regAddr3 = stoi(CIR[3].substr(1)) + 5;
					}
				}
				}
			}
		}

		else if (CIR[0] == "beq") {
			opcode = beq;
			if (CIR[1].find_first_of("t") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					regAddr1 = stoi(CIR[1].substr(1));
					regAddr2 = stoi(CIR[2].substr(1));
					constant = stoi(CIR[3].substr(0));

				}
				else if (CIR[2].find_first_of("s") == 0) {
					regAddr1 = stoi(CIR[1].substr(1));
					regAddr2 = stoi(CIR[2].substr(1)) + 5;
					constant = stoi(CIR[3].substr(0));
				}
			}

			else if (CIR[1].find_first_of("s") == 0) {
				if (CIR[2].find_first_of("t") == 0) {
					regAddr1 = stoi(CIR[1].substr(1)) + 5;
					regAddr2 = stoi(CIR[2].substr(1));
					constant = stoi(CIR[3].substr(0));
				}
				else if (CIR[2].find_first_of("s") == 0) {
					regAddr1 = stoi(CIR[1].substr(1)) + 5;
					regAddr2 = stoi(CIR[2].substr(1)) + 5;
					constant = stoi(CIR[3].substr(0));
				}
			}
		}

		else if (CIR[0] == "j") {
			opcode = j;
			constant = stoi(CIR[3].substr(0));
		}


		//EXECUTE and WRITEBACK


		//each case is an enumerated opcode and the integer variable opcode that the switch statement
		//evaulates is passed from the decode
		switch (opcode) {
		case lw:										//Load Word instruction execution
			ACC = stoi(memory[constant][0].substr(0));  //accumulator stores the contents of the memory location passed
			reg[regAddr1] = ACC;						//from the decoded instruction. Memory is stored as a string, the 
														//stoi is a command to convert the string into integer value. accumulator 
			break;										//passes this value into the regisiter, end switch statement after execution


		case sw:										//Store Word instruction execution
			ACC = reg[regAddr1];						//only if the memory location is not in the instruction part of memory then
			if(ACC > instCount)							//accumulator stores contents of the register which the address has
				memory[constant][0] = to_string(ACC);   //been passed from the decoded instruction, accumulator passes this 
			break;										//value into decoded memory location. This is stored as a string, so
														//to_string function converts the integer in accumulator into a string
														//to be stored, end switch statement after execution

		case ADD:					//Addition instruction execution
			ACC = reg[regAddr2];    //store the contents of the first regisiter, to be added, in the accumulator
			ALU = ACC;				//copy the value in the accumulator into the ALU to be manipulated
			ACC = reg[regAddr3];    //store the contents of the second regisiter, to be added, in the accumulator
			ALU = ALU + ACC;        //ALU adds together the values stored in both ALU and ACC
			ACC = ALU;				//value calculated in the ALU is passed to accumulator
			reg[regAddr1] = ACC;    //accumulator value is placed in register that is being manipulated
			break;					//end switch statement after execution

		case SUB:					//Subtraction instruction execution
			ACC = reg[regAddr2];    //store the contents of the first regisiter in the accumulator
			ALU = ACC;				//copy the value in the accumulator into the ALU to be manipulated
			ACC = reg[regAddr3];    //store the contents of the second regisiter, to be subtracted, in the accumulator
			ALU = ALU - ACC;        //ALU subtracts value stored in first regisiter by the value in second register
			ACC = ALU;              //value calculated in the ALU is passed to accumulator
			reg[regAddr1] = ACC;    //accumulator value is placed in register that is being manipulated
			break;                  //end switch statement after execution

		case ADDI:                  //Immediate Addition instruction execution
			ACC = reg[regAddr2];    //store the contents of the regisiter in the accumulator
			ALU = ACC;              //copy the value in the accumulator into the ALU to be manipulated
			ACC = constant;         //store the contents of the decoded constant in the accumulator
			ALU = ACC + ALU;        //ALU adds the value stored in the regisiter and the constant
			ACC = ALU;              //value calculated in the ALU is passed to accumulator
			reg[regAddr1] = ACC;    //accumulator value is placed in register that is being manipulated
			break;                  //end switch statement after execution

		case MULT:                  //Multiplication instruction execution
			ACC = reg[regAddr2];    //store the contents of the first regisiter, to be multiplied, in the accumulator
			ALU = ACC;              //copy the value in the accumulator into the ALU to be manipulated
			ACC = reg[regAddr3];    //store the contents of the second regisiter, to be multiplied, in the accumulator
			ALU = ALU * ACC;        //ALU multiplies the values stored in the two regisiter
			ACC = ALU;              //value calculated in the ALU is passed to accumulator
			reg[regAddr1] = ACC;    //accumulator value is placed in register that is being manipulated
			break;                  //end switch statement after execution

		case MOD:                   //Modulus instruction execution
			ACC = reg[regAddr2];    //store the contents of the first regisiter in the accumulator
			ALU = ACC;              //copy the value in the accumulator into the ALU to be manipulate
			ACC = reg[regAddr3];    //store the contents of the second regisiter in the accumulator
			ALU = ALU % ACC;        //ALU find the modulus of first regisister and second regisiter
			ACC = ALU;              //value calculated in the ALU is passed to accumulator
			reg[regAddr1] = ACC;    //accumulator value is placed in register that is being manipulated
			break;                  //end switch statement after execution

		case beq:                   //Branch if equal instruction execution
			ACC = reg[regAddr1];    //store the contents of the first regisiter in the accumulator
			ALU = ACC;				//copy the value in the accumulator into the ALU to be manipulated
			ACC = reg[regAddr2];    //store the contents of the first regisiter in the accumulator
			ALU = ALU - ACC;        //ALU subtracts value stored in first regisiter by the value in second register
			if (ALU == 0)			//ALU causes a branch is the value calculated is zero, this means the values
				PC = constant;      //stored in each register is the same. Branch to instruction placed in decoded
			break;                  //constant in instruction. End switch statement after execution

		case j:                     //Jump instruction execution
			ACC = constant;         //store value of decoded constant inside the accumulator
			PC = ACC;               //send value of accumulator to the program counter
			break;                  //end switch statement after execution
		}
		cout << "Instruction: " << PC << " - " << reg[3] << endl;

		//DEBUG mode

#ifdef DEBUG								//if DEBUG is define, which can be done at the start of the program then this section
		//of code writes to a Log.txt file
		ofstream log;						//output stream class variable called log used to operate of Log.txt file
		log.open("Log.txt", fstream::app);  //open the Log.txt file and append it with the following output
		log << "PC: " << PC << endl;
		log << "Instruction - " << CIR[0] << endl;
		log << "t3 contents: " << reg[3] << endl << endl;  //appends Log.txt file
		log.close();						//close the Log.txt file
#endif										//end of DEBUG section

	}


	system("pause");
}

void getCode()
{
	string word;    //temporary string variable to store words from file
	ifstream file;  //declare input stream class variable to operate on selected file

	//Choose which text file to read
	file.open("Squares.txt", ios::in);
	//file.open("Primes.txt", ios::in);

	for (int i = 0; i < 128; i++)     //initalize memory to all string zeroes
		for (int j = 0; j < 4; j++) {
			memory[i][j] = "0";
		}

	//instMem[100][0] = "100";

	char character;		    //temporary character variable used to find the end of line
	int wordCount = 0;
	while (file >> word)   //while loop iterates until all words in file are read
	{					   //store word in file into word string variable
		memory[instCount][wordCount] = word; //place the word into memory location
		file.get(character);				 //read each character in file
		wordCount++;						 //each while loop iteration increase wordCount 
											 //to moved to next memory location along

		if (character == '\n') {			 //check if it is the end of a line in the file
			instCount++;					 //increase instCount to move to next memory location down
			wordCount = 0;					 //reset wordCount
		}
	}
	return;
}
