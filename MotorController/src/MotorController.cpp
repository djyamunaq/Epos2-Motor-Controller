#include "MotorController.h"
#include <string.h>
#include <iostream>

void epos::LogError(string functionName, unsigned int errorCode) {
    char* errorInfo = new char[1000];
    if(VCS_GetErrorInfo(errorCode, errorInfo, 1000) == 0) {
        cout << BOLDRED << "Unknown Error" << RESET << endl;
        return;
    }
    cout << "\033[1;31m[" << std::hex << errorCode << std::dec << "] " << functionName << ": " << errorInfo << "\033[0m\n";
}

void epos::LogInfo(string info) {
    cout << BOLDBLUE << info << RESET << endl;
}

epos::MotorController::MotorController() {

}

epos::MotorController::~MotorController() {

}
