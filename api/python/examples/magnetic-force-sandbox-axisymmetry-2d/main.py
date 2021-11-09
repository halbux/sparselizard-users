from spylizard import *

setaxisymmetry()

# Line region 'bnd' is defined below to perform a line integral of the Maxwell stress tensor.
bar = 1; ring = 2; air = 3; clamp = 4; sides = 5; bnd = 6;

mymesh = mesh()
# Grow 5 element layers from the bar region:
mymesh.selectlayer(7, air, bar, 5)
# Take the layer skin:
mymesh.selectskin(8, 7)
# Take the bar skin:
mymesh.selectskin(9, bar)
# Remove the bar skin from the layer skin:
mymesh.selectexclusion(bnd, 8, [9])
# Refine by splitting once:
mymesh.split(1)
# Load the mesh:
mymesh.load("ring2d.msh")

all = selectall()

# Visualize the Maxwell stress tensor line integration region:
expression(1).write(bnd, "bnd.vtu", 1)

# Nodal shape functions 'h1' for the z component of the vector potential.
az = field("h1")

# In 2D axisymmetry the vector potential only has a z component:
a = array3x1(0,0,az)

# Use interpolation order 4:
az.setorder(all, 4)

# Put a magnetic wall at the outer air sides:
az.setconstraint(sides)

# Vacuum magnetic permeability [H/m]: 
mu0 = 4.0*getpi()*1e-7

# Define the permeability in all regions.
mu = parameter()

mu.setvalue(all, mu0)
# Overwrite on the magnetic bar region:
mu.setvalue(bar, 1e5*mu0)


magnetostatics = formulation()

# The strong form of the magnetostatic formulation is curl( 1/mu * curl(a) ) = j, with b = curl(a):
magnetostatics += integral(all, 1/mu* curl(dof(a)) * curl(tf(a)) )
# External current density source:
magnetostatics += integral(ring, -array3x1(0,0,1e5) * tf(a))

magnetostatics.solve()

b = curl(a)

b.write(all, "b.vtu", 2)
az.write(all, "az.vtu", 2)

# Compute the total force acting on the bar using a cell integral of the virtual work term:
fvolcalc = gettotalforce(bar, b/mu, mu)[1]
# Compute the total force acting on the bar using a boundary integral of the Maxwell stress tensor T:
T = 1/mu * ( b*transpose(b) - 0.5*b*b * eye(3) )
fsurcalc = 2*getpi()*compy(on(all, T)*normal(7)).integrate(bnd, 5)


# Mechanical displacement field with 3 components:
u = field("h1xyz")

# Use interpolation order 4:
u.setorder(all, 4)

# Clamp the x and z displacement components.
# The y component is clamped using a port to be able to calculate the reaction force.
u.compx().setconstraint(clamp)
u.compz().setconstraint(clamp)

# Define the y component displacement-force port pair:
Uy = port(); Fy = port();
u.compy().setport(clamp, Uy, Fy)

elasticity = formulation()

# Force the y displacement on the clamp to zero:
elasticity += Uy - 0.0

elasticity += integral(bar, predefinedelasticity(dof(u), tf(u), 100e9, 0.3))
# Add the magnetostatic Maxwell stresses:
elasticity += integral(all, predefinedmagnetostaticforce(tf(u, bar), b/mu, mu))
# Alternatively an integration on the bar skin can be performed:
# elasticity += integral(9, compx(on(air,T)) * normal(bar) * compx(tf(u)))
# elasticity += integral(9, compx(on(bar,T)) * normal(air) * compx(tf(u)))
# elasticity += integral(9, compy(on(air,T)) * normal(bar) * compy(tf(u)))
# elasticity += integral(9, compy(on(bar,T)) * normal(air) * compy(tf(u)))

elasticity.solve()

u.write(bar, "u.vtu", 4)

# Compute the total reaction force:
freaction = 2*getpi()*Fy.getvalue()

print('Total force with cell integral is', fvolcalc, 'N')
print('Total force with boundary integral is', fsurcalc, 'N')
print('Mechanical reaction force at clamp is', freaction, 'N')

maxu = norm(u).max(bar, 5)[0]
print('Max displacement is', maxu, 'm')

# Code validation line. Can be removed.
print((abs(maxu-4.3131e-8).evaluate()/4.3131e-8 < 8e-5) and (abs(fvolcalc-1496.3).evaluate()/1496.3 < 2e-5) and (abs(fsurcalc-1496.3).evaluate()/1496.3 < 2e-5) and (abs(freaction+1496.3).evaluate()/1496.3 < 2e-5))

