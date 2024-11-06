#include "VoleMachine.hpp"

int main(){
    Vole_Machine Machine;
    bool result = Menu(Machine);
    if(result){
        main();
    }
}

