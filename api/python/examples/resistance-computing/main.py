from spylizard import *

# The domain regions as defined in 'quad.geo':
all = 1; left = 2; right = 3;

mymesh = mesh("quad.msh")

# Nodal shape functions 'h1' for the electric potential field v:
v = field("h1"); x = field("x"); y = field("y");

# Use interpolation order 2:
v.setorder(all, 2)

# Constraints on v are set with ports here

# Electric conductivity [S/m] increasing with the y coordinate:
sigma = 0.1 * (1+8*y)

R = 3.0 # Ohm

# Define the V0 port not associated to the FEM problem: 
V0 = port()

# Associate a V/I (voltage/current) port pair to field v on the left
# and right electrodes. Field v will be constant on each electrode.
Vl = port(); Il = port(); Vr = port(); Ir = port();

v.setport(left, Vl, Il)
v.setport(right, Vr, Ir)
#                |   |
#      primal port   dual port
#
# The dual port holds the global Neumann term on the port region.
# For an electrokinetic formulation this equals the total current.

electrokinetic = formulation()

# Set a 1 A current flowing in through the left electrode:
electrokinetic += Il - 1.0
# Link the right electrode to port V0:
electrokinetic += Vr - (V0 - R*Ir)
# Set V0 to 1V:
electrokinetic += V0 - 1.0

# Define the weak formulation for the DC current flow:
electrokinetic += integral(all, -sigma * grad(dof(v)) * grad(tf(v)))

# Generate, solve and transfer the solution to field v and to the ports:
electrokinetic.solve()

vl = Vl.getvalue(); vr = Vr.getvalue();
il = Il.getvalue(); ir = Ir.getvalue();

# Compute the FEM resistance:
Rfem = (vl - vr)/il

print("Vl =", vl, "V | Vr =", vr, "V")
print("Il =", il, "A | Ir =", ir, "A")
print("FEM resistance is", Rfem, "Ohm")

# Write v and j for illustration:
v.write(all, "v.pos", 2)
(sigma * -grad(v)).write(all, "j.pos", 2)

# Code validation line. Can be removed.
print((Rfem < 2+1e-14) and (Rfem > 2-1e-14) and (vr < 4+1e-14) and (vr > 4-1e-14))

