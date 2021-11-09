from spylizard import *

conductor = 1; shield = 2; air = 3; contour = 4;

# Load the mesh:
mymesh = mesh("magmesh.msh")

# Define the whole volume region for convenience:
wholedomain = selectunion([conductor,shield,air])

# Define the magnetic permeability mu [H/m] in the air, conductor (aluminium) and magnetic shield (steel):
mu0 = 4*getpi()*1e-7

mu = parameter()

mu.setvalue(air, mu0)
mu.setvalue(conductor, mu0)
mu.setvalue(shield, 1000*mu0)

# Define a spanning tree to gauge the magnetic vector potential (otherwise the matrix is singular).
# Start growing the tree from the regions with constrained potential vector (here the contour): 
spantree = spanningtree([contour])
# Write it for illustration:
spantree.write("spantree.pos")

# Define the magnetic vector potential 'a' and provide the tree. Use edge shape functions 'hcurl'.
a = field("hcurl", spantree)

# Gauge the vector potential field on the whole volume:
a.setgauge(wholedomain)

# Use higher interpolation orders where needed:
a.setorder(shield, 2)
a.setorder(conductor, 1)
a.setorder(air, 1)

# Put a magnetic wall (i.e. set field 'a' to 0) all around the domain (no magnetic flux can cross it):
a.setconstraint(contour)

# Define the magnetostatic formulation:
magnetostatics = formulation()

# The strong form of the magnetostatic formulation is curl( 1/mu * curl(a) ) = j, with b = curl(a):
magnetostatics += integral(wholedomain, 1/mu* curl(dof(a)) * curl(tf(a)) )
# A current density of 1A/m^2 flows in the z direction in the conductor region:
magnetostatics += integral(conductor, -array3x1(0,0,1)*tf(a))

# Generate the algebraic matrices A and vector b of the Ax = b problem:
magnetostatics.generate()

# Get the x solution vector:
sol = solve(magnetostatics.A(), magnetostatics.b())

# Update field 'a' with the solution that has just been computed:
a.setdata(wholedomain, sol)

# Write the magnetic induction field b = curl(a) [T]:
curl(a).write(wholedomain, "b.pos", 1)

# Code validation line. Can be removed:
print((norm(a).max(wholedomain,4)[0] < 1.96437e-08) and (norm(a).max(wholedomain,4)[0] > 1.96435e-08))

