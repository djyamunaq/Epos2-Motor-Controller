#include "solver.h"
#include <iostream>
#include <cmath>

using namespace std;

PlanarRobot::PlanarRobot(double &aIn){
    a = aIn;
    k = a/2/pi;
    phi_0_c = 0.6/k;
    q0 = phi_0_c;
}

double PlanarRobot::Lenght(int &qIn){
    double q1_rad = q0 + (2*pi*qIn / 7400.0);
    return a/2/pi * (q1_rad/2 * sqrt( pow(q1_rad,2) + 1) + 0.5 * log(q1_rad +  sqrt(pow(q1_rad,2)+1)) - q0/2 *  sqrt(pow(q0,2)+1) - 0.5 * log(q0 +  sqrt( pow(q0, 2)+1)));
}

double PlanarRobot::F(double &xIn, double &targetIn){
    return pow(a*(-phi_0_c*sqrt(pow(phi_0_c,2) + 1)/2 + (phi_0_c/2 + xIn/2)*sqrt(pow(phi_0_c + xIn,2) + 1) - 0.5*log(phi_0_c + sqrt(pow(phi_0_c,2) + 1)) + 0.5*log(phi_0_c + xIn + sqrt(pow(phi_0_c + xIn,2) + 1)))/(2*pi) - targetIn,2);
}

double PlanarRobot::GradF(double &xIn, double &targetIn){
    //return 113.716340724486*(0.112530385849425*(0.00079746326444206*xIn + 0.028239392069272)*(xIn/2 + 17.7057635934048)/sqrt(pow(0.028239392069272*xIn + 1,2) + 0.00079746326444206) + 0.00158889484285418*(35.4115271868096*(0.00079746326444206*xIn + 0.028239392069272)/sqrt((0.028239392069272*xIn + 1,2) + 0.00079746326444206) + 1)/(xIn + 35.4115271868096*sqrt(pow(0.028239392069272*xIn + 1,2) + 0.00079746326444206) + 35.4115271868096) + 0.0562651929247125*sqrt(pow(0.028239392069272*xIn + 1,2) + 0.00079746326444206))*(-0.0937753215411875*targetIn + 0.0562651929247125*(xIn/2 + 17.7057635934048)*sqrt(pow(0.028239392069272*xIn + 1,2) + 0.00079746326444206) + 0.000794447421427092*log(xIn + 35.4115271868096*sqrt(pow(0.028239392069272*xIn + 1,2) + 0.00079746326444206) + 35.4115271868096) - 1);

    return a*(a*(-phi_0_c*sqrt(pow(phi_0_c,2) + 1)/2 + (phi_0_c/2 + xIn/2)*sqrt(pow(phi_0_c + xIn,2) + 1) - 0.5*log(phi_0_c + sqrt(pow(phi_0_c,2) + 1)) + 0.5*log(phi_0_c + xIn + sqrt(pow(phi_0_c + xIn,2) + 1)))/(2*pi) - targetIn)*((phi_0_c/2 + xIn/2)*(phi_0_c + xIn)/sqrt(pow(phi_0_c + xIn,2) + 1) + 0.5*((phi_0_c + xIn)/sqrt(pow(phi_0_c + xIn,2) + 1) + 1)/(phi_0_c + xIn + sqrt(pow(phi_0_c + xIn,2) + 1)) + sqrt(pow(phi_0_c + xIn,2) + 1)/2)/pi;
}

void PlanarRobot::Solver(double &targetIn, int &qOut){
    double d0;
    double X1;
    for ( int i = 0; i < Niter; i++)
    {
        d0 = -GradF(X1, targetIn);
        X1 = X1 + tk*d0;
        if (F(X1, targetIn) < tol) {break;}
    }   
    qOut = int(7400 * (X1/2/pi));
}
