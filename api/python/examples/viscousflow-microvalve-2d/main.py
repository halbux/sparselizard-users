from spylizard import *

def createmesh():

    # Give names to the physical region numbers:
    support = 1; fluid = 2; pillar = 3; membrane = 4; inlet = 7; outlet = 8;

    # Define the x and y geometrical dimensions:
    y1 = 15e-6; y2 = 25e-6; y3 = 30e-6;
    x1 = -100e-6; x2 = -50e-6; x3 = -25e-6; x4 = 25e-6; x5 = 50e-6; x6 = 100e-6;
    
    # Define the mesh finesse:
    n = 10
    
    q1 = shape("quadrangle", support, [x1,0,0, x2,0,0, x2,y1,0, x1,y1,0], [n,n,n,n])
    q2 = shape("quadrangle", support, [x5,0,0, x6,0,0, x6,y1,0, x5,y1,0], [n,n,n,n])
    
    q3 = shape("quadrangle", fluid, [x2,0,0, x3,0,0, x3,y1,0, x2,y1,0], [n,n,n,n])
    q4 = shape("quadrangle", fluid, [x4,0,0, x5,0,0, x5,y1,0, x4,y1,0], [n,n,n,n])
    
    q5 = shape("quadrangle", pillar, [x3,0,0, x4,0,0, x4,y1,0, x3,y1,0], [n,n,n,n])
    
    q6 = shape("quadrangle", support, [x1,y1,0, x2,y1,0, x2,y2,0, x1,y2,0], [n,n,n,n])
    q7 = shape("quadrangle", support, [x5,y1,0, x6,y1,0, x6,y2,0, x5,y2,0], [n,n,n,n])
    
    q8 = shape("quadrangle", fluid, [x2,y1,0, x3,y1,0, x3,y2,0, x2,y2,0], [n,n,n,n])
    q9 = shape("quadrangle", fluid, [x4,y1,0, x5,y1,0, x5,y2,0, x4,y2,0], [n,n,n,n])
    q10 = shape("quadrangle", fluid, [x3,y1,0, x4,y1,0, x4,y2,0, x3,y2,0], [n,n,n,n])
    
    q11 = shape("quadrangle", membrane, [x1,y2,0, x2,y2,0, x2,y3,0, x1,y3,0], [n,n,n,n])
    q12 = shape("quadrangle", membrane, [x5,y2,0, x6,y2,0, x6,y3,0, x5,y3,0], [n,n,n,n])
    q13 = shape("quadrangle", membrane, [x2,y2,0, x3,y2,0, x3,y3,0, x2,y3,0], [n,n,n,n])
    q14 = shape("quadrangle", membrane, [x4,y2,0, x5,y2,0, x5,y3,0, x4,y3,0], [n,n,n,n])
    q15 = shape("quadrangle", membrane, [x3,y2,0, x4,y2,0, x4,y3,0, x3,y3,0], [n,n,n,n])
    
    # Get the inlet and outlet lines:
    l1 = q3.getsons()[0]
    l2 = q4.getsons()[0]
    
    l1.setphysicalregion(inlet)
    l2.setphysicalregion(outlet)
    
    # Provide to the mesh all shapes of interest:
    mymesh = mesh([q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14,q15,l1,l2])
    
    return mymesh


# Define the physical regions that will be used:
support = 1; fluid = 2; pillar = 3; membrane = 4; inlet = 7; outlet = 8;

# Create the geometry and the mesh:    
mymesh = createmesh()

# Write the mesh for display:
mymesh.write("microvalve.msh")

# Define the fluid boundary as the intersection between the solid and the fluid regions:
solid = selectunion([pillar,support,membrane])
fluidboundary = selectintersection([fluid,solid])

# Dynamic viscosity of water [Pa.s] and density [kg/m3]:
mu = 8.9e-4; rho = 1000;

# Field v is the flow velocity. It uses nodal shape functions "h1" with two components.
# Field p is the relative pressure.
v = field("h1xy"); p = field("h1");

# Force the flow velocity to 0 at the solid interface:
v.setconstraint(fluidboundary)

# Set a relative pressure of 100 Pa at the valve inlet and 0 at the outlet:
p.setconstraint(inlet, 100)
p.setconstraint(outlet)

# Use an order 1 interpolation for p and 2 for v on the fluid region:
p.setorder(fluid, 1)
v.setorder(fluid, 2)

# Define the weak formulation of the Stokes flow problem.
# The strong form can be found at https:#en.wikipedia.org/wiki/Stokes_flow
viscousflow = formulation()

viscousflow += integral(fluid, predefinedstokes(dof(v), tf(v), dof(p), tf(p), mu, rho, 0, 0) )

# Generate, solve and save:
viscousflow.solve()

# Write the p and v fields to file:
p.write(fluid, "p.vtk", 1)
v.write(fluid, "v.vtk", 2)

# Output the flowrate for a unit width:
flowrate = (-normal(fluid)*v).integrate(inlet, 4)
print("\nFlowrate for a unit width:", flowrate, "m^3/s")

# Code validation line. Can be removed.
print((flowrate < 1.4845e-7) and (flowrate > 1.4844e-7))

