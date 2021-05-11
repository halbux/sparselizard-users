// Clamped-clamped membrane with geometric nonlinearity: frequency sweep around the resonance peak using multiharmonic (aka. harmonic balance) FEM
//
// Example kindly provided and paper-validated by S. Saxena (paper is "PARALLEL HARMONIC BALANCE METHOD FOR ANALYSIS OF NONLINEAR DYNAMICAL SYSTEMS")

#define _USE_MATH_DEFINES

#include "sparselizard.h"
#include <cmath>

using namespace sl;

int main(void)
{	
    // The domain regions as defined in 'disk.geo':
    int vol = 1, sur = 2, loadpoint = 3;

    // User input filename and frequency sweep stepsize:
    double step, startfreq, endfreq;
    std::string filename;

    std::cout << "Enter msh file name to read                                   : ";
    std::cin >> filename;
    std::cout << "Enter starting frequency for sweep in rad/s                   : ";
    std::cin >> startfreq;
    std::cout << "Enter ending frequency for sweep in rad/s                     : ";
    std::cin >> endfreq;
    std::cout << "Enter step size for frequency sweep in rad/s                  : ";
    std::cin >> step;
    

    // Frequency sweep:
    double fd_rad   = startfreq;
    double fd       = fd_rad/(2*M_PI);
    
    // Mesh is exported from GMSH with format 2 ASCII:
    mesh mymesh(filename, 1);

    // Set the drive frequency for analysis:
    setfundamentalfrequency(fd);
    
    // Nodal shape functions 'h1' with 3 components and 3 harmonic terms (2*3 +1 coefficients)
    // Field u is the displacement of the beam.
    field u("h1xyz", {1,2,3,4,5});

    // Use interpolation order 2 on 'vol', the whole domain:
    u.setorder(vol, 2);
    
    // Clamp on surface 'sur' (i.e. 0 valued-Dirichlet conditions):
    u.setconstraint(sur);
  
    // E is Young's modulus. nu is Poisson's ratio. 
    parameter E, nu, rho;
    E|vol = 210e9; nu|vol = 0.3; rho|vol = 7800;

    // Mass damping only, C = alpha*M
    double alpha = 3.0;
  
    formulation elasticity;

    // The elasticity formulation with geometric nonlinearity:
    elasticity += integral(vol, 5,              predefinedelasticity(dof(u), tf(u), u, E, nu, 0.0));
    // Add a point force in the -z direction:
    elasticity += integral(loadpoint,           array1x3(0,0,-200)*tf(u.harmonic(2)));
    // Add inertia term:
    elasticity += integral(vol,                -rho*dtdt(dof(u))*tf(u));
    // Add the damping term:
    elasticity += integral(vol,                -alpha*rho*dt(dof(u))*tf(u));

    int count = 1;
    double f1, f2;
    if (startfreq < endfreq){
        f1 = fd_rad;
        f2 = endfreq;
    }
    else{
        f1 = endfreq;
        f2 = fd_rad;
    }
    
    while (f1 < f2)
    {
        setfundamentalfrequency(fd);

        std::cout << "Percentage done                                               : " << std::abs((fd_rad - startfreq)/(endfreq - startfreq))*100 << "%" << std::endl;
        std::cout << "Solving for drive frequency                                   : " << fd_rad << std::endl;

        double umax = 0, relchange = 1.0;
        while (relchange > 1e-6)
        {
            // Generate, solve and transfer the solution to field u:
            std::cout << "Solving formulation elasticity for total iteration count  : " << count << std::endl;
            elasticity.solve();

            double um = norm(u.harmonic(2)).max(vol,5)[0];
            relchange = std::abs(um - umax)/um;
            umax = um;

            std::cout<<"Max U2 for the system is : " << um << " and relative change is : " << relchange << std::endl;
        }

        // Write the deflection to ParaView .vtk format.
        u.write(vol, "u_f"+std::to_string(fd_rad)+".vtk", 2, 30);
        u.write(vol, "u_h"+std::to_string(fd_rad)+".vtk", 2);

        fd_rad += step;
        fd      = fd_rad/(2*M_PI);
        count  += 1;
        
        if (startfreq < endfreq){
            f1 = fd_rad;
        }
        else{
            f2 = fd_rad;
        }        
    }
    
}

