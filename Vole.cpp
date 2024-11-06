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
// 40RS [4056], Store in Register 6 whatever in Register 5
// 5RST, Add whatever is in Register S and in Register T [IN 2'S COMPLEMENT FORM] and put the result in Register R
// 6RST, same as 5RST but in floating-point form
// 7RST, Bitwise (OR) the content of register S and T and put the result in register R ~BONUS~
// 8RST, Bitwise (AND) the content of register S and T and put the result in register R ~BONUS~
// 9RST, Bitwise (XOR) the content of register S and T and put the result in register R ~BONUS~
// ARxX, Rotate the content of Register R cyclically right X steps ~BONUS~
// BRXY [B543], Compare whatever in Register 5 to whatever in Register 0, if both are Equal, Jump to memory place 43, if theyre not equal skip
// C000, Stop Executing the porgram
// DRXY, Jump to instruction in Cell XY, if content of register R is greater than the content in Register 0, data is interpreted as integers in two's complement notation ~BONUS~

// Register 0: 05
// Register 1: 00
// Register 2: 01
// Add Register 1 [08] to Register 2 [AC] and put the result in Register 1 [=01], Register 1: 01
// Store in Memory [0][0] whatever in Register 1 [B4]
// Store in Register 5 whatever in Register 0, Register 5 = 05
// Store in Register 9 whatever in Reg1 and Reg5, Register 9 = 05
// Bitwise OR Register 9 and Register 5 and put result in Register B
// Bitwise AND Register 0 and Register B and put result in Register C
// Bitwise XOR Register  and Register C and put result in Register F

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
        regex Reg("^[1-9A-D][0-9A-F]{3}$");
        if(regex_match(instruction, Reg)){
            return true;
        }
        else{
            return false;
        }
    }
};

