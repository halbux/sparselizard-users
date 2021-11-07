from spylizard import *

# The domain regions as defined in 'disk.geo':
vol = 1; sur = 2; top = 3;

mymesh = mesh("disk.msh")

# Nodal shape functions 'h1' with 3 components.
# Field u is the membrane deflection.
u = field("h1xyz")

# Use interpolation order 3 on 'vol', the whole domain:
u.setorder(vol, 3)

# Clamp on surface 'sur' (i.e. 0 valued-Dirichlet conditions):
u.setconstraint(sur)

# E is Young's modulus. nu is Poisson's ratio. rho is the volumic mass.
E = parameter(); nu = parameter(); rho = parameter();

E.setvalue(vol, 150e9)
nu.setvalue(vol, 0.3)
rho.setvalue(vol, 2330)

elasticity = formulation()

# The linear elasticity formulation is classical and thus predefined:
elasticity += integral(vol, predefinedelasticity(dof(u), tf(u), E, nu))
# Add the inertia terms:
elasticity += integral(vol, -rho*dtdt(dof(u))*tf(u))

elasticity.generate()

# Get the stiffness and mass matrix:
K = elasticity.K()
M = elasticity.M()

# Create the object to solve the generalized eigenvalue problem K*x = lambda*M*x :
eig = eigenvalue(K, M)

# Compute the 10 eigenvalues closest to the target magnitude 0.0 (i.e. the 10 first ones):
eig.compute(10, 0.0)

# Print the eigenfrequencies:
eig.printeigenfrequencies()

# The eigenvectors are real thus we only need the real part:
myeigenvectors = eig.geteigenvectorrealpart()

# Loop on all eigenvectors found:
for i in range(0, len(myeigenvectors)):

    # Transfer the data from the ith eigenvector to field u:
    u.setdata(top, myeigenvectors[i])
    # Write the deflection on the top surface of the membrane with an order 3 interpolation:
    u.write(top, "u"+str(i)+".pos", 3)

# Code validation line. Can be removed.
print((eig.geteigenvaluerealpart()[0] < 6.25240e+06) and (eig.geteigenvaluerealpart()[0] > 6.25235e+06))

