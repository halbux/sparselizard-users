from spylizard import *

line = 1; left = 2; right = 3;

mymesh = mesh("line.msh")

# Nodal shape functions 'h1' for the electric potential field:
v = field("h1")

# Use interpolation order 1 on the whole domain:
v.setorder(line, 1)

# Force 10 V on the left and 2 V on the right:
v.setconstraint(left, 10)
v.setconstraint(right, 2)

# epsilon is the electric permittivity:
epsilon = 8.854e-12

electrostatics = formulation()

electrostatics += integral(line, -epsilon*grad(dof(v))*grad(tf(v)))

electrostatics.generate()

solv = solve(electrostatics.A(), electrostatics.b())

# Transfer the data from the solution vector to the v field:
v.setdata(line, solv)
# Write v:
v.write(line, "v.pos", 1)

# Code validation line. Can be removed.
print((solv.norm() < 21.5964) and (solv.norm() > 21.5962))

