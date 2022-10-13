#include <iostream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "MotorController.h"

using std::string;
using std::cout;
using std::cin;

int main(int argc, char** argv) {
    unsigned int errorCode;

    string deviceName = "EPOS2";
    string protocolName = MAXON_SERIAL_V2;
    string interfaceName = "USB";
    string portName = "USB0";
	unsigned int baudrate = 1e6; 

    epos::MotorController mc; 

    mc.connect(deviceName, protocolName, interfaceName, portName, baudrate);

    unsigned int nodeId = 1;
    long pos = 0;
    epos::MovementProfile movementProfile;
    movementProfile.velocity = 1000; 
    movementProfile.acceleration = 100;
    movementProfile.deceleration = 100;

    while(1) {
        cout << "Next Target Position: ";
        cin >> pos;

        // mc.move(pos, &movementProfile, ABSOLUTE, IMMEDIATE);
        mc.startMovement(nodeId, pos, ABSOLUTE, IMMEDIATE);
        // mc.printMovementProfile(nodeId);
    }

    return 0;
}