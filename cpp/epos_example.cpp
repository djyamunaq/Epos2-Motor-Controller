#include <iostream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "MotorController.h"
#include "TrajectoryGenerator.h"
#include <eigen3/Eigen/Dense>
#include <sys/time.h>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::vector;

int main(int argc, char** argv) {
    /* =================================================================== */
    /* Motor configuration*/

    unsigned int errorCode;
    string deviceName = "EPOS2";
    string protocolName = MAXON_SERIAL_V2;
    string interfaceName = "USB";
    string portName = "USB0";
	unsigned int baudrate = 1e6; 

    epos::MotorController mc; 
    unsigned int nodeId = 1;
    mc.connect(deviceName, protocolName, interfaceName, portName, baudrate);
    mc.activatePositionMode(nodeId);

    /* =================================================================== */
    /* Generate Trajectory */
        // Initial and final positions of End-Effector frame origin
    Eigen::Vector2d pis = {0, 0}; 
    Eigen::Vector2d pfs = {10, 10};

        // Time range and initial time of movement
    double Dt = 1;
    double ti = 1;

    TrajectoryGenerator trajectory = TrajectoryGenerator(pis, pfs, Dt, ti);

    /* =================================================================== */ 
    /* Motor configuration */
        // Motor Initial Position in degrees
    double motor_theta = 0;
        // Motor radius
    double motor_radius = 1;

    /* =================================================================== */ 
    /* Pulley configuration */
        // Pulley radius
    double pulley_r = 1;
        // Pulley position
    Eigen::Vector2d pulley_pos = {15, 15};
    
    /* =================================================================== */ 
    /* Wire configuration */
        // Wire radius
    double wire_r = 1;

    /* =================================================================== */ 
    /* End-Effector configuration */
        // EE side size
    double ee_s = 2;
        // EE top-left corner position
    Eigen::Vector3d ee_pos = {-ee_s/2, ee_s/2, 1};
        // EE orientation
    double ee_theta = 0;

    /* =================================================================== */ 
    /* Movement */
        // Time step
    double dt = 1e-3;
        // Initial time
    double t = 0;

    vector<unsigned long int> times;

        // Movement loop
    while(t < Dt) {
            // Current position
        Eigen::Vector2d X = trajectory.X(t);
            // Desired position
        Eigen::Vector2d Xd = trajectory.X(t+dt);
            
            // Transformation Matrix (EE Local frame to World Frame)
        Eigen::Matrix<double, 3, 3> T;
        T << cos(ee_theta), -sin(ee_theta), X[0],
             sin(ee_theta), cos(ee_theta), X[1],
             0, 0, 1;
        Eigen::Matrix<double, 3, 3> Td;
        Td << cos(ee_theta), -sin(ee_theta), Xd[0],
             sin(ee_theta), cos(ee_theta), Xd[1],
             0, 0, 1;

            // Calculate positions in world frame 
        Eigen::Vector3d X_world = T*ee_pos;
        Eigen::Vector3d Xd_world = Td*ee_pos;

        double wire_len = (pulley_pos - X_world.block<2, 1>(0, 0)).norm();
        double wire_len_d = (pulley_pos - Xd_world.block<2, 1>(0, 0)).norm();

            // Calculate variation in wire length
        double Dwire_len = wire_len_d - wire_len;

            // Calculate motor angular variation
        double Dth = Dwire_len/(2*M_PI*motor_radius);

            // Get current pos
        double pos = mc.getPos(nodeId);
        pos = pos + 7400*Dth;

        unsigned long int sec= time(NULL);
        times.push_back(sec);
        
        mc.setPos(nodeId, pos);

        cout << pos << endl;

            // Update time
        t += dt;
    }

    double avg = 0;
    for(int i=1; i<times.size(); i++) {
        unsigned long int t_prev = times[i-1];
        unsigned long int t_curr = times[i];

        avg += (t_curr - t_prev);
    }
    
    avg /= times.size();

    cout << "Avg: " << avg;

    return 0;
}