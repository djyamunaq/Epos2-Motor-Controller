#include <stdio.h>
#include "cmath"

using namespace std;

class PlanarRobot{
public:
//! \brief constructo, initialization
//! \param a rope size
    PlanarRobot(double &aIn);

//! \brief return L(q_motor)
    double Lenght(int &qIn);

//! \brief return F = (L-L_target)^2
    double F(double &xIn, double &targetIn);

//! \brief return dF
    double GradF(double &xIn, double &targetIn);
    
//! \brief solve with Optimization q(L_target)
    void Solver(double &targetIn, int &qOut);

private:
    double a;           //rope size
    double k;           //parameter 1
    double phi_0_c;     //start angle
    double q0 ;         //joint motor init
    double X0  = 0.0;   //start computation
    double tol = 1e-3;  //tolerence
    double tk  = 10e-2; //step
    double pi = 3.1416; //pi
    int Niter = 100;    //Iteration
};