#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <utility>
#include <regex>

using namespace std;

// Scripts:
// 1RXY [1058], Store in Register 0 whatever is in Memory address 58
// 2RXY [2058], Store in Register 0 number 58 [XY]
// 3RXY [31A1], Store in Memory A1 whatever in Register 1 
// 40RS [4056], Store in Register 5 whatever in Register 5
// 5RST, Add whatever is in Register S and in Register T [IN 2'S COMPLEMENT FORM] and put the result in Register R
// 6RST, same as 5RST but in floating-point form
// BRXY [B543], Compare whatever in Register 5 to whatever in Register 0, if both are Equal, Jump to memory place 43, if theyre not equal skip
// C000, Stop Executing the porgram

class Memory;
class Register;


class ALU{
protected:
    vector<int> identify_memory(string Pattern) {
        vector<int> index_array;
        for (short i = 0; i < 2; i++) {
            if (isdigit(Pattern[i])) {
                index_array.push_back(Pattern[i] - '0');
            } 
            else if (isalpha(Pattern[i])) {
                switch (toupper(Pattern[i])) { 
                    case 'A': index_array.push_back(10); break;
                    case 'B': index_array.push_back(11); break;
                    case 'C': index_array.push_back(12); break;
                    case 'D': index_array.push_back(13); break;
                    case 'E': index_array.push_back(14); break;
                    case 'F': index_array.push_back(15); break;
                }
            }
        }

        while (index_array.size() < 2) {
            index_array.push_back(0);
        }

        return index_array;
    }
public:
    int HexToDecimal(string Hex){
        return stoi(Hex, nullptr, 16);
    }
    string DecToHex(int Dec) {
        stringstream ss;
        ss << hex << uppercase << Dec;
        return ss.str();
    }
    bool IsAValid_Instruction(string instruction){
        regex Reg("^[1-6BC][1-9A-F]{3}$");
        if(regex_match(instruction, Reg)){
            return true;
        }
        else{
            return false;
        }
    }
};

class CU : public ALU {
public:
    void Execute_1RXY(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }

    void Execute_2RXY(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }

    void Execute_3RXY(Memory &memory, Register &Reg, const string &Instruction) {

    }

    void Execute_40RS(Memory &memory, Register &Reg, const string &Instruction) {

    }

    void Execute_5RST(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }

    void Execute_6RST(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }

    void Execute_BRXY(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }

    void Execute_C000(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }
};


class Memory: public ALU{
private:
    int CurrentStorage = 0;
    string Memory[16][16] = {""};

public:

    void StoreValue(string Value, string Pattern){
        vector<int> Location = identify_memory(Pattern);
        Memory[Location[0]][Location[1]] = Value;
    }


    void StoreInstruction(string Instruction){
        string first_2 = Instruction.substr(0, 2); 
        string second_2 = Instruction.substr(2, 2);
        int row1 = CurrentStorage / 16;
        int col1 = CurrentStorage % 16;
        Memory[row1][col1] = first_2;
        int row2 = (CurrentStorage + 1) / 16;
        int col2 = (CurrentStorage + 1) % 16;
        Memory[row2][col2] = second_2;
        CurrentStorage += 2;
    }

    string GetValue(string Pattern){
        vector<int> Location = identify_memory(Pattern);
        return Memory[Location[0]][Location[1]];
    }

    void DisplayMemory(){
        cout << "Memory: " << endl;
        for(short i{0}; i < 16; i++){
            cout << "[  ";
            for(short j{0}; j < 16; j++){ 
                if(Memory[i][j] == ""){
                    cout << "00";
                }
                else{
                    cout << Memory[i][j];
                }
                if (j < 15) cout << "   ";
            }
            cout << "  ]" << endl;
        }
    }
};



class Register{
private:
    static vector<pair<string, string>> Registers;
public:
    Register(){
        if (Registers.empty()) {
            Registers.resize(16, make_pair("R", "0"));
        }
    }
    Register(const string &RegName, const string &InsideValue){
        if (Registers.empty()) {
            Registers.resize(16, make_pair("R", "0"));
        }
        StoreRegister(RegName, InsideValue);
    }
    void StoreRegister(const string &RegName, const string &InsideValue){
        bool found = false;
        for(auto &reg : Registers){
            if(reg.first == RegName){
                reg.second = InsideValue;
                found = true;
                break;
            }
        }
        if(!found){
            for(auto &reg : Registers){
                if(reg.first == "R"){
                    reg.first = RegName;
                    reg.second = InsideValue;
                    break;
                }
            }
        }
    }
    string GetRegisterContent(const string &RegName) const{
        for(const auto &reg : Registers){
            if(reg.first == RegName){
                return reg.second;
            }
        }
        return "0";
    }
    static void Display_Registers(){
        for(const auto &reg : Registers){
            if(reg.first != "R"){
                cout << "[ " << reg.first << ": " << reg.second << " ]" << endl;
            }
        }
    }
};

vector<pair<string, string>> Register::Registers;


class CPU : public ALU {
private:
    string IR;
    Register Reg;
    CU Cu;
    int PC = 0;
public:
    void Execute_Step(const string &Step, Memory &memory) {
        if (IsAValid_Instruction(Step)) {
            switch (toupper(Step[0])) {
                case '1':
                    Cu.Execute_1RXY(memory, Reg, Step);
                    break;
                case '2':
                    Cu.Execute_2RXY(memory, Reg, Step);
                    break;
                case '3':
                    Cu.Execute_3RXY(memory, Reg, Step);
                    break;
                case '4':
                    Cu.Execute_40RS(memory, Reg, Step);
                    break;
                case '5':
                    Cu.Execute_5RST(memory, Reg, Step);
                    break;
                case '6':
                    Cu.Execute_6RST(memory, Reg, Step);
                    break;
                case 'B':
                    Cu.Execute_BRXY(memory, Reg, Step);
                    break;
                case 'C':
                    Cu.Execute_C000(memory, Reg, Step);
                    break;
            }
        }
    }
};



class Vole_Machine : public ALU{
    private:
    CPU processor;
    Memory program_memory;
    public:
    void Execute_Program(string filepath){
        fstream file(filepath, ios::in);
        string line;
        while(!file.eof()){
            getline(file, line);
            if(IsAValid_Instruction(line)){
                processor.Execute_Step(line, program_memory);
            }
        }
    }
};


int main(){
    // Memory mem;
    // mem.StoreInstruction("102B");
    // mem.StoreInstruction("2096");
    // mem.StoreInstruction("2013");
    // mem.StoreInstruction("1919");
    // mem.StoreValue("10","FF");
    // mem.DisplayMemory();

    // ALU test;
    // cout << test.DecToHex(166) << endl;
    // cout << test.HexToDecimal("FF") << endl;

    // Register reg;
    // reg.StoreRegister("R0","55");
    // reg.StoreRegister("R4","F5");
    // reg.StoreRegister("R2","AB");
    // reg.Display_Registers();
}
