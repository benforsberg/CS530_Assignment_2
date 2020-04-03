#include "Opcode.h"
#include <stdio.h>
#include <iostream>

int main() {
    cout<<Opcode::getInstruction(Opcode::findOpcode("84"));
    //cout << "Hello World!";
}