class Register{
private:
    static vector<pair<string, string>> Registers;
public:
    Register(){
        if(Registers.empty()){
            Registers.resize(16, make_pair("R", "0"));
        }
    }
    Register(const string &RegName, const string &InsideValue){
        if(Registers.empty()){
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
    void FindANDStore(string regname, string store){
        bool found = false;
        for(short i{0}; i < Registers.size(); i++){
            if(Registers[i].first == regname){
                Registers[i].second = store;
                found = true;
            }
        }
        if(!found){
            StoreRegister(regname, store);
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
    void Clear_Register(){
        Registers.clear();
    }
};

vector<pair<string, string>> Register::Registers; 



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
        Memory[row1+1][col1] = first_2;
        int row2 = (CurrentStorage + 1) / 16;
        int col2 = (CurrentStorage + 1) % 16;
        Memory[row2+1][col2] = second_2;
        CurrentStorage += 2;
    }

    string GetValue(string Pattern){
        vector<int> Location = identify_memory(Pattern);
        return Memory[Location[0]][Location[1]];
    }
    string GetValue(int row, int column) const {
        return Memory[row][column];
    }

    void DisplayMemory(){
        cout << "Memory: " << endl;
        for(short i{0}; i < 16; i++){
            cout << "[  ";
            for(short j{0}; j < 16; j++){ 
                if(Memory[i][j] == "" || Memory[i][j] == "0"){
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
    void Clear_Memory(){
        for(short i{0}; i < 16; i++){
            for(short j{0}; j < 16; j++){
                Memory[i][j] = "";
            }
        }
        CurrentStorage = 0;
    }
};


class CU : public ALU {
public:
    void Execute_1RXY(Memory &memory, Register &Reg, const string &Instruction) {
        char registerChar = Instruction[1];
        string memoryAddress = Instruction.substr(2, 2);
        string value = memory.GetValue(memoryAddress);
        string registerName = "R";
        registerName += registerChar;
        Reg.StoreRegister(registerName, value);
    }

    
    void Execute_2RXY(Memory &memory, Register &Reg, const string &Instruction) {
        char registerChar = Instruction[1];
        string immediateValue = Instruction.substr(2, 2);
        string registerName = "R";
        registerName += registerChar;
        Reg.StoreRegister(registerName, immediateValue);
    }

    void Execute_3RXY(Memory &memory, Register &reg, const string &Instruction) {
        string registerName = "R";
        registerName += Instruction[1];
        string memory_address = Instruction.substr(2, 2);
        string value = reg.GetRegisterContent(registerName);
        memory.StoreValue(value, memory_address);
    }

    void Execute_40RS(Memory &memory, Register &reg, const string &instruction){
        string register_R = "R";
        register_R += instruction[2];
        string register_S = "R";
        register_S += instruction[3]; 

        string value_R = reg.GetRegisterContent(register_R);
        reg.StoreRegister(register_S, value_R);
    }


    void Execute_5RST(Memory &memory, Register &reg, const string &instruction) {
        string register_R = "R";
        register_R += instruction[1];
        string register_S = "R";
        register_S += instruction[2];
        string register_T = "R";
        register_T += instruction[3];

        int value_S = stoi(reg.GetRegisterContent(register_S), nullptr, 16);
        int value_T = stoi(reg.GetRegisterContent(register_T), nullptr, 16);
        int result = value_S + value_T;

        string result_hex = DecToHex(result);
        reg.StoreRegister(register_R, result_hex);
    }

    void Execute_6RST(Memory &memory, Register &reg, const string &instruction) {
        string register_R = "R";
        register_R += instruction[1];
        string register_S = "R";
        register_S += instruction[2];
        string register_T = "R";
        register_T += instruction[3];

        string value_S_hex = reg.GetRegisterContent(register_S);
        string value_T_hex = reg.GetRegisterContent(register_T);

        float value_S_float = HexToDecimal(value_S_hex);
        float value_T_float = HexToDecimal(value_T_hex);
        float result_float = value_S_float + value_T_float;

        string result_hex = DecToHex(result_float);

        reg.StoreRegister(register_R, result_hex);
    }

    void Execute_789RST(Register &Reg, const string Instruction){  // 712F
        int result;
        string registerR = "R";
        registerR.push_back(Instruction[1]);
        string registerS = "R";
        registerS.push_back(Instruction[2]);
        string registerT = "R";
        registerT.push_back(Instruction[3]);
        
        string regSContent = Reg.GetRegisterContent(registerS);
        string regTContent = Reg.GetRegisterContent(registerT);


        int decimal_regS = HexToDecimal(regSContent);
        int decimal_regT = HexToDecimal(regTContent);

        switch(Instruction[0]){
            case '7':
                result = decimal_regS | decimal_regT;
                break;
            case '8':
                result = decimal_regS & decimal_regT;
                break;
            case '9':
                result = decimal_regS ^ decimal_regT;
                break;
        }

        Reg.FindANDStore(registerR, DecToHex(result));
    }


    void Execute_ARxX(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }   

    void Execute_BRXY(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }

    void Execute_C000(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }

    void Execute_DRXY(Memory &memory, Register &Reg, const string &Instruction) {
        // Implementation here
    }
};


class CPU : public ALU{
public:
    string IR;
    Register Reg;
    CU Cu;
    int PC = 0;
    void SetRegister(Register& reg){
        Reg = reg;
    }
    void Execute_Step(const string &Step, Memory &memory){
        if(IsAValid_Instruction(Step)){
            switch (toupper(Step[0])){
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
                case '7':
                case '8':
                case '9':
                    Cu.Execute_789RST(Reg, Step);
                    break;
                case 'A':
                    Cu.Execute_ARxX(memory, Reg, Step);
                    break;
                case 'B':
                    Cu.Execute_BRXY(memory, Reg, Step);
                    break;
                case 'C':
                    Cu.Execute_C000(memory, Reg, Step);
                    break;
                case 'D':
                    Cu.Execute_DRXY(memory, Reg, Step);
                    break;
            }
        }
    }
    void DisplayRegisters(){
        Reg.Display_Registers();
    }
    void clear_register(){
        Reg.Clear_Register();
    }
};



class Vole_Machine : public ALU{
private:
    CPU processor;
    Memory program_memory;
    string Step;
    int CurrentStep = 0;  
    bool AllStepsFinished = false;

public:
    void StoreInstructions(string filepath){
        string line;
        fstream file(filepath, ios::in);
        while (!file.eof()){
            getline(file, line);
            if (IsAValid_Instruction(line)){
                program_memory.StoreInstruction(line);
                processor.PC++;
            }
        }
    }

    void DisplayEverything(){
        program_memory.DisplayMemory();
        processor.DisplayRegisters();
        cout << "PC: " << processor.PC << endl;
    }

    void Restart(){
        program_memory.Clear_Memory();
        processor.clear_register();
        processor.PC = 0;
        CurrentStep = 0;
        AllStepsFinished = false; 
    }

    void Execute_Program(){
        if(AllStepsFinished = false){
            for (int i = 1; i < 16; i++){
                for (int j = 0; j < 16; j += 2){
                    Step.append(program_memory.GetValue(i, j));
                    Step.append(program_memory.GetValue(i, j + 1));
                    if (IsAValid_Instruction(Step)) {
                        processor.Execute_Step(Step, program_memory);
                    }
                    else{
                        AllStepsFinished = true;
                        return;
                    }
                    Step.clear();
                }
            }
        }
    }

    void Execute_OneStep(){
        int row = 1 + CurrentStep/16;
        int col = (CurrentStep % 16) * 2;

        if (row >= 16 || col >= 16){
            cout << "No more steps to execute." << endl;
            return;
        }

        Step.append(program_memory.GetValue(row, col));
        Step.append(program_memory.GetValue(row, col + 1));

        if (IsAValid_Instruction(Step)){
            processor.Execute_Step(Step, program_memory);
            cout << "Step number " << CurrentStep + 1 << " have been Executed." << endl;
            CurrentStep++; 
        }
        else{
            cout << "Invalid instruction at Step [" << CurrentStep + 1 << "]." << endl;
        }

        Step.clear();
    }
};


int main(){
    cout << "~ Vole Machine Program ~" << endl;
    char answer;
    string filename;
    fstream file;
    Vole_Machine Machine;
    while(true){
        cout << "1) Load Program" << endl;
        cout << "2) Execute Program" << endl;
        cout << "3) Execute Step By Step" << endl;
        cout << "4) Restart" << endl;
        cout << "5) Display Memory & Registers" << endl;
        cout << "6) Exit" << endl;
        cin >> answer;
        switch(answer){
            case '1':
                cout << "Enter File name: ";
                cin >> filename;
                file.open(filename, ios::in);
                while(file.fail()){
                    cout << "Invalid input, enter a correct file name: ";
                    cin >> filename;
                    file.open(filename, ios::in);
                }
                Machine.StoreInstructions(filename);
                break;
            case '2':
                Machine.Execute_Program();
                break;
            case '3':
                Machine.Execute_OneStep();
                break;
            case '4':
                Machine.Restart();
                file.close();
                break;
            case '5':
                Machine.DisplayEverything();
                break;
            case '6':
                exit(1);
            default:
                cout << "Invalid Answer, Enter a valid one";
                cin >> answer;
        }
        
    }
}


    // Memory mem;
    // mem.StoreInstruction("102B");
    // mem.StoreInstruction("2096");
    // mem.StoreInstruction("2013");
    // mem.StoreInstruction("1919");
    // mem.StoreInstruction("1244");
    // mem.StoreValue("10","FF");
    // cout << "---------" << mem.GetValue("05");  
    // Register reg;
    // reg.StoreRegister("R0","32");
    // reg.StoreRegister("R4","A4");
    // reg.StoreRegister("R2","AB");
    // mem.DisplayMemory();
    // reg.Display_Registers()
    // cout << "Executing 7RST with instruction '____':" << endl
    // CPU Cpu;
    // Cpu.SetRegister(reg);
    // Cpu.Execute_Step("7504", mem);
    // Cpu.Execute_Step("8504", mem);
    // Cpu.Execute_Step("9504", mem);
    // cout << "After Execution of ____:" << endl;
    // reg.Display_Registers();
    // mem.DisplayMemory();
    // return 0;
