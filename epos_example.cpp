#include <iostream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "MotorController.h"

using std::string;

int main(int argc, char** argv) {
    unsigned int errorCode;

    string deviceName = "EPOS2";
    string protocolName = MAXON_SERIAL_V2;
    string interfaceName = "USB";
    string portName = "USB0";
	unsigned int baudrate = 1e6; 

    epos::MotorController mc; 

    mc.connect(deviceName, protocolName, interfaceName, portName, baudrate);

    long pos = 0;

    while(1) {
        cout << "Next Target Position: ";
        cin >> pos;

        /* Move to position relative/absolute (0/1) position pos wait last move end/immediately (0/1) */
        mc.move(pos, 0, 0);
    }

    return 0;
}