from spylizard import *

vol = 1; sur = 2; top = 3;

mymesh = mesh("disk.msh")

# Nodal shape functions 'h1' with 3 components.
# Field u is the membrane deflection.
u = field("h1xyz")

# Use interpolation order 2 on 'vol', the whole domain:
u.setorder(vol, 2)

# Clamp on surface 'sur' (i.e. 0 valued-Dirichlet conditions):
u.setconstraint(sur)

# E is Young's modulus. nu is Poisson's ratio. rho is the volumic mass.
E = parameter(); nu = parameter(); rho = parameter();

E.setvalue(vol, 150e9)
nu.setvalue(vol, 0.3)
rho.setvalue(vol, 2330)

# Proportional damping C = alpha*M + beta*K:
alpha = 100; beta = 0.00006;

elasticity = formulation()

# The linear elasticity formulation is classical and thus predefined:
elasticity += integral(vol, predefinedelasticity(dof(u), tf(u), E, nu))
# Add the inertia terms:
elasticity += integral(vol, -rho*dtdt(dof(u))*tf(u))
# Add the proportional damping terms to get damping matrix C = alpha*M + beta*K. 
# The mechanical equation can be written as M*dtdt(u) + C*dt(u) + K*u = 0.
# All terms with a first order time derivative on the dof are added to C.
elasticity += integral(vol, alpha * -rho*dt(dof(u))*tf(u))
elasticity += integral(vol, beta * predefinedelasticity(dt(dof(u)), tf(u), E, nu))

elasticity.generate()

# Get the stiffness, damping and mass matrix:
K = elasticity.K()
C = elasticity.C()
M = elasticity.M()

# In case of proportional damping this yields the same results:
# C = alpha*M + beta*K

# Create the object to solve the second order polynomial eigenvalue problem (M*lambda^2 + C*lambda + K)u = 0 :
eig = eigenvalue(K, C, M) # Replace by eig(K, M) for an undamped simulation

# Compute the 10 eigenvalues closest to the target magnitude 0.0 (i.e. the 10 first ones):
eig.compute(10, 0.0)

# Print the eigenfrequencies:
eig.printeigenfrequencies()

# The eigenvectors are real only in the undamped case:
myrealeigenvectors = eig.geteigenvectorrealpart()
myimageigenvectors = eig.geteigenvectorimaginarypart()

# Loop on all eigenvectors found:
for i in range(0, len(myrealeigenvectors)):

    # Transfer the data from the ith eigenvector to field u:
    u.setdata(vol, myrealeigenvectors[i])
    # Write the deflection on the membrane with an order 2 interpolation:
    u.write(vol, "u"+str(i)+".vtk", 2)

# Code validation line. Can be removed.
checkval = eig.geteigenvaluerealpart()[1]*myimageigenvectors[3].norm()
print((checkval > -0.0029259) and (checkval < -0.00292588))

