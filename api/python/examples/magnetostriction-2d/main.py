from spylizard import *

gaps = 1; core = 2; windingpos = 3; windingneg = 4; air = 5; clamp = 7; boundary = 8;

# Load the mesh, refine it by splitting once and define the domain boundary:
mymesh = mesh()
mymesh.split(1)
mymesh.selectskin(boundary)
mymesh.load("choke.msh")

all = selectall()
solid  = selectunion([core, gaps])
nonmag = selectunion([air, gaps, windingpos, windingneg])

# Winding current [A] times number of turns:
In = 200 * 100
# Calculate the current density:
jsource = In / expression(1).integrate(windingpos, 5)

# Nodal shape functions 'h1' for the z component of the vector potential.
az = field("h1")
az.setorder(all, 2)

# Put a magnetic wall at the outer air boundary:
az.setconstraint(boundary)

# In 2D the vector potential only has a z component:
a = array3x1(0,0,az)

# Vacuum magnetic permeability [H/m]: 
mu0 = 4.0*getpi()*1e-7

# Define the permeability in all regions.
mu = parameter()

mu.setvalue(core, 1000.0*mu0)
mu.setvalue(nonmag, mu0)

magnetostatics = formulation()

# The strong form of the magnetostatic formulation is curl( 1/mu * curl(a) ) = j, with b = curl(a):
magnetostatics += integral(all, 1/mu * curl(dof(a)) * curl(tf(a)) )
# Add the current density source jsource [A/m2] in the z direction:
magnetostatics += integral(windingpos, -array3x1(0,0,jsource) * tf(a))
magnetostatics += integral(windingneg, -array3x1(0,0,-jsource) * tf(a))

magnetostatics.solve()

b = curl(a)

B = norm(b)
bx = compx(b)
by = compy(b)

az.write(all, "az.pos", 1)
b.write(all, "b.pos", 1)


# Compute the elastic deformation due to the Maxwell and magnetostrictive stresses.

# Load the measured magnetostriction data:
measureddata = spline("magnetostriction.txt")

# Define the expression giving lambda tangent as a function of the magnetic induction b.
# This internally uses a natural cubic spline interpolation of the loaded data samples.
lT = expression(measureddata, norm(b))
lT = lT * 1e-6 # data loaded is in um/m
lN = -0.5 * lT

# Mechanical displacement field:
u = field("h1xy")
u.setorder(all, 2)

# E is Young's modulus. nu is Poisson's ratio:
E = parameter(); nu = parameter();

E.setvalue(core, 220e9)
E.setvalue(gaps, 15e9)

nu.setvalue(core, 0.3)
nu.setvalue(gaps, 0.3)

u.setconstraint(clamp)


# Magnetostrictive strain (in the xy coordinates) in Voigt form (xx,yy,xy):
ems = 1/(B*B) * array3x1(lT*bx*bx + lN*by*by, lT*by*by + lN*bx*bx, bx*by*(lT-lN))
# Plane stress elasticity tensor:
H = E/(1-nu*nu) * array3x3(1,nu,0, nu,1,0, 0,0,0.5*(1-nu))


elasticity = formulation()

elasticity += integral(solid, predefinedelasticity(dof(u), tf(u), E, nu, "planestress"))
# Magnetostatic Maxwell stresses:
elasticity += integral(all, predefinedmagnetostaticforce(tf(u, solid), b/mu, mu) )
# Magnetostrictive stresses:
elasticity += integral(core, H * ems * strain(tf(u)) )

elasticity.solve()

probeval = norm(u).interpolate(solid, [0, 0.33438, 0])[0]

print("U max is", norm(u).max(solid, 5)[0], "and U probe is", probeval, "m")

u.write(solid, "u.pos", 1)

# Code validation line. Can be removed.
print(abs(probeval - 1.127e-07).evaluate()/probeval < 1e-4)

