import serial
import time
from ctypes import *

# EPOS Command Library path
path = './epos-lib/libEposCmd.so.6.8.1.0'

# Load library
cdll.LoadLibrary(path)
epos = CDLL(path)

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class CONSTANTS:
    RELATIVE = 0
    ABSOLUTE = 1
    WAIT = 0
    IMMEDIATE = 1
    
#include "MotorController.h"
#include <string.h>
#include <iostream>

class MotorController:
    def __init__(self):
        self.deviceHandle = c_int(0)
        self.errorCode = c_int(0)
        self.baudrate = c_int(0)
        self.movementProfiles = {}
    
    def LogError(self, info, errorCode):
        if errorCode is None:
            msg = f'{bcolors.FAIL}' + str(info) + f'{bcolors.ENDC}'
            print(msg)
            return

        errorInfo = create_string_buffer(1000)
        if epos.VCS_GetErrorInfo(errorCode, byref(errorInfo), 1000) == 0:
            msg = f'{bcolors.FAIL}[' + str(hex(errorCode.value)) + '] Unknown error' + f'{bcolors.ENDC}'
            print(msg)
            return

        msg = f'{bcolors.FAIL}[' + str(hex(errorCode.value)) + '] ' + str(info) + ': ' + errorInfo.value.decode('utf-8') + f'{bcolors.ENDC}'

        print(msg)

    def LogInfo(self, info):
        msg = f'{bcolors.OKBLUE}' + str(info) + f'{bcolors.ENDC}' 
        print(msg)

    def LogDebug(self, info):
        msg = f'{bcolors.WARNING}' + str(info) + f'{bcolors.ENDC}' 
        print(msg)

    def checkMotorState(self, nodeId):
        lBaudrate = c_int(0)
        lTimeout = c_int(0)

        if epos.VCS_GetProtocolStackSettings(self.deviceHandle, byref(lBaudrate), byref(lTimeout), byref(self.errorCode)) == 0:
            self.LogError('VCS_GetProtocolStackSettings', self.errorCode)
            exit(1)

        if epos.VCS_SetProtocolStackSettings(self.deviceHandle, self.baudrate, lTimeout, byref(self.errorCode)) == 0:
            self.LogError('VCS_SetProtocolStackSettings', self.errorCode)
            exit(1)

        isFault = c_uint(0)

        if epos.VCS_GetFaultState(self.deviceHandle, nodeId, byref(isFault), byref(self.errorCode)) == 0:
            self.LogError('VCS_GetFaultState', self.errorCode)

        if isFault:
            info = 'clear fault, node = ' + nodeId
            self.LogInfo(info)

            if epos.VCS_ClearFault(self.deviceHandle, nodeId, byref(self.errorCode)) == 0 :
                self.LogError('VCS_ClearFault', self.errorCode)
                exit(1)
                
        isEnabled = c_uint(0)

        if epos.VCS_GetEnableState(self.deviceHandle, nodeId, byref(isEnabled), byref(self.errorCode)) == 0:
            self.LogError('VCS_GetEnableState', self.errorCode)
            exit(1)

        if not isEnabled and epos.VCS_SetEnableState(self.deviceHandle, nodeId, byref(self.errorCode)) == 0:
            self.LogError('VCS_SetEnableState', self.errorCode)

    def connect(self, deviceName, protocolName, interfaceName, portName, baudrate):
        self.baudrate = baudrate

        c_deviceName = c_char_p(deviceName.encode('utf-8'))
        c_protocolName = c_char_p(protocolName.encode('utf-8'))
        c_interfaceName = c_char_p(interfaceName.encode('utf-8'))
        c_portName = c_char_p(portName.encode('utf-8'))

        self.LogInfo('Connecting...')

        # Open the port to send and receive commands -> successful if != 0
        self.deviceHandle = epos.VCS_OpenDevice(c_deviceName, c_protocolName, c_interfaceName, c_portName, byref(self.errorCode))

        if self.deviceHandle == 0 or self.errorCode.value != 0:
            self.LogError('Error connecting to Port:', self.errorCode.value)
            exit(1)

        self.LogInfo("Successfully connected")

    def startMovement(self, nodeId, pos, relAbs, immWait):
        self.checkMotorState(nodeId)

        c_nodeId = c_int(nodeId)
        c_pos = c_int(pos)
        c_relAbs = c_int(relAbs)
        c_immWait = c_int(immWait)

        movementProfile = self.getMovementProfile(nodeId)

        if epos.VCS_SetPositionProfile(self.deviceHandle, nodeId, movementProfile['velocity'], movementProfile['acceleration'], movementProfile['deceleration'], byref(self.errorCode)) == 0:
            self.LogError('startMovement->VCS_SetPositionProfile', self.errorCode)
            exit(1)

        if epos.VCS_MoveToPosition(self.deviceHandle, c_nodeId, c_pos, c_relAbs, c_immWait, byref(self.errorCode)) == 0:
            msg = 'Error while moving to position ' + str(pos)
            self.LogError(msg, None)
            exit(1)
        
        time.sleep(1)

    def stopMovement(self, nodeId):
        if epos.VCS_HaltPositionMovement(self.deviceHandle, nodeId, byref(self.errorCode)) == 0:
            self.LogError("stopMovement", self.errorCode)
            exit(1)

    def setMovementProfile(self, nodeId, movementProfile):
        self.movementProfiles[nodeId] = movementProfile

    def getMovementProfile(self, nodeId):
        if nodeId not in self.movementProfiles:
            velocity = c_int(0) 
            acceleration = c_int(0) 
            deceleration = c_int(0)

            if epos.VCS_GetPositionProfile(self.deviceHandle, nodeId, byref(velocity), byref(acceleration), byref(deceleration), byref(self.errorCode)) == 0:
                self.LogError("getMovementProfile", self.errorCode)
                exit(1)
            
            movementProfile = {'velocity': velocity, 'acceleration': acceleration, 'deceleration': deceleration}

            self.movementProfiles[nodeId] = movementProfile

            return movementProfile

        return self.movementProfiles[nodeId]

    def printMovementProfile(self, nodeId):

        movementProfile = self.getMovementProfile(nodeId)
    
        msg = "Movement Profile -> Node " + str(nodeId)
        self.LogInfo(msg)

        msg = "\tVelocity: " + str(movementProfile['velocity'].value)
        self.LogInfo(msg)

        msg = "\tAcceleration: " + str(movementProfile['acceleration'].value)
        self.LogInfo(msg)

        msg = "\tDeceleration: " + str(movementProfile['deceleration'].value)
        self.LogInfo(msg)
    