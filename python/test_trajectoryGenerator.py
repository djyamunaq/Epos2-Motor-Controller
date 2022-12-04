import matplotlib.pyplot as plt
import numpy as np
import time
from trajectoryGenerator import TrajectoryGenerator
from MotorController import *
from math import cos, sin, pi

# ===================================================================
# 

def main():
    # ===================================================================
    # Motor configuration
    motorController = MotorController()
    motorController.connect("EPOS2", "MAXON SERIAL V2", "USB", "USB0", 1000000)
    nodeId = 1
    turnVal = 7400

    # ===================================================================
    # Generate Trajectory
        # Initial and final positions of End-Effector frame origin
    pis = np.array([0, 20]) 
    pfs = np.array([5, 3])
        # Time range and initial time of movement
    Dt = 2
    ti = 1

    trajectory = TrajectoryGenerator(pis, pfs, Dt, ti)

    # ===================================================================
    # Motor configuration
        # Motor Initial Position in degrees
    motor_theta = 0
        # Motor radius
    motor_radius = 1

    # ===================================================================
    # Pulley configuration
        # Pulley radius
    pulley_r = 1
        # Pulley position
    pulley_pos = np.array([15, 15])
    
    # ===================================================================
    # Wire configuration
        # Wire radius
    wire_r = 1

    # ===================================================================
    # End-Effector configuration
        # EE side size
    ee_s = 2
        # EE top-left corner position
    ee_pos = np.array([-ee_s/2, ee_s/2, 1])
        # EE orientation
    ee_theta = 0

    # ===================================================================
    # Movement
        # Time step
    dt = 1e-3
        # Initial time
    t = 0
        # Movement loop
    while t < Dt:
            # Current position
        X = trajectory.X(t)
            # Desired position
        Xd = trajectory.X(t+dt)
            
            # Transformation Matrix (EE Local frame to World Frame)
        T = np.array([
            [cos(ee_theta), -sin(ee_theta), X[0]],
            [sin(ee_theta), cos(ee_theta), X[1]],
            [0, 0, 1]
        ])
        Td = np.array([
            [cos(ee_theta), -sin(ee_theta), Xd[0]],
            [sin(ee_theta), cos(ee_theta), Xd[1]],
            [0, 0, 1]
        ])
            # Calculate positions in world frame 
        X_world = np.dot(T, ee_pos)
        Xd_world = np.dot(Td, ee_pos)

        wire_len = np.linalg.norm(pulley_pos - X_world[0:2])
        wire_len_d = np.linalg.norm(pulley_pos - Xd_world[0:2])

            # Calculate variation in wire length
        Dwire_len = wire_len_d - wire_len

        # print('Dwire_len:', Dwire_len)

            # Calculate motor angular variation
        Dth = Dwire_len/(2*pi*motor_radius)
        print('Dth:', Dth)
            # Get current pos
        pos = motorController.getPos(nodeId)
        pos = pos + turnVal*Dth
        print('Pos:', pos)
        
        motorController.setPos(nodeId, pos)

            # Update time
        t += dt


if __name__=='__main__':
    main()