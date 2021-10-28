from pinelizard import *

dielectric = 1; air = 2; electrode = 3; ground = 4;
    
mymesh = mesh("capacitor.msh")
    
# Select the entire domain:
all = selectall()

# Nodal shape functions 'h1' for the electric potential field v:
v = field("h1")

# Use interpolation order 3:
v.setorder(all, 3)

# Ground:
v.setconstraint(ground)

# Electric permittivity
epsilon = parameter()

epsilon[air] = 8.854e-12
epsilon[dielectric] = 3.9 * 8.854e-12

# Associate a V/Q (voltage/charge) port pair to field v on the electrode.
# The electrode is supposed to be a perfect conductor with a constant v.
V = port(); Q = port();

v.setport(electrode, V, Q)
#                    |  |
#          primal port  dual port
#
# The dual port holds the global Neumann term on the port region.
# For an electrostatic formulation this equals the electrode charge.

electrostatics = formulation()

# Set a 0.1 nC charge per unit depth on the electrode:
electrostatics += Q - 0.1e-9

electrostatics += integral(all, -epsilon * grad(dof(v)) * grad(tf(v)))

# Generate, solve and transfer the solution to field v and to the ports:
electrostatics.solve()

# Compute the capacitance:
C = Q.getvalue() / V.getvalue()

print(' Capacitance is ',  C , ' F per unit depth')
print(' Electrode voltage is ' , V.getvalue() , ' V')

# Write v and E for illustration:
v.write(all, "v.pos", 2)
(-grad(v)).write(all, "E.pos", 2);

# Code validation line. Can be removed.
print( (C < 1.30635e-10) and (C > 1.30633e-10))

