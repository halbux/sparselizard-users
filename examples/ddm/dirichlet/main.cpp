#include "sparselizard.h"

using namespace sl;

// Usual disk meca problem solved with DDM with Dirichlet interface conditions. Mixed conditions also ready but doesn't fit in a 10 line code.
//
// THIS WILL RUN SLOWER ON YOUR LAPTOP THAN WITHOUT DDM IF YOU ALLOW (DEFAULT) EACH PROCESS TO TAKE THE MAX NUM THREADS ALLOWED! THAT'S NORMAL!
// REFINE THE MESH AND ALLOW MAX 1 THREAD PER PROCESS OR RUN ON A SUPERCOMPUTER TO SEE THE SPEEDUP.
//
// For a larger number of processes the convergence slows down, this will be fixed with a coarse grid preconditionner in the future (you are welcome to contribute!).

int main(void)
{	
    int skin = 5;

    slmpi::initialize();
    
    int rank = slmpi::getrank();

    // Just to create the global geometry skin (skip if already defined):
    if (rank == 0)
    {
        mesh initstep;
        initstep.selectskin(skin);
        initstep.load("disk.msh");
        initstep.write("diskwithskin.msh");
    }
    slmpi::barrier();
    //
    
    std::string partname = allpartition("diskwithskin.msh");
        
    int vol = 1, sur = 2, top = 3;
    
    mesh mymesh("gmsh:"+partname, skin, 1, (rank==0));
    
    field u("h1xyz");

    u.setorder(vol, 2);
    u.setconstraint(sur);
  
    formulation elasticity;

    elasticity += integral(vol, predefinedelasticity(dof(u), tf(u), 150e9, 0.3));
    elasticity += integral(vol, array3x1(0,0,-10)*tf(u));

    elasticity.allsolve(1e-8, 500);
    
    (0.5e9*u).write(vol, "u"+std::to_string(rank+1000)+".vtk", 2);
    
    slmpi::finalize();
}

