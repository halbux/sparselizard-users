from spylizard import *

import numpy as np

def sparselizard(alpha):
	
    # Give names to the physical region numbers:
    rotmagmat = 1; magnet = 2; magnetgap = 3; gaprot = 4; gapstat = 5; statmagmat = 6; windslot = 7; winda = 8; windb = 9; windc = 10;
    gammarot = 11; gammastat = 12; gamma1rot = 13; gamma2rot = 14; gamma1stat = 15; gamma2stat = 16; inarc = 17; outarc = 18;

    # Load the rotor and stator mesh without merging the interface. Set verbosity to 0.
    mymesh = mesh(0, ["rotor.msh", "stator.msh"], 0)

    # Define new physical regions for convenience:
    rotor = selectunion([rotmagmat, magnet, magnetgap, gaprot])
    windings = selectunion([winda, windb, windc])
    stator = selectunion([gapstat, windslot, statmagmat, windings])
    nonmag = selectunion([magnet, magnetgap, gaprot, gapstat, windslot, windings])
    gamma1 = selectunion([gamma1rot,gamma1stat])
    gamma2 = selectunion([gamma2rot,gamma2stat])
    all = selectunion([rotor,stator])
    
    mymesh.rotate(rotor, 0,0,alpha)

    # Peak winding current [A] times number of turns:
    Imax = 300
    # Calculate the area of a winding:
    windarea = expression(1).integrate(winda, 5)


    # Nodal shape functions 'h1' for the z component of the vector potential.
    az = field("h1"); x = field("x"); y = field("y"); z = field("z");

    # In 2D the vector potential only has a z component:
    a = array3x1(0,0,az)

    # Use interpolation order 2:
    az.setorder(all, 2)

    # Put a magnetic wall at the inner rotor and outer stator boundaries:
    az.setconstraint(inarc)
    az.setconstraint(outarc)

    # The remanent induction field in the magnet is 0.5 Tesla perpendicular to the magnet:
    normedradialdirection = array3x1(x,y,0)/sqrt(x*x+y*y)
    bremanent = 0.5 * normedradialdirection


    # Vacuum magnetic permeability [H/m]: 
    mu0 = 4.0*getpi()*1e-7

    # Define the permeability in all regions.
    #
    # Taking into account saturation and measured B-H curves can be easily done
    # by defining an expression based on a 'spline' object (see documentation).
    #
    mu = parameter()

    mu.setvalue(all, 2000.0*mu0)
    # Overwrite on non-magnetic regions:
    mu.setvalue(nonmag, mu0)


    magnetostatics = formulation()

    # The strong form of the magnetostatic formulation is curl( 1/mu * curl(a) ) = j, with b = curl(a):
    magnetostatics += integral(all, 1/mu* curl(dof(a)) * curl(tf(a)) )

    # Add the remanent magnetization of the rotor magnet:
    magnetostatics += integral(magnet, -1/mu* bremanent * curl(tf(a)))

    # Add the current density source js [A/m2] in the z direction in the stator windings.
    # A three-phased actuation is used. The currents are dephased by the mechanical angle
    # times the number of pole pairs. This gives a stator field rotating at synchronous speed.

    # Change the phase (degrees) to tune the electric angle: 
    phase = 0

    jsz = parameter()

    jsz.setvalue(winda, Imax/windarea * sin( (phase + 4.0*alpha - 0.0) * getpi()/180.0))
    jsz.setvalue(windb, Imax/windarea * sin( (phase + 4.0*alpha - 60.0) * getpi()/180.0))
    jsz.setvalue(windc, Imax/windarea * sin( (phase + 4.0*alpha - 120.0) * getpi()/180.0))

    magnetostatics += integral(windings, -array3x1(0,0,jsz) * tf(a))

    # Rotor-stator continuity condition (including antiperiodicity settings with factor '-1'):
    magnetostatics += continuitycondition(gammastat, gammarot, az, az, [0,0,0], alpha, 45.0, -1.0, 1)
    # Rotor and stator antiperiodicity condition:
    magnetostatics += periodicitycondition(gamma1, gamma2, az, [0,0,0], [0,0,45.0], -1.0, 1)


    magnetostatics.solve()

    az.write(all, "a"+str(int(alpha))+".vtu", 2)
    curl(a).write(all, "b"+str(int(alpha))+".vtu", 2)


    # The magnetostatic force acting on the motor is computed below.

    # This field will hold the x and y component of the magnetic forces:
    magforce = field("h1xy")
    magforce.setorder(all, 1)

    # The magnetic force is projected on field 'magforce' on the solid stator region.
    # This is done with a formulation of the type dof*tf - force calculation = 0.
    forceprojection = formulation()

    forceprojection += integral(statmagmat, dof(magforce)*tf(magforce))
    forceprojection += integral(stator, - predefinedmagnetostaticforce(tf(magforce, statmagmat), 1/mu*curl(a), mu))

    forceprojection.solve()

    # Calculate the torque:
    leverarm = array3x1(x,y,0)

    torque = compz(crossproduct(leverarm, magforce)).integrate(statmagmat, 5)

    # The torque has to be scaled to the actual motor z length (50 mm) and multiplied
    # by 8 to take into account the full 360 degrees of the motor.
    # A minus sign gives the torque on the rotor (opposite sign than the stator torque).
    torque = - torque * 8.0 * 0.05
    
    # To compute the torque with Arkkio's formula replace the above by the code below.
    #
    # ez = array3x1(0,0,1)
    # rer = array3x1(x,y,z)
    # ret = crossproduct(ez,rer)
    # area = expression(1).integrate(gaprot, 5)
    # torque = 0.05 * (2.0*getpi()/area * curl(a)*rer * 1.0/mu0 * curl(a)*ret).integrate(gaprot, 5)

    return torque


	
clk = wallclock()

torque = 0

print('Mechanical angle [degrees] and torque [Nm]:')

for alpha in np.arange(0.0, 45.1, 1.0):

    torque = sparselizard(alpha)
    print(alpha, torque)

clk.print("Total run time:")

# Code validation line. Can be removed:
print((torque < 3.58709) and (torque > 3.58707))

