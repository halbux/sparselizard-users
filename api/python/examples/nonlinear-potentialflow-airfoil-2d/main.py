from spylizard import *

# The domain regions as defined in 'airfoil2D.geo':
air = 1; airfoil = 2; downstream = 3; upstream = 4;

# Load the 'airfoil2D' mesh: 
mymesh = mesh("airfoil2D.msh")

# Define the whole outer boundary:
gammaouter = selectunion([upstream, downstream])

# Define the velocity potential field 'phi' with standard nodal shape functions ("h1").
# grad(phi) is the fluid velocity. Field x is the x coordinate field.
phi = field("h1"); x = field("x");

# Use interpolation order 1:
phi.setorder(air, 1)

# Specific weight of air under some circumstances:
gamma = 1.4

# Define the air density 'rho' and the Mach number:
rho = pow(1 + (gamma-1)/2.0 * 0.7 * 0.7 * (1-grad(phi)*grad(phi)), 1.0/(gamma-1))
machnumber = sqrt(grad(phi)*grad(phi)) / sqrt(1.0/(0.7*0.7) + 0.5*(gamma-1) * (1-grad(phi)*grad(phi)))

# We suppose outside the air domain a uniform speed of 1 with direction left to right.
# Since grad(phi) is the speed we have that grad(x) indeed gives us what we want.
phi.setconstraint(gammaouter, x)

# Define the potential flow formulation:
pf = formulation()

# On the airfoil boundary the default Neumann condition dphi/dnormal = 0 
# automatically ensures that there is no fluid entering the airfoil.
# We thus do not need anything else than this:
pf += integral(air, rho * grad(dof(phi)) * grad(tf(phi)) )

# Start the nonlinear iteration with an all zero guess:
sol = vec(pf)

relres = 1.0
while relres > 1e-7:

    # Generate the formulation:
    pf.generate()
    # Get A and b to solve Ax = b:
    A = pf.A()
    b = pf.b()
    
    # Compute a relative residual:
    relres = (b - A*sol).norm() / b.norm()
    
    # Solve Ax = b:
    sol = solve(A, b)
    
    # Transfer the data from the solution vector to field 'phi' on the whole 'air' region:
    phi.setdata(air, sol)
    
    print("Current iteration has relative residual:", relres)


# Write the fluid speed (i.e. grad(phi)) and the Mach number:
grad(phi).write(air, "flowspeed.pos", 1)
machnumber.write(air, "machnumber.pos", 1)

# Code validation line. Can be removed.
print((machnumber.integrate(air, 3) < 62.4149) and (machnumber.integrate(air, 3) > 62.4145))

