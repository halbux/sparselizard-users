from spylizard import *

coil = 1; tube = 2; air = 3; coilskin = 4; tubeskin = 5; vin = 6; vout = 7; domainboundary = 8;

# Load the mesh file:
mymesh = mesh("inductionheating.msh")

# Define extra regions for convenience:
conductor = selectunion([coil,tube])
wholedomain = selectall()
domainboundary = selectunion([domainboundary,vin,vout])

mu = parameter(); sigma = parameter();
# Define the magnetic permeability mu [H/m] everywhere (the materials here are not magnetic):
mu.setvalue(wholedomain, 4*getpi()*1e-7)
# Conductivity of the copper coil and aluminium tube [S/m]:
sigma.setvalue(coil, 6e7)
sigma.setvalue(tube, 3e7)

# Set the working frequency to 50 Hz:
setfundamentalfrequency(50)

# Define a spanning tree to gauge the magnetic vector potential (otherwise the matrix to invert is singular).
# Start growing the tree from the regions with constrained potential vector (here the domain boundary): 
spantree = spanningtree([domainboundary])

# Use nodal shape functions 'h1' for the electric scalar potential 'v'.
# Use edge shape functions 'hcurl' for the magnetic vector potential 'a'.
# A spanning tree has to be provided to field 'a' for gauging.
# Since the solution has a component in phase with the electric actuation
# and a quadrature component we need 2 harmonics at 50Hz 
# (harmonic 1 is DC, 2 is sine at 50Hz and 3 cosine at 50Hz).
a = field("hcurl", [2,3], spantree); v = field("h1", [2,3]);

# Gauge the vector potential field on the whole volume:
a.setgauge(wholedomain)

# Select adapted interpolation orders for field a and v:
a.setorder(wholedomain, 0)
v.setorder(wholedomain, 1)

# Put a magnetic wall (i.e. set field a to 0) all around the domain (no magnetic flux can cross it):
a.setconstraint(domainboundary)
# Also ground v on face 'vout':
v.setconstraint(vout)
# Set v to 1V on face 'vin' for the in-phase component and to 0 for the quadrature component:
v.setconstraint(vin, [1,0])

# Define the weak magnetodynamic formulation:
magdyn = formulation()

# The strong form of the magnetodynamic a-v formulation is 
# 
# curl( 1/mu * curl(a) ) + sigma * (dt(a) + grad(v)) = js, with b = curl(a) and e = -dt(a) - grad(v)
#
# Magnetic equation:
magdyn += integral(wholedomain, 1/mu* curl(dof(a)) * curl(tf(a)) )
magdyn += integral(conductor, sigma*dt(dof(a))*tf(a) + sigma* grad(dof(v))*tf(a) )
# Electric equation:
magdyn += integral(conductor, sigma*grad(dof(v))*grad(tf(v)) + sigma*dt(dof(a))*grad(tf(v)) )

# Generate, solve and transfer the solution to fields a and v:
magdyn.solve()

# Write the magnetic induction field b = curl(a) [T], electric field e = -dt(a) - grad(v) [V/m] and current density j [A/m^2]:
e = -dt(a) - grad(v)

curl(a).write(wholedomain, "b.pos", 1)
e.write(conductor, "e.pos", 1)
(sigma*e).write(conductor, "j.pos", 1)
v.write(conductor, "v.pos", 1)

# Code validation line. Can be removed:
minj = norm(sigma*(-2*getpi()*50*a.harmonic(2) - grad(v.harmonic(3)))).min(tube,4)[0]
print((minj < 216432) and (minj > 216430))

