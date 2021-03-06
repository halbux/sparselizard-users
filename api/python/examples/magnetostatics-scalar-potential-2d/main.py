from spylizard import *

magnet1 = 1; magnet2 = 2; magnet3 = 3; magnet4 = 4; magnet5 = 5; steel = 6; air = 7; zeropotential = 8;

mymesh = mesh("halbacharray.msh")

# Define new physical regions for convenience:
magnets = selectunion([magnet1, magnet2, magnet3, magnet4, magnet5])
wholedomain = selectunion([magnets, steel, air])

# Nodal shape functions 'h1' for the magnetic scalar potential 'phi'.
phi = field("h1")

# Use interpolation order 2 on the whole domain:
phi.setorder(wholedomain, 2)

# The magnetic scalar potential (just like the electric potential)
# needs to be fixed at least at one node to have a well-posed problem.
# Here it is forced to 0 at a permanent magnet corner point.
phi.setconstraint(zeropotential)

# Vacuum magnetic permeability [H/m]: 
mu0 = 4*getpi()*1e-7

# Define the permeability in all regions:
mu = parameter()

mu.setvalue(air, mu0)
mu.setvalue(steel, 1000*mu0)
mu.setvalue(magnets, mu0)


magnetostatic = formulation()

# In the absence of current sources the static Maxwell equations give:
# 
# curl h = 0
# 
# One can thus define a magnetic scalar potential 'phi' such that h = -grad(phi).
#
# Considering also that div(b) = 0 we get
#
# div(mu*(-grad(phi))) = 0
#
# with b = mu * h.
#
# In the permanent magnet region b = mu0 * (h + m),
# i.e. the material is non-magnetic but it is pre-magnetized by the magnetization vector m [A/m].
# We thus get:
# div(mu0*(-grad(phi)) + mu0*m) = 0
# 
# A permanent magnet with a [0, 800e3] A/m magnetization is considered.

# The weak form corresponding to the above equations:
magnetostatic += integral(wholedomain, -grad(dof(phi)) * mu * grad(tf(phi)) )

# This is when all magnets are oriented in the y direction:
magnetostatic += integral(magnets, array2x1(0, 800e3) * mu * grad(tf(phi)) )
# This is in Halbach configuration (to maximise the magnetic field above the array):
#magnetostatic += integral(magnet1, array2x1(-800e3, 0) * mu * grad(tf(phi)) )
#magnetostatic += integral(magnet2, array2x1(0, -800e3) * mu * grad(tf(phi)) )
#magnetostatic += integral(magnet3, array2x1(800e3, 0) * mu * grad(tf(phi)) )
#magnetostatic += integral(magnet4, array2x1(0, 800e3) * mu * grad(tf(phi)) )
#magnetostatic += integral(magnet5, array2x1(-800e3, 0) * mu * grad(tf(phi)) )


magnetostatic.generate()

# Solve the algebraic system Ax = b:
sol = solve(magnetostatic.A(), magnetostatic.b())

# Transfer the data from the solution vector to the phi field:
phi.setdata(wholedomain, sol)

# Write the magnetic scalar potential and the magnetic field with an order 2 interpolation.
phi.write(wholedomain, "phi.pos", 2)
norm(-grad(phi)).write(wholedomain, "hnorm.pos", 2)
(-grad(phi)).write(wholedomain, "h.pos", 2)

# Evaluate the magnetic field 1.5cm above the center of the magnet array:
magfieldnorm = norm(-grad(phi)).interpolate(wholedomain, [0,0.02,0])
print('Magnetic field 1.5cm above the array center:', magfieldnorm[0], 'A/m')

# Write 20 magnetic field lines starting on the top side of the magnet array:
ln = shape("line", -1, [-0.025,0.01,0, 0.025,0.01,0], 20)
(-grad(phi)).streamline(selectunion([air, steel]), "magneticfieldline.pos", ln.getcoords(), 0.01/5)


# Calculate and print the total force on the steel disk:
totalforce = gettotalforce(steel, -grad(phi), mu)
printtotalforce(steel, -grad(phi), mu)


# Code validation line. Can be removed.
print((magfieldnorm[0] < 64963.8) and (magfieldnorm[0] > 64963.6) and (totalforce[0] < -58.3914) and (totalforce[0] > -58.3918))

