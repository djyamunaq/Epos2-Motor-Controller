#include "solver.h"
#include <iostream>
#include <cmath>

using namespace std;

int main(){
    double coeff = 0.5323;
    double a = 0.2*coeff;

    PlanarRobot MyRobot (a);

    double LOut;
    double L_target = 0.00;
    int Motor_Joint;
    
    
    for (int  i = 1; i < 10; i++)
    {
        MyRobot.Solver(L_target, Motor_Joint);
        cout<<"\nJoint position according L_target = "<<L_target<< " is : " << Motor_Joint << endl;
        cout<<"L according Motor_joint = "<<Motor_Joint<< " is : " << MyRobot.Lenght(Motor_Joint) << "\n";
        L_target = 10*i;
    }
}