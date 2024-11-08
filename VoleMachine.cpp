#include "VoleMachine.hpp"

////////////////// ALU Class //////////////////

vector<int> ALU::identify_memory(string Pattern){
    vector<int> index_array;
    for (short i = 0; i < 2; i++){
        if (isdigit(Pattern[i])){
            index_array.push_back(Pattern[i] - '0');
        } 
        else if (isalpha(Pattern[i])){
            switch (toupper(Pattern[i])){ 
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

int ALU::HexToDecimal(string Hex){
    return stoi(Hex, nullptr, 16);
}

string ALU::DecToHex(int Dec){
    stringstream ss;
    ss << hex << uppercase << Dec;
    return ss.str();
}

bool ALU::IsAValid_Instruction(string instruction){
    regex Reg("^[1-9A-D][0-9A-F]{3}$");
    if(regex_match(instruction, Reg)){
        return true;
    }
    else{
        return false;
    }
}

////////////////// Register Class //////////////////

vector<pair<string, string>> Register::Registers;

Register::Register(){
    if(Registers.empty()){
        Registers.resize(16, make_pair("R", "0"));
    }
}

Register::Register(const string &RegName, const string &InsideValue){
    if(Registers.empty()){
        Registers.resize(16, make_pair("R", "0"));
    }
    StoreRegister(RegName, InsideValue);
}

void Register::StoreRegister(const string &RegName, const string &InsideValue){
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

void Register::FindANDStore(string regname, string store){
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

string Register::GetRegisterContent(const string &RegName) const{
    for(const auto &reg : Registers){
        if(reg.first == RegName){
            return reg.second;
        }
    }
    return "0";
}

void Register::Display_Registers(){
    for(const auto &reg : Registers){
        if(reg.first != "R"){
            cout << "[ " << reg.first << ": " << reg.second << " ]" << endl;
        }
    }
}

void Register::Clear_Register(){
    Registers.clear();
    Display_Registers();
}

////////////////// Memory Class //////////////////

void Memory::StoreValue(string Value, string Pattern){
    vector<int> Location = identify_memory(Pattern);
    Memory[Location[0]][Location[1]] = Value;
}

void Memory::StoreInstruction(string Instruction){
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

string Memory::GetValue(string Pattern){
    vector<int> Location = identify_memory(Pattern);
    return Memory[Location[0]][Location[1]];
}

string Memory::GetValue(int row, int column) const {
    return Memory[row][column];
}

void Memory::DisplayMemory(){
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

void Memory::Clear_Memory(){
    for(short i{0}; i < 16; i++){
        for(short j{0}; j < 16; j++){
            Memory[i][j] = "";
        }
    }
    CurrentStorage = 0;
}

////////////////// CU Class //////////////////

void CU::Execute_1RXY(Memory &memory, Register &Reg, const string &Instruction) {
    char registerChar = Instruction[1];
    string memoryAddress = Instruction.substr(2, 2);
    string value = memory.GetValue(memoryAddress);
    string registerName = "R";
    registerName += registerChar;
    Reg.StoreRegister(registerName, value);
}


void CU::Execute_2RXY(Memory &memory, Register &Reg, const string &Instruction) {
    char registerChar = Instruction[1];
    string immediateValue = Instruction.substr(2, 2);
    string registerName = "R";
    registerName += registerChar;
    Reg.StoreRegister(registerName, immediateValue);
}

void CU::Execute_3RXY(Memory &memory, Register &reg, const string &Instruction) {
    string registerName = "R";
    registerName += Instruction[1];
    string memory_address = Instruction.substr(2, 2);
    string value = reg.GetRegisterContent(registerName);
    memory.StoreValue(value, memory_address);
}

void CU::Execute_40RS(Memory &memory, Register &reg, const string &instruction){
    string register_R = "R";
    register_R += instruction[2];
    string register_S = "R";
    register_S += instruction[3]; 

    string value_R = reg.GetRegisterContent(register_R);
    reg.StoreRegister(register_S, value_R);
}

void CU::Execute_5RST(Memory &memory, Register &reg, const string &instruction) {
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

void CU::Execute_6RST(Memory &memory, Register &reg, const string &instruction) {
    string register_R = "R";
    register_R += instruction[1];
    string register_S = "R";
    register_S += instruction[2];
    string register_T = "R";
    register_T += instruction[3];

    string value_S_hex = reg.GetRegisterContent(register_S);
    string value_T_hex = reg.GetRegisterContent(register_T);

    uint32_t int_rep_S = stoi(value_S_hex, nullptr, 16);
    uint32_t int_rep_T = stoi(value_T_hex, nullptr, 16);

    float float_S, float_T;
    std::memcpy(&float_S, &int_rep_S, sizeof(float));
    std::memcpy(&float_T, &int_rep_T, sizeof(float));

    float result_float = float_S + float_T;

    uint32_t int_result;
    std::memcpy(&int_result, &result_float, sizeof(float));

    string result_hex = DecToHex(int_result);
    reg.StoreRegister(register_R, result_hex);
}

void CU::Execute_789RST(Register &Reg, const string Instruction){
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

void CU::Execute_ARxX(Memory &memory, Register &reg, const string &instruction) {
    string register_R = "R";
    register_R += instruction[1];
    string X = reg.GetRegisterContent(register_R);
    unsigned int num;
    std::stringstream ss;
    ss << std::hex << X;
    ss >> num;
    int Z = HexToDecimal(string(1, instruction[3]));
    unsigned int rotated = (num >> Z) | (num << (8 - Z)); 
    rotated &= 0xFF;
    std::stringstream result;
    result << std::hex << std::setw(2) << std::setfill('0') << rotated;
    string res = result.str();
    transform(res.begin(), res.end(), res.begin(), ::toupper);
    reg.FindANDStore(register_R, res);
}


void CU::Execute_BRXY(Memory &memory, Register &reg, CPU &cpu, const string &instruction) {
    string branch_address = instruction.substr(2, 2);
    int register_R_value = stoi(reg.GetRegisterContent("R"), nullptr, 16);
    int register_0_value = stoi(reg.GetRegisterContent("0"), nullptr, 16);
    if (register_R_value > register_0_value) {
        cpu.SetPC(HexToDecimal(branch_address));
    } else {
        cpu.SetPC(cpu.GetPC() + 1);
    }
}


void CU::Execute_DRXY(Memory &memory, Register &reg, const string &instruction) {
    string register_name = "R";
    register_name += instruction[1];
    int decrement_value = HexToDecimal(instruction.substr(2, 2));

    int register_value = HexToDecimal(reg.GetRegisterContent(register_name));
    int result = register_value - decrement_value;

    reg.StoreRegister(register_name, DecToHex(result));
}

////////////////// CPU Class //////////////////


void CPU::SetRegister(Register& reg){
    Reg = reg;
}

void CPU::JUMP(int X, int Y) {
    StepCounter[0] = X;
    StepCounter[1] = Y;
}

int CPU::GetPC(){
    return PC;
}

void CPU::SetPC(int value) {
    PC = value;
}

void CPU::Execute_Step(Memory &memory) {
    int row = StepCounter[0];
    int col = StepCounter[1] * 2;
    string Result;
    string Step = memory.GetValue(row, col) + memory.GetValue(row, col + 1);
    transform(Step.begin(), Step.end(), Step.begin(), ::toupper);
    
    if (IsAValid_Instruction(Step)) {
        switch(toupper(Step[0])) {
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
                Cu.Execute_BRXY(memory, Reg, *this, Step);
                break;
            case 'C':
                return;
            case 'D':
                Cu.Execute_DRXY(memory, Reg, Step);
                break;
        }
    }
    else {
        cout << "Invalid instruction at [" << StepCounter[0] << "][" << StepCounter[1] << "]" << endl;
        return;
    }
    PC++;
    StepCounter[1]++;
    if (StepCounter[1] == 16) {
        StepCounter[1] = 0;
        StepCounter[0]++;
    }
}

void CPU::DisplayRegisters(){
    Reg.Display_Registers();
}

void CPU::clear_register(){
    Reg.Clear_Register();
}

void CPU::UpdateStepCounter() {
    StepCounter[1] += 1;
    if (StepCounter[1] == 16) {
        StepCounter[1] = 0;
        StepCounter[0] += 1; 
    }
}

////////////////// Vole Machine Class //////////////////

void Vole_Machine::StoreInstructions(string filepath){
    processor.PC = 0;
    string line;
    fstream file(filepath, ios::in);
    while (!file.eof()){
        getline(file, line);
        if (IsAValid_Instruction(line)){
            program_memory.StoreInstruction(line);
        }
    }
}

void Vole_Machine::DisplayEverything(bool print){
    program_memory.DisplayMemory();
    processor.DisplayRegisters();
    if(print){
        cout << "PC: " << processor.PC << endl;
        cout << "IR: " << processor.IR << endl;
    }
}

void Vole_Machine::Restart(){
    program_memory.Clear_Memory();
    processor.clear_register();
    processor.PC = 0;
    CurrentStep = 0;
    AllStepsFinished = false; 
}

void Vole_Machine::Execute_Program(){
    if(AllStepsFinished == true){
        cout << "All steps have been already executed." << endl;
        return;
    }
    processor.PC = 0;
    for (int i = 1; i < 16; i++){
        for (int j = 0; j < 16; j += 2){
            Step.append(program_memory.GetValue(i, j));
            Step.append(program_memory.GetValue(i, j + 1));
            transform(Step.begin(), Step.end(), Step.begin(), ::toupper);
            if (IsAValid_Instruction(Step)){
                if(Step[0] == 'C'){
                    processor.PC++;
                    AllStepsFinished = true;
                    return;
                }
                processor.Execute_Step(program_memory);
            }
            else{
                AllStepsFinished = true;
                return;
            }
            processor.IR = Step;
            Step.clear();
        }
    }
    
}

void Vole_Machine::Execute_OneStep() {
    if (AllStepsFinished) {
        cout << "All steps have already been executed." << endl;
        return;
    }
    int row = 1 + (processor.PC / 8);
    int col = (processor.PC % 8) * 2;
    if (row >= 16) {
        cout << "No more steps to execute: end of memory reached." << endl;
        AllStepsFinished = true;
        return;
    }
    Step.append(program_memory.GetValue(row, col));
    Step.append(program_memory.GetValue(row, col + 1));
    transform(Step.begin(), Step.end(), Step.begin(), ::toupper);


    if (IsAValid_Instruction(Step)) {
        if (Step[0] == 'C') {
            cout << "Step: " << Step << " Has been Executed." << endl;
            cout << "No more steps to execute: Cxxx reached." << endl;
            processor.PC++;
            AllStepsFinished = true;
            return;
        }
        processor.Execute_Step(program_memory);
        cout << "Step: " << Step << " Has been Executed." << endl;   
    } else {
        cout << "Invalid instruction at PC [" << processor.PC << "]: " << Step << endl;
        AllStepsFinished = true;
        return;
    }

    processor.IR = Step;
    Step.clear();
}

////////////////// Main Menu //////////////////

bool Menu(Vole_Machine& Machine){
    bool print_type = true;
    cout << "~ Vole Machine Program ~" << endl;
    char answer;
    string filename;
    fstream file;
    while(true){
        cout << "1) Load Program" << endl;
        cout << "2) Execute Program" << endl;
        cout << "3) Execute One Step" << endl;
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
                print_type = false;
                break;
            case '3':
                Machine.Execute_OneStep();
                break;
            case '4':
                Machine.Restart();
                file.close();
                return true; 
                break;
            case '5':
                Machine.DisplayEverything(print_type);
                break;
            case '6':
                exit(1);
            default:
                cout << "Invalid Answer, Enter a valid one";
                cin >> answer;
        }
        
    }
}
