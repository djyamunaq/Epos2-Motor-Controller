#include <iostream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "MotorController.h"

using std::string;

int main(int argc, char** argv) {
    unsigned int errorCode;

    char* deviceName = new char[255];
    char* protocolName = new char[255];
    char* interfaceName = new char[255];
    char* portName = new char[255];
    unsigned int nodeId = 1;
	unsigned int baudRate = 1e6; 

    strcpy(deviceName, "EPOS2");
    strcpy(protocolName, MAXON_SERIAL_V2);
    strcpy(interfaceName, "USB");
    strcpy(portName, "USB0");

    char* nameSel = new char[1000];
    // VCS_GetInterfaceNameSelection(deviceName, protocolName, TRUE, interfaceNameSel, 1000, NULL, &errorCode);

    // VCS_GetDeviceNameSelection(FALSE, nameSel, 1000, NULL, &errorCode);

    cout << "OKOK: ";
    cout << nameSel << endl;;
    exit(1);

    epos::LogInfo("Connecting...");

    /* Open the port to send and receive commands -> successful if != 0 */
	HANDLE deviceHandle = VCS_OpenDevice(deviceName, protocolName, interfaceName, portName, &errorCode);

    if(deviceHandle == 0 || errorCode != 0) {
        epos::LogError("Connection Error: ", errorCode);
        return -1;
    }

    epos::LogInfo("Successful connected");

    unsigned int lBaudrate = 0;
    unsigned int lTimeout = 0;
    unsigned int result;

    if(VCS_GetProtocolStackSettings(deviceHandle, &lBaudrate, &lTimeout, &errorCode) == 0) {
        epos::LogError("VCS_GetProtocolStackSettings", errorCode);
        return -1;
    }

    if(VCS_SetProtocolStackSettings(deviceHandle, baudRate, lTimeout, &errorCode) == 0) {
        epos::LogError("VCS_SetProtocolStackSettings", errorCode);
        return -1;
    }

    int isFault;

    if(VCS_GetFaultState(deviceHandle, nodeId, &isFault, &errorCode ) == 0) {
		epos::LogError("VCS_GetFaultState", errorCode);
	}

    if(isFault) {
        string info = "clear fault, node = '" + nodeId;
        epos::LogInfo(info);

        if(VCS_ClearFault(deviceHandle, nodeId, &errorCode) == 0) {
            epos::LogError("VCS_ClearFault", errorCode);
            return -1;
        }
    }

    int isEnabled = 0;

    if(VCS_GetEnableState(deviceHandle, nodeId, &isEnabled, &errorCode) == 0) {
        epos::LogError("VCS_GetEnableState", errorCode);
        return -1;
    }

    if(!isEnabled && VCS_SetEnableState(deviceHandle, nodeId, &errorCode) == 0) {
        epos::LogError("VCS_SetEnableState", errorCode);
    }

    long pos = 0;

    do {
        cout << "Next Target Position: ";
        cin >> pos;

        /* Params: port handle, node id, target pos, absolute mov (TRUE/FALSE), immediately move (TRUE/FALSE) */
        if(!VCS_MoveToPosition(deviceHandle, 1, pos, FALSE, TRUE, 0)) {
            cout << "Error while moving to position " << pos << endl;
            return -1;
        }
        sleep(1);
    } while(1);

    return 0;
}