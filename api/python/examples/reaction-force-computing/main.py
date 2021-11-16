from spylizard import *

# The domain regions as defined in 'disk.geo':
vol = 1; sur = 2; top = 3;

# The mesh is curved to capture accurately the circular geometry:
mymesh = mesh("disk.msh")

# Nodal shape functions 'h1' with 3 components.
# Field u is the membrane deflection.
u = field("h1xyz")

# Volume of the disk (radius is 1 m):
volume = getpi()*0.1

# Use interpolation order 2 on 'vol', the whole domain:
u.setorder(vol, 2)

# Associate a U/F (displacement/force) port pair to field u on the clamp face 'sur'.
# The displacement components on the port region must be constant (here equal to zero).
Ux = port(); Uy = port(); Uz = port(); Fx = port(); Fy = port(); Fz = port();
u.compx().setport(sur, Ux, Fx)
u.compy().setport(sur, Uy, Fy)
u.compz().setport(sur, Uz, Fz)
#                       |  |
#             primal port  dual port
#
# The dual port holds the global Neumann term on the port region.
# For an elasticity formulation this equals the reaction force.

elasticity = formulation()

# Add port relations Ux = 0, Uy = 0 and Uz = 0 to clamp the disk:
elasticity += Ux - 0.0
elasticity += Uy - 0.0
elasticity += Uz - 0.0

# The linear elasticity formulation is classical and thus predefined:
elasticity += integral(vol, predefinedelasticity(dof(u), tf(u), 150e9, 0.3))
# Add an arbitrary uniform volumic force (N/m^3) in the x, y and z direction:
elasticity += integral(vol, array3x1(1,2,3)*tf(u))

# Generate, solve and transfer the solution to field u and to the ports:
elasticity.solve()

print("Reaction force on clamp is (", Fx.getvalue(), ",", Fy.getvalue(), ",", Fz.getvalue(), ") N")
print("Exact reaction on clamp is (", -1*volume, ",", -2*volume, ",", -3*volume, ") N")

# Print the max deflection:
print("Max displacement is", norm(u).max(vol, 5)[0], "m")

# Write the deflection to ParaView .vtk format. Write with an order 2 interpolation.
u.write(vol, "u.vtk", 2)

# Code validation line. Can be removed.
fsum = Fx.getvalue() + Fy.getvalue() + Fz.getvalue()
print((-fsum < 1.8849559) and (-fsum > 1.8849555))

