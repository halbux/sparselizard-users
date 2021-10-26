from pinelizard import *

mymesh = mesh("natconv.msh")

p0=1.01325e5; T0=293.15; Tc=(37+273.15-T0);

fluid = 1; disk = 2; inlet = 3; outlet = 4; sides = 5;

all = selectunion([fluid, disk]);
diskskin = selectintersection([fluid, disk]);

zflux = selectunion([sides, outlet]);
noslip = selectunion([sides, diskskin]);
fluidskin = selectunion([sides, diskskin, inlet, outlet]);

v = field("h1xy"); p = field("h1"); T = field("h1"); x = field("x"); field("y");

p.setorder(all, 2); 
v.setorder(all, 2); 
T.setorder(all, 2);

rho0, muB,lmb,kappa,Cp,rhot,dtrho,gradrho = (expression,)*8

R_const=8.3144598;Rs=287

lmb=-8.38278e-7+8.35717342E-8*(T0)-7.69429583E-11*T0**2+4.6437266E-14*T0**3-1.06585607E-17*T0**4

muB=0.6*lmb
kappa=-0.00227583562+1.15480022E-4*(T0)-7.90252856E-8*T0**2+4.11702505E-11*T0**3-7.43864331E-15*T0**4
Cp=1047.63657-0.372589265*(T0)+9.45304214E-4*T0**2-6.02409443E-7*T0**3+1.2858961E-10*T0**4
rho0=(p0)*0.02897/(R_const*(T0))
rhot=(p0+p)*0.02897/(R_const*(T+T0))
dtrho=rho0*(dt(p)/(p+p0)-dt(T)/(T+T0))
gradrho=rho0*(grad(p)/(p+p0)-grad(T)/(T+T0))

v.setconstraint(noslip, array2x1(0,0))
v.setconstraint(inlet,1e-3*array2x1(0,1))
p.setconstraint(outlet, 0)

T.setconstraint(disk, Tc)
T.setconstraint(inlet, 0.0)

criterion = zienkiewiczzhu(grad(compx(v))) + zienkiewiczzhu(grad(compy(v)))

errtarget = 1.0;
errorindicator = ifpositive(criterion - errtarget, 1, 0)

T.setvalue(disk, Tc);

crit = ifpositive( abs(T) - 1.0, 1, 0 )

mymesh.setadaptivity(crit, 0, 3)

for i in range(3):
    adapt(1)
    
crit = errorindicator

mymesh.setadaptivity(crit, 0, 3)

v.setorder(crit, 2, 3)
p.setorder(crit, 1, 2)
T.setorder(crit, 1, 2)

convec = formulation()

convec += integral(fluid, predefinednavierstokes(dof(v), tf(v),v, dof(p), tf(p), lmb, rhot, dtrho, gradrho, True,False,True))
convec += integral(fluid, (rhot)*9.81*array2x1(0,-1)*tf(v))
convec += integral(fluid,1e-10*grad(dof(p))*grad(tf(p)))
       
convec += integral(fluid,kappa*grad(dof(T))*grad(tf(T)))
convec += integral(fluid,rho0*Cp*dt(dof(T))*tf(T))
convec += integral(fluid,rho0*Cp*v*grad(dof(T))*tf(T))
convec += integral(fluid,rho0*Cp*div(v)*dof(T)*tf(T))
convec += integral(fluidskin,-(-kappa*grad(dof(T)))* -normal(fluid) *tf(T,zflux))

T.setvalue(fluid, 0.0)
dtinit = vec(convec)

eul = impliciteuler(convec, dtinit)

eul.settolerance(1e-3)
eul.setrelaxationfactor(.55)

ts = 0.01
nts = 100

settime(0);

for i in range(nts):
    eul.next(ts,20)
    v.write(fluid, "v" + str(1000 + i) + ".vtu", 3)		
    p.write(fluid, "p" + str(1000 + i) + ".vtu", 3)
    T.write(fluid, "T" + str(1000 + i) + ".vtu", 3)
    
    #criterion.write(fluid, "criterion" + str(1000 + i) + ".vtu", 2)
    #fieldorder(v.compx()).write(fluid, "fieldorderv" + str(1000 + i) + ".vtu", 2)
    
    maxcrit = criterion.max(fluid, 1)[0]
    
    print("@" + str(i) + str(maxcrit) + " (#dofs is " + str(convec.countdofs()))
		
    adapt(1)
