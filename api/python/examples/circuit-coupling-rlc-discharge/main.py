from spylizard import *

all = 1; left = 2; right = 3;

mymesh = mesh("quad.msh")

# Nodal shape functions 'h1' for the electric potential field v:
v = field("h1")

# Use interpolation order 2:
v.setorder(all, 2)

# Ground the right FEM side:
v.setconstraint(right)

# Electric conductivity [S/m]:
sigma = 0.005

# Associate a V/I (voltage/current) port pair to field v on the electrode.
# The electrode is supposed to be a perfect conductor with a constant v.
Vc = port(); I = port(); V = port();

v.setport(left, V, I)
#               |  |
#     primal port  dual port
#
# The dual port holds the global Neumann term on the port region.
# For an electrokinetic formulation this equals the total current.
#
# I is positive if it enters the FEM block.

R = 360; L = 100e-3; C = 0.1e-6;

elec = formulation()

# Set the RLC circuit equations:
elec += Vc - (V + R*I + L*dt(I))
elec += dt(Vc) + 1/C*I

# Define the weak formulation for the conduction current flow:
elec += integral(all, -sigma * grad(dof(v)) * grad(tf(v)))

# Initial capacitor voltage is 100 V:
Vc.setvalue(100)

# Time resolution object (all matrices can be reused):
ga = genalpha(elec, vec(elec), vec(elec), 0, [1, 1, 1, 1])

# Lowest high-frequency damping:
ga.setparameter(1)

# Store the Vc and I values at all timesteps:
Vcvals = []; Ivals = []; tvals = [];

settime(0)
while gettime() < 2000e-6:

    Vcvals.append(Vc.getvalue())
    Ivals.append(I.getvalue())
    tvals.append(gettime())
    
    ga.next(5e-6)

print('FEM resistance is', V.getvalue()/I.getvalue(), 'Ohm')

# Write the vectors to text file:
writevector("Vc.txt", Vcvals)
writevector("I.txt", Ivals)
writevector("t.txt", tvals)

# Code validation line. Can be removed.
print((abs(Ivals[27] - 6.8700e-02).evaluate()/Ivals[27] < 2e-4) and (abs(Ivals[92] + 2.7478e-02).evaluate()/abs(Ivals[92]).evaluate() < 6e-4))

