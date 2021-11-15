from spylizard import *

def createmesh():

    # Give names to the physical region numbers:
    volume = 1; input = 2; output = 3;

    # Define the x, y and z coordinate fields:
    x = field("x"); y = field("y"); z = field("z");

    length = 0.2; thickness = 0.01; width = 0.05;

    # Define the 2D base to be extruded:
    leftquad = shape("quadrangle", -1, [-length/2,-width/2,-thickness/2, -0.5*length/2,-width/2,-thickness/2, -0.5*length/2,width/2,-thickness/2, -length/2,width/2,-thickness/2], [8,8,8,8])
    rightquad = shape("quadrangle", -1, [0.5*length/2,-width/2,-thickness/2, length/2,-width/2,-thickness/2, length/2,width/2,-thickness/2, 0.5*length/2,width/2,-thickness/2], [8,8,8,8])
    centralquad = shape("quadrangle", -1, [-0.5*length/2,-width/2,-thickness/2, 0.5*length/2,-width/2,-thickness/2, 0.5*length/2,width/2,-thickness/2, -0.5*length/2,width/2,-thickness/2], [16,8,16,8])

    # Extrude the 2D base:
    leftblock = leftquad.extrude(volume, thickness, 3)
    rightblock = rightquad.extrude(volume, thickness, 3)
    centralblock = centralquad.extrude(volume, thickness, 3)

    # Make the central block less large in the middle:
    centralblock.move(array3x1(0, 15*(abs(x)-0.5*length/2)*y, 0))
    # Make it also a little thinner (in the z direction) in the middle:
    centralblock.move(array3x1(0, 0, 15*(abs(x)-0.5*length/2)*z))

    # Get the input and output faces:
    inputface = leftblock.getsons()[4]
    outputface = rightblock.getsons()[2]

    # Assign the physical region numbers as detailed above:
    inputface.setphysicalregion(input)
    outputface.setphysicalregion(output)

    # Load to the mesh all shapes important for the finite element simulation:
    mymesh = mesh([leftblock,rightblock,centralblock,inputface,outputface])

    # The mesh can be written at any time to have a feedback while creating the geometry!
    # mymesh.write("meshed.msh")

    return mymesh


volume = 1; input = 2; output = 3;

# Create the geometry and the mesh:    
mymesh = createmesh()

# Write to file for visualization:
mymesh.write("meshed.msh")

# Voltage [V] applied between the input and output faces:
appliedvoltage = 0.2

# Create the electric potential field v and temperature field T (both with nodal shape functions h1):
v = field("h1"); T = field("h1");

# Use an order 2 interpolation for both fields:
v.setorder(volume, 2)
T.setorder(volume, 2)

# Apply the requested voltage to the input and ground the output:
v.setconstraint(input, appliedvoltage)
v.setconstraint(output)
# Assume the temperature is fixed to 293 K on the input and output faces:
T.setconstraint(input, 293)
T.setconstraint(output, 293)

# The tungsten resistivity 'rho' [Ohm*m] is a function of the temperature (5.60e-8 at 293 K with a 0.0045 [K^-1] temperature coefficient): 
rho = parameter(); k = parameter();
rho.setvalue(volume, 5.6e-8*(1+0.0045*(T-293)))
# Its thermal conductivity varies from 173 to 118 [W/(m*K)] between 293 and 1000 K: 
k.setvalue(volume, 173-(173-118)*(T-293)/(1000-293))

# Expression for the electric field E [V/m] and current density j [A/m^2]:
E = -grad(v)
j = 1/rho * E


# Define the weak formulation for the static current flow.
#
# The strong form is:
#
# div(1/rho * grad(v)) = 0
#	
# with E = -grad(v)
#	
electrokinetics = formulation()

electrokinetics += integral(volume, grad(tf(v))*1/rho*grad(dof(v)))

# Define the weak formulation for the heat equation.
#
# Can be found at https:#en.wikiversity.org/wiki/Nonlinear_finite_elements/Weak_form_of_heat_equation
#
# Strong form is r*cp*dT/dt - div(k*grad(T)) = volumic heat sources [W/m^3]
#
# where r [kg/m^3] is the density and cp [J/(kg*K)] is the specific heat capacity.
#
heatequation = formulation()

# Here the time derivative is zero (static simulation):
# heatequation += integral(volume, r*cp*dt(dof(T))*tf(T))
heatequation += integral(volume, grad(tf(T))*k*grad(dof(T)))
# Add the current heating source:
heatequation += integral(volume, -j*E*tf(T))



# Start with a uniform 293 K temperature everywhere:
T.setvalue(volume, 293)

# Initial all-zero solution vector for the heat equation:
solheat = vec(heatequation)

relres = 1.0
while relres > 1e-7:

    # Compute the static current everywhere:
    electrokinetics.generate()
    # Get A and b to solve Ax = b:
    solv = solve(electrokinetics.A(), electrokinetics.b())
    # Transfer the data from the solution vector to the v field to be used for the heat equation below:
    v.setdata(volume, solv)
    
    # Deduce the temperature field everywhere:
    heatequation.generate()
    # Get A and b to solve Ax = b:
    Aheat = heatequation.A()
    bheat = heatequation.b()
    
    # Compute a relative residual:
    relres = (bheat - Aheat*solheat).norm() / bheat.norm()
    
    # Solve Ax = b:
    solheat = solve(Aheat, bheat)
    
    # Transfer the data from the solution vector to the T field to be used for the electrokinetics above:
    T.setdata(volume, solheat)
    
    print("Current iteration has relative residual:", relres)


# Compute the total current flowing through the input face
# in an alternative (but less accurate) way to using ports.
#
# Since the computation involves a gradient that has to be 
# calculated in the volume (and not on the input face) 
# one can not simply call (normal(volume)*j).integrate(input,4)
# since with this a surface gradient will be calculated.
# 'on()' is called to force the evaluation in the volume.
I = (-normal(volume)*on(volume, j)).integrate(input, 4)
# Compute the electric resistance R between input and output:
R = appliedvoltage/I

print("\nResistance is", R, "Ohm. Current is", I, "A\n")

# Compute the peak temperature on the whole volume:
peaktemperature = T.max(volume,2)[0]
print("Peak temperature is", peaktemperature, "K\n")

# Write v, j and T:
v.write(volume, "v.pos", 2)
j.write(volume, "j.pos", 2)
T.write(volume, "T.pos", 2)

# Code validation line. Can be removed.
print((peaktemperature < 618.446) and (peaktemperature > 618.444) and (I < 1851.30) and (I > 1851.28))

