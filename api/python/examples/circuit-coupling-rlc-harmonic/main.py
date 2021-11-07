from spylizard import *

all = 1; left = 2; right = 3;

mymesh = mesh("quad.msh")

# Supply frequency f0 is 1 kHz:
setfundamentalfrequency(1000)

# Nodal shape functions 'h1' for the electric potential field v.
# Use harmonics 2 and 3 for a frequency analysis @f0 Hz so that
# v = v2*sin(2*pi*f0*t) + v3*cos(2*pi*f0*t).
v = field("h1", [2,3])

# Use interpolation order 2:
v.setorder(all, 2)

# Ground the right FEM side:
v.setconstraint(right)

# Electric conductivity [S/m] to get a 200 Ohm FEM resistance:
sigma = 0.005

# Associate a V/I (voltage/current) port pair to field v on the electrode.
# The electrode is supposed to be a perfect conductor with a constant v.
V = port([2,3]); I = port([2,3]); Vin = port([2,3]);

v.setport(left, V, I)
#               |  |
#     primal port  dual port
#
# The dual port holds the global Neumann term on the port region.
# For an electrokinetic formulation this equals the total current.
#
# I is positive if it enters the FEM block.

L = 100e-3; C = 1e-6;

elec = formulation()

# Supply voltage Vin harmonic 2 is set to 1 V while harmonic 3 is set to 0V.
# We therefore get Vin = 1.0*sin(2*pi*f0*t) + 0.0*cos(2*pi*f0*t).
elec += Vin.harmonic(2) - 1.0
elec += Vin.harmonic(3) - 0.0
# Connect the LC components to the FEM block. The relations are
# Vin = V + Vl + Vc with Vl = L*dt(I) and dt(Vc) = 1/C*I.
elec += dt(Vin) - (dt(V) + L*dtdt(I) + 1/C*I)

# Define the weak formulation for the conduction current flow:
elec += integral(all, -sigma * grad(dof(v)) * grad(tf(v)))

# Generate, solve and transfer the solution to field v and to the ports:
elec.solve()

# Calculate the real and imaginary part of the impedance Z:
Vr = 1.0; Vi = 0.0; Ir = I.harmonic(2).getvalue(); Ii = I.harmonic(3).getvalue();

Vc = complex(Vr, Vi); Ic = complex(Ir, Ii);
Z = Vc/Ic

print('RLC impedance at 1kHz is', Z.real, '+', Z.imag, "j Ohm")

Zrexact = 200
Ziexact = 469.1635876260633

print('Exact value is', Zrexact, '+', Ziexact, 'j Ohm')

# Code validation line. Can be removed.
print((abs(Z.real - Zrexact).evaluate()/Zrexact < 1e-14) and (abs(Z.imag - Ziexact).evaluate()/Ziexact < 1e-14))

