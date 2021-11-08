from spylizard import *

left = 1; skin = 2; wholedomain = 3;

mymesh = mesh("waveguide2D.msh")

# Edge shape functions 'hcurl' for the electric field E.
# Fields x and y are the x and y coordinate fields.
E = field("hcurl"); x = field("x"); y = field("y");

# Use interpolation order 2 on the whole domain:
E.setorder(wholedomain, 2)

# The cutoff frequency for a 0.2 m width is freq = 0.75 GHz in theory. 
# With this code and a fine enough mesh you will get the same value.
freq = 0.9e9; c = 3e8; pi = 3.14159;

# The waveguide is a perfect conductor. We thus force all
# tangential components of E to 0 on the waveguide skin.
E.setconstraint(skin)
# We force an electric field in the y direction on region 'left'
# that is 0 on the exterior of 'left' and one sine period inside.
# The electric field varies in time at frequency freq.
E.setconstraint(left, sin(y/0.1*pi)* sin(2*pi*freq*t()) *array3x1(0,1,0))

maxwell = formulation()

# This is the weak formulation for electromagnetic waves in time:
maxwell += integral(wholedomain, -curl(dof(E))*curl(tf(E)) - 1/(c*c)*dtdt(dof(E))*tf(E))

# Define the generalized alpha object to time-solve formulation 'maxwell' 
# with initial all zero time derivative vectors 'vec(maxwell)'.
# The general system to solve in time is M*dtdtx + C*dtx + K*x = b.
#
# The last argument is a vector 'isconstant' telling if the:
#
# - excitation vector b is constant in time for isconstant[0]
# - matrix K is constant in time for isconstant[1]
# - matrix C is constant in time for isconstant[2]
# - matrix M is constant in time for isconstant[3]
#
# You can set isconstant[0] to 'true' if in b only the constraints
# are time dependent but the other excitation sources are not. 
#
# Setting properly the 'isconstant' vector can give a dramatic speedup
# since it may avoid reassembling or allow reusing the matrix factorization.
#
ga = genalpha(maxwell, vec(maxwell), vec(maxwell), 1, [1, 1, 1, 1])

timestep = 0.05*1.0/freq
numsteps = 400

settime(0)
for ts in range(0, numsteps):

    # Compute one timestep:
    ga.next(timestep)
       
    # Write with an order 2 interpolation and with the name of your choice:
    E.write(wholedomain, "E"+str(ts+100)+".pos", 2)


# Code validation line. Can be removed.
print((ga.gettimederivative()[0].norm() < 3.0665e+09) and (ga.gettimederivative()[0].norm() > 3.0661e+09))

