from spylizard import *

wavg1 = 1; wavg2 = 2; clad = 3; all = 4; bound = 5;

mymesh = mesh()
mymesh.selectskin(bound, all)
mymesh.load("optical_waveguide.msh")

waveguides = selectunion([wavg1, wavg2])

# Waveguide boundary plotting:
expression(1).write(selectintersection([waveguides, clad]), "skin.vtu", 1)

clk = wallclock()

# Edge shape functions 'hcurl' for the tranverse electric field Et.
# Nodal shape functions 'h1' for the longitudinal electric field El.
Et = field("hcurl"); El = field("h1");

# Use interpolation order 2 on the whole domain:
Et.setorder(all, 2)
El.setorder(all, 2)

# Material properties definition
n = parameter(); epsr = parameter(); mur = parameter();
n.setvalue(clad, 1.4)
n.setvalue(wavg1, 2.0)
n.setvalue(wavg2, 2.0)
epsr.setvalue(all, n*n)
mur.setvalue(all, 1.0)

# Light property
wavelen = 680e-9; c = 299792458.0; f0 = c/wavelen; k0 = 2.0*getpi()/wavelen;
print("\nlambda =", wavelen*1e9, "nm, f0 =", f0, "Hz, k0 =", k0, "1/m\n")

# Perfect conductor boundary condition:
El.setconstraint(bound)
Et.setconstraint(bound)

# Weak formulation for the eigenvalue problem for confined propagation of an EM wave in a waveguide:
#
# 1/mur*curl(Et)*curl(Et') - k0^2*epsr*Et*Et' = -bt^2/mur*( grad(El)*Et') + Et*Et' )
#
# bt^2/mur*( grad(El)*grad(El') + Et*grad(El') ) = k0^2*bt^2*epsr* El*El'
#
# where curl() and grad() have a special definition in the transverse (xy) plane.
#
# The bt constant coming from the space derivation of exp(i*bt*z) is replaced by a time derivative dt().


# Operators grad() and curl() in the transverse plane:
dtdtgradEl = expression(3,1,[dtdt(dx(dof(El))), dtdt(dy(dof(El))), 0])
gradtfEl = expression(3,1,[dx(tf(El)), dy(tf(El)), 0])

mode = formulation()

mode += integral(all, curl(dof(Et))*curl(tf(Et))- k0*k0*mur*epsr*(dof(Et))*tf(Et))
mode += integral(all, dtdtgradEl*tf(Et)+ dtdt(dof(Et))*tf(Et))

mode += integral(all, dtdtgradEl*gradtfEl+dtdt(dof(Et))*gradtfEl)
mode += integral(all, -k0*k0*mur*epsr*dtdt(dof(El))*tf(El))	

mode.generate()

# Get the stiffness matrix K and mass matrix M:
K = mode.K()
M = mode.M()

# Create the object to solve the eigenvalue problem:
eig = eigenvalue(K, M)

# Compute the 5 eigenvalues closest to the target magnitude bt^2 (propagation constant^2).
# We are looking for modes around neff_target, the target effective refractive index.
neff_target = 1.6; bt = k0*neff_target; bt2 = bt*bt;
print("Target is neff =", neff_target, ", beta =", bt, "rad/m\n")
# Propagation mode eigenvalue is purely imaginary (i*bt), we thus target -bt^2:
eig.compute(5, -bt2)

# Get all eigenvectors and eigenvalues found:
myrealeigenvectors = eig.geteigenvectorrealpart()
myrealeigenvalues = eig.geteigenvaluerealpart()


neffcs = []

Etotal = array3x1(compx(Et), compy(Et), El)

# Loop on all eigenvalues found:
index = 1
for i in range(0, len(myrealeigenvalues)):

    if myrealeigenvalues[i] < 0:
    
        # Transfer the data from the ith eigenvector to fields Et and El:
        Et.setdata(all, myrealeigenvectors[i])
        El.setdata(all, myrealeigenvectors[i])

        # Compute the propagation constant and the effective refractive index:
        btc = sqrt(abs(myrealeigenvalues[i])).evaluate()
        neffc = btc/k0
        # We need to write separately on the clad and waveguide regions to visualize the discontinuity:
        Etotal.write(clad, "Eclad_"+ str(index) +".vtu", 2)
        Etotal.write(selectunion([wavg1,wavg2]), "Ewav_"+ str(index) +".vtu", 2)
        # Display mode information:
        print("Mode", index, ": beta =", btc, "rad/m, neff =", neffc)

        neffcs.append(neffc)
        
        index += 1
    

# Write a .pvd file to visualize all the modes together:
grouptimesteps("Eclad.pvd", "Eclad_", 1, neffcs)
grouptimesteps("Ewav.pvd", "Ewav_", 1, neffcs)

clk.print("\nTotal time elapsed:")

# Code validation line. Can be removed.
print((neffcs[0] < 1.65780) and (neffcs[0] > 1.65778))

