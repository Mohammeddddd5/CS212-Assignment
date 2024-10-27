#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <utility>

using namespace std;

// Scripts:
// 1RXY [1058], Store in Register 0 whatever is in Memory address 58
// 2RXY [2058], Store in Register 0 number 58 [XY]
// 3RXY [31A1], Store in Memory A1 whatever in Register 1 
// 40RS [4056], Store in Register 5 whatever in Register 5
// 5RST, Add whatever is in Register S and in Register T [IN 2'S COMPLEMENT FORM] and put the result in Register R
// 6RST, same as 5RST but in floating-point form
// BRXY [B543], Compare whatever in Register 5 to whatever in Register 0, if both are Equal, Jump to memory place 43, if theyre not equal skip
// C000, Stop Executing the porgram.


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
};

class CU{

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

class CPU{
    private:
    int PC;
    string IR;
    Register Reg;
    ALU alu;
    CU Cu;
    public:
};



class Vole_Machine{

};


int main(){
    Memory mem;
    mem.StoreInstruction("102B");
    mem.StoreInstruction("2096");
    mem.StoreInstruction("2013");
    mem.StoreInstruction("1919");
    mem.StoreValue("10","FF");
    mem.DisplayMemory();

    ALU test;
    cout << test.DecToHex(166) << endl;
    cout << test.HexToDecimal("FF") << endl;

    Register reg;
    reg.StoreRegister("R0","55");
    reg.StoreRegister("R4","F5");
    reg.StoreRegister("R2","AB");
    reg.Display_Registers();
}
