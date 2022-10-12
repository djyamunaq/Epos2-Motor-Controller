#include "MotorController.h"
#include <string.h>
#include <iostream>

void epos::MotorController::LogError(string msg, unsigned int errorCode) {
    char* errorInfo = new char[1000];
    if(errorCode == -1) {
        cout << "\033[1;31m[" << std::hex << this->errorCode << std::dec << "] " << msg << "\033[0m\n";
    } else if(VCS_GetErrorInfo(this->errorCode, errorInfo, 1000) == 0) {
        cout << BOLDRED << "Unknown Error" << RESET << endl;
    } else {
        cout << "\033[1;31m[" << std::hex << this->errorCode << std::dec << "] " << msg << ": " << errorInfo << "\033[0m\n";
    }
}

void epos::MotorController::LogInfo(string info) {
    cout << BOLDBLUE << info << RESET << endl;
}

epos::MotorController::MotorController() {

}

epos::MotorController::~MotorController() {

}

/**/
void epos::MotorController::connect(string deviceName, string protocolName, string interfaceName, string portName, unsigned int baudrate) {

    unsigned int errorCode, lBaudrate, lTimeout, nodeId;

    nodeId = 1;

    char* c_deviceName = new char[255];
    strcpy(c_deviceName, deviceName.c_str());

    char* c_protocolName = new char[255];;
    strcpy(c_protocolName, protocolName.c_str());

    char* c_interfaceName = new char[255];;
    strcpy(c_interfaceName, interfaceName.c_str());

    char* c_portName = new char[255];;
    strcpy(c_portName, portName.c_str());

    LogInfo("Connecting...");

    /* Open the port to send and receive commands -> successful if != 0 */
	this->deviceHandle = VCS_OpenDevice(c_deviceName, c_protocolName, c_interfaceName, c_portName, &(this->errorCode));

    delete c_deviceName, c_protocolName, c_interfaceName, c_portName;

    if(this->deviceHandle == 0 || this->errorCode != 0) {
        LogError("Connection Error: ", this->errorCode);
        exit(1);
    }

    LogInfo("Successful connected");

    if(VCS_GetProtocolStackSettings(this->deviceHandle, &lBaudrate, &lTimeout, &(this->errorCode)) == 0) {
        LogError("VCS_GetProtocolStackSettings", this->errorCode);
        exit(1);
    }

    if(VCS_SetProtocolStackSettings(this->deviceHandle, baudrate, lTimeout, &(this->errorCode)) == 0) {
        LogError("VCS_SetProtocolStackSettings", this->errorCode);
        exit(1);
    }

    int isFault;

    if(VCS_GetFaultState(this->deviceHandle, nodeId, &isFault, &(this->errorCode) ) == 0) {
		LogError("VCS_GetFaultState", this->errorCode);
	}

    if(isFault) {
        string info = "clear fault, node = '" + nodeId;
        LogInfo(info);

        if(VCS_ClearFault(this->deviceHandle, nodeId, &(this->errorCode)) == 0) {
            LogError("VCS_ClearFault", this->errorCode);
            exit(1);
        }
    }

    int isEnabled = 0;

    if(VCS_GetEnableState(this->deviceHandle, nodeId, &isEnabled, &(this->errorCode)) == 0) {
        LogError("VCS_GetEnableState", this->errorCode);
        exit(1);
    }

    if(!isEnabled && VCS_SetEnableState(this->deviceHandle, nodeId, &(this->errorCode)) == 0) {
        LogError("VCS_SetEnableState", this->errorCode);
        exit(1);
    }
}

void epos::MotorController::move(unsigned int pos, unsigned int relAbs, unsigned int immWait) {
    /* Params: port handle, node id, target pos, absolute mov (TRUE/FALSE), immediately move (TRUE/FALSE) */
    if(!VCS_MoveToPosition(this->deviceHandle, 1, pos, relAbs, immWait, &(this->errorCode))) {
        string msg = "Error while moving to position " + to_string(pos);
        LogError(msg, -1);
        exit(1);
    }
    sleep(1);
}