from spylizard import *

# The physical region numbers as defined in the mesh file:
tube = 1; air = 2; domainskin = 3;

setaxisymmetry()

mymesh = mesh("tubeinair.msh")

# Define the whole domain for convenience:
wholedomain = selectunion([tube, air])

# Magnetic permeability of the air:
mu = 4.0*getpi()*1e-7

# In this axisymmetric problem the magnetic vector potential can
# be expressed as a vector with only a non-zero z component:
az = field("h1"); x = field("x");

# Use interpolation order 1:
az.setorder(wholedomain, 1)

a = array3x1(0,0,az)

# Put a magnetic wall (i.e. set field a to 0) all 
# around the domain (no magnetic flux can cross it):
az.setconstraint(domainskin)

# The magnetic vector potential a is rewritten as the sum of a known
# source term and an unknown part to be determined: atotal = a + asource.
# We want to apply an external induction field b of 1 mT in 
# the y direction that linearly increases over time:
bsource = array3x1(0,1e-3*t(),0)
# Since b = curl(a) (in cylindrical coordinates) a valid choice for a is:
asource = array3x1(0,0,-1e-3*0.5*t()*x)
# And dt(a)/dt is:
dtasource = array3x1(0,0,-1e-3*0.5*x)


# The strong form of the magnetodynamic a-v formulation is 
# 
# curl( 1/mu * curl(a) ) + sigma * (dt(a) + grad(v)) = js, with b = curl(a) and e = -dt(a) - grad(v)
#
# Here grad(v) is zero because of axisymmetry and y direction bsource and the electric field becomes:
#
e = -dt(a)-dtasource

# The conductivity of the high temperature superconductor is modeled using 
# a power law relation between the electric field and the current density:
#
# J = Jc/Ec^(1/n) * norm(E)^((1-n)/n) * E
#    |_______________________________|
#                sigma(E)
#
# where the critical current density Jc [A/m^2] and critical electric field Ec [V/m] are supposed 
# constant here and the J(E) relation is isotropic. An extra 1e-11 is added to never divide by 0.
#
n = 30.0; Ec = 1e-4; Jc = 1e8;

sigma = Jc/( pow(Ec,1.0/n) ) * pow( norm(e) + 1e-11, (1.0-n)/n )


# Define the weak magnetodynamic formulation:
magdyn = formulation()

magdyn += integral(wholedomain, 1/mu*( curl(dof(a)) + bsource ) * curl(tf(a)) )
magdyn += integral(tube, sigma*( dt(dof(a)) + dtasource )*tf(a) )


# Start the implicit Euler time resolution with the field values and a zero time derivative:
eul = impliciteuler(magdyn, vec(magdyn), 0)

# Tolerance on the nonlinear iteration:
eul.settolerance(1e-4)

# Use a 1 second timestep and run until 150 sec (150 timesteps):
numsteps = 150
timestep = 1.0
bcenter = []

settime(0)
for i in range(0, numsteps):

    # Use a fixed-point nonlinear iteration at every timestep (unlimited number of nl iterations with -1).
    numnlits = eul.next(timestep, -1)
    
    # Write a and b = curl(a) to disk:
    norm(a).write(wholedomain, "norma" + str(i) + ".vtu", 1)
    norm(curl(a) + bsource).write(wholedomain, "normbtotal" + str(i) + ".vtu", 1)
    
    # Output the b induction field [T] at the tube center to assess the shielding effectiveness.
    # Interpolate at a x coordinate slightly away from 0 to avoid NaN issues:
    bcenter.append(norm(curl(a) + bsource).interpolate(wholedomain, [1e-10,0,0])[0])
    print("b source", timestep*(i+1), "mT --> b tube center", bcenter[i]*1e3, "mT (", numnlits, "NL its )")

# Combine all timesteps in a ParaView .pvd file for convenience:
grouptimesteps("normbtotal.pvd", "normbtotal", 0, eul.gettimes())

# Write to file the field at the tube center for all timesteps:
writevector("bcenter.csv", bcenter)

# Code validation line. Can be removed:
print((bcenter[numsteps-1] < 0.03746) and (bcenter[numsteps-1] > 0.03744))

