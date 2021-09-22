#include "sparselizard.h"

using namespace sl;

int main(void)
{	
    // The domain regions as defined in 'disk.geo':
    int zeropotential = 1, material = 2, air = 3;
    
    // The mesh can be curved!
    mesh mymesh("disk.msh");

    int wholedomain = selectunion({material, air});

    field phi("h1");

    // Use interpolation order 1 on the whole domain:
    phi.setorder(wholedomain, 1);

    // Force the potential to 0 on the edge:
    phi.setconstraint(zeropotential);

    // constant magnetization
    expression magnetization = array2x1(0,1);
    
    formulation demag;

    demag += integral(wholedomain, grad(dof(phi))*grad(tf(phi)));
    demag += integral(material, -magnetization*grad(tf(phi)));

    demag.generate();

    // Solve the algebraic system Ax = b:
    vec sol = solve(demag.A(), demag.b());

    // Transfer the data from the solution vector to the phi field:
    phi.setdata(wholedomain, sol);

    // Write the magnetic scalar potential and the magnetic field with an order 2 interpolation.
    phi.write(wholedomain, "phi.pos", 2);
    norm(-grad(phi)).write(wholedomain, "hnorm.pos", 2);
    (-grad(phi)).write(wholedomain, "h.pos", 2);
    (magnetization-grad(phi)).write(material, "b.pos", 2);
    magnetization.write(material,"magnetization.pos",2);

}

