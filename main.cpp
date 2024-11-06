#include "VoleMachine.hpp"

int main(){
    Vole_Machine Machine;
    bool result = Menu(Machine);
    if(result = true){
        main();
    }
}

// cout << "~ Vole Machine Program ~" << endl;
//     char answer;
//     string filename;
//     fstream file;
//     Vole_Machine Machine;
//     while(true){
//         cout << "1) Load Program" << endl;
//         cout << "2) Execute Program" << endl;
//         cout << "3) Execute Step By Step" << endl;
//         cout << "4) Restart" << endl;
//         cout << "5) Display Memory & Registers" << endl;
//         cout << "6) Exit" << endl;
//         cin >> answer;
//         switch(answer){
//             case '1':
//                 cout << "Enter File name: ";
//                 cin >> filename;
//                 file.open(filename, ios::in);
//                 while(file.fail()){
//                     cout << "Invalid input, enter a correct file name: ";
//                     cin >> filename;
//                     file.open(filename, ios::in);
//                 }
//                 Machine.StoreInstructions(filename);
//                 break;
//             case '2':
//                 Machine.Execute_Program();
//                 break;
//             case '3':
//                 Machine.Execute_OneStep();
//                 break;
//             case '4':
//                 Machine.Restart();
//                 file.close();
//                 main();
//                 break;
//             case '5':
//                 Machine.DisplayEverything();
//                 break;
//             case '6':
//                 exit(1);
//             default:
//                 cout << "Invalid Answer, Enter a valid one";
//                 cin >> answer;
//         }
        
//     }

// g++ -o VoleMachine main.cpp VoleMachine.cpp

// ./VoleMachine.exe

