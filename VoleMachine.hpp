#ifndef VOLEMACHINE_HPP
#define VOLEMACHINE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <utility>
#include <regex>
#include <bitset>
#include <cstring>
#include <iomanip>


using namespace std;

class Memory;
class Register;

class ALU {
protected:
    vector<int> identify_memory(string Pattern);
public:
    int HexToDecimal(string Hex);
    string DecToHex(int Dec);
    bool IsAValid_Instruction(string instruction);
};

class Register {
private:
    static vector<pair<string, string>> Registers;
public:
    Register();
    Register(const string &RegName, const string &InsideValue);
    void StoreRegister(const string &RegName, const string &InsideValue);
    void FindANDStore(string regname, string store);
    string GetRegisterContent(const string &RegName) const;
    static void Display_Registers();
    void Clear_Register();
};

class Memory : public ALU {
private:
    int CurrentStorage = 0;
    string Memory[16][16] = {""};

public:
    void StoreValue(string Value, string Pattern);
    void StoreInstruction(string Instruction);
    string GetValue(string Pattern);
    string GetValue(int row, int column) const;
    void DisplayMemory();
    void Clear_Memory();
};

class CU : public ALU {
public:
    void Execute_1RXY(Memory &memory, Register &Reg, const string &Instruction);
    void Execute_2RXY(Memory &memory, Register &Reg, const string &Instruction);
    void Execute_3RXY(Memory &memory, Register &Reg, const string &Instruction);
    void Execute_40RS(Memory &memory, Register &reg, const string &instruction);
    void Execute_5RST(Memory &memory, Register &reg, const string &instruction);
    void Execute_6RST(Memory &memory, Register &reg, const string &instruction);
    void Execute_789RST(Register &Reg, const string Instruction);
    void Execute_ARxX(Memory &memory, Register &Reg, const string &Instruction);
    void Execute_BRXY(Memory &memory, Register &Reg, const string &Instruction);
    void Execute_C000(Memory &memory, Register &Reg, const string &Instruction);
    void Execute_DRXY(Memory &memory, Register &Reg, const string &Instruction);
};

class CPU : public ALU {
public:
    string IR;
    Register Reg;
    CU Cu;
    int PC = 0;
    void SetRegister(Register& reg);
    void Execute_Step(const string &Step, Memory &memory);
    void DisplayRegisters();
    void clear_register();
};

class Vole_Machine : public ALU {
private:
    CPU processor;
    Memory program_memory;
    string Step;
    int CurrentStep = 0;
    bool AllStepsFinished = false;

public:
    void StoreInstructions(string filepath);
    void DisplayEverything();
    void Restart();
    void Execute_Program();
    void Execute_OneStep();
};

bool Menu(Vole_Machine& Machine);

#endif
