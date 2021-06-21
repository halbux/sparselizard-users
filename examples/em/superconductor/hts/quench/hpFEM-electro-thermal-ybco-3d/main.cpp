// This code illustrates the setup for a nonlinear electro-thermal simulation of a 3D HTS tape.
// The four main layers of interest are taken into account (see 'tape.geo'): Cu-Substrate-HTS-Cu
// with realistic sizes (30 cm long, 5 mm wide, 2 um thick HTS, 20 um thick Cu and 100 um thick substrate).
// A transient analysis is performed from an initial 77 K to a hotspot temperature of 150 K.
// Quench is initiated by bringing a 'heater' portion of the HTS layer initially into its normal zone.
// Material properties are taken from literature and should be realistic in the temperature
// range from  < 4 to > 150 K and for external magnetic fields from about 1 to 10 T.
//
// Thanks to H. Milanchian for the material properties.
// For any possible mistake please blame A. Halbach.
//
// Developed on sparselizard 202106.1 ('discrete dawn').


#include "sparselizard.h"


using namespace sl;

expression sigma_rebco(expression T, expression E, expression B, double theta);
expression resistivity_cu(double RRR, expression T, expression B);
expression cp_cu(expression T);
expression k_cu(double RRR, expression T, expression B);

int main(void)
{
    printversion();

    int botcu = 1, substrate = 2, rebco = 3, topcu = 4, heater = 5, in = 6, out = 7, skin = 8;

    mesh mymesh;
    mymesh.selectskin(skin);
    mymesh.load("tape.msh");
    
    int copper = selectunion({botcu,topcu});
    int tape = selectall();

     // Input current [A]:
    double Iin = 50;
    // Operating temperature [K]:
    double Top = 77;
    // Residual resistivity ratio of copper
    double RRR = 10.0;
    // External magnetic field strength [T] and angle:
    double Bzext = 1, theta = getpi()/2.0;
	
    
    // Electric potential field v [V] and temperature T [K]:
    field v("h1"), T("h1"), x("x"), y("y");

    // Initial interpolation orders before p-adaptivity:
    v.setorder(tape, 1);
    T.setorder(tape, 1);

    // Electric field [V/m]:
    expression E = -grad(v);
    
    // Electric supply ports:
    port V, I;
    v.setport(in, V, I);
    // Ground ports:
    port Vg, Ig;
    v.setport(out, Vg, Ig);

    // Adiabatic everywhere except at the in/out (far away from the quench area).
    // A convection term of the form h*(T - Tfluid) can be easily taken into account in the weak form.
    T.setconstraint(in, Top);
    T.setconstraint(out, Top);
    
    
    expression sigmarebco = sigma_rebco(T, E, Bzext, theta); // [ohm^-1.m^-1] computes the conductivity of rebco

    // Rebco thermal conductivity data (van Nugteren). Range x : T (1[K], 300[K]). y : thermal conductivity [W/(K.m)]
    expression krebco(spline("rebco_k.txt"), T);

    // Rebco specific heat data. Range x : T (1[K], 300[K]). y : specific heat [J/(kg.K)]
    expression cprebco(spline("rebco_cp.txt"), T);

    // Copper:
    expression rcu = resistivity_cu(RRR, T, Bzext);  // [ohm.m] computes the resistivity of Copper
    expression kcu = k_cu(RRR, T, Bzext);            // [W / (K.m)] computes the thermal conductivity of Copper
    expression cpcu = cp_cu(T);                  // [J / (kg.K)] computes the heat capacity of Copper

    // Substrate (Hastelloy) properties are extracted from J. Lu (2008) paper
    // all resistivity, k & cp are measured based on T variations
    // extracted data are loaded in text files

    // Hastelloy resistivity data. Range x : T (3.960[K], 381.498[K]) y : resistivity [ohm.m]
    expression rsub(spline("hastelloy_r.txt"), T);

    // Hastelloy thermal conductivity data. Range x : T (2.347[K], 198.826[K]). y : thermal conductivity [W/(K.m)]
    expression ksub(spline("hastelloy_k.txt"), T);

    // Hastelloy specific heat data. Range x : T (2.159[K], 303.625[K]). y : specific heat [J/(kg.K)]
    expression cpsub(spline("hastelloy_cp.txt"), T);


    // Parameters sigma [S/m], k [W/(m*K)], cp [J/(kg*K)], rho [kg/m3];
    parameter sigma, k, cp, rho;
    
    sigma|rebco     = sigmarebco;
    sigma|copper    = 1.0/rcu;
    sigma|substrate = 1.0/rsub;

    k|rebco     = krebco;
    k|copper    = kcu;
    k|substrate = ksub;

    cp|rebco     = cprebco;
    cp|copper    = cpcu;
    cp|substrate = cpsub;

    rho|rebco     = 6300;
    rho|copper    = 8960;
    rho|substrate = 7850;
	
    expression j = sigma*E;
    
    // For visualization:
    expression(1).write(heater, "heater.vtu", 1);
    
    
    // P-adaptivity settings (see doc):
    int maxTorder = 3, maxVorder = 3;
    T.setorder(0.001, 1, maxTorder, 0);
    v.setorder(0.001, 1, maxVorder, 1e-4);
    // H-adaptivity settings (see doc):
    expression hcrit = ifpositive(fieldorder(T) - (maxTorder-0.5), 1, 0);
    mymesh.setadaptivity(hcrit, 0, 1);
    
    
    // Formulation:
    formulation heattransient;

    heattransient += integral(tape, predefineddiffusion(dof(T), tf(T), k, rho*cp) );
    // Heating from the current flow.
    // Note: due to the limited ~16 digits accuracy of 'double' and the large range of
    // electric conductivities (from 1 up to 1e15 max limit in the HTS) there is a very tiny
    // but non-zero current flowing in the other layers than the HTS, even at very low temperatures (< 10 K).
    // This might cause a very slow artificial heating for very large supply currents and very low temperatures.
    heattransient += integral(tape, -j*E*tf(T) );

    formulation currentflow;
    
    currentflow += I - Iin; // Supply current, will flow through the HTS
    currentflow += Vg - 0.0;
    
    currentflow += integral(tape, -sigma * grad(dof(v)) * grad(tf(v)));
    
    
    // Initial tape temperature:
    T.setvalue(tape, Top);
    
    
    // Quench is initiated.
    // Note: parameter values can be reset here because parameter expressions are evaluated at
    // runtime unlike any other expression which is fixed in its symbolic form when used.
    sigma|heater = 1000; // a pretty poor conductor
    
    
    wallclock clktot;
    
    genalpha ga(heattransient, vec(heattransient), vec(heattransient), 3);
    
    // Staggered electro-thermal resolution:
    ga.presolve({currentflow});
    
    // Lowest possible numerical damping:
    ga.setparameter(1);
    
    // Time-adaptivity: lower the first argument for a finer timestepping.
    ga.setadaptivity(0.04, 1e-3, 500e-3);
    
    double Tmax = -1;
    std::vector<double> allTmax = {};
    std::vector<double> alldeltav = {};
    
    settime(0);
    while (Tmax < 150)
    {
        wallclock clk;
        ga.next(-1, -1);
        
        T.write(tape, "T"+std::to_string(ga.count())+".vtu", maxTorder);
        grouptimesteps("T.pvd", "T", 1, ga.gettimes());
    
        T.write(skin, "Tskin"+std::to_string(ga.count())+".vtu", maxTorder);
        grouptimesteps("Tskin.pvd", "Tskin", 1, ga.gettimes());
        
        j.write(tape, "j"+std::to_string(ga.count())+".vtu", maxVorder);
        grouptimesteps("j.pvd", "j", 1, ga.gettimes());
        
        v.write(tape, "v"+std::to_string(ga.count())+".vtu", maxVorder);
        grouptimesteps("v.pvd", "v", 1, ga.gettimes());
    
        fieldorder(T).write(skin, "foT"+std::to_string(ga.count())+".vtu", 1);
        grouptimesteps("foT.pvd", "foT", 1, ga.gettimes());
        
        fieldorder(v).write(skin, "fov"+std::to_string(ga.count())+".vtu", 1);
        grouptimesteps("fov.pvd", "fov", 1, ga.gettimes());
    
        Tmax = T.max(tape,5)[0];
        double deltav = v.max(in, 5)[0];
        
        std::cout << "--> Tmax = " << Tmax << " K | dv = " << 1e3*deltav << " mV | #dofs = " << heattransient.countdofs() << std::endl;
        
        allTmax.push_back(Tmax);
        alldeltav.push_back(deltav);
        
        writevector("time.csv", ga.gettimes());
        writevector("Tmax.csv", allTmax);
        writevector("deltav.csv", alldeltav);

        // Hp-adaptivity based on the settings defined previously:
        adapt(0);
    }
    
    clktot.print("Total computation time:");

}

expression sigma_rebco(expression T, expression E, expression B, double theta)
{
    // This function computes rebco's sigma [ohm^-1.m^-1]
    // input: T [K],  E,  B [T]

    double g0 = 0.03, g1 = 0.25, g2 = 0.06, g3 = 0.058,
           Jc0_c = 8.034, Tstar_c = 30.35, m1 = 0.2983, n1 = 1.511,
           Jc0_ab = 4.401, Tstar_ab = 43.27, c = 0.7698, n2 = 2.207, h = -0.01107, p = -0.177;
    double v_ = 1.85;

    // Jc dependecy of perpedicular magnetic field
    expression Jc_c = Jc0_c* pow(10.0, 5.0)* exp(-pow(T/Tstar_c, n1))* exp(-pow(B, m1));

    // Jc dependency of parallle magnetic field
    expression Jc_ab = Jc0_ab* pow(10.0, 5.0)* exp(-pow(T/Tstar_ab, n2))* pow((B + c), (h*T+p));

    // Anisotropy factor
    expression g = g0 + g1 * exp(-B * g2 * exp(T*g3));

    // Critical current density in the input temperature T,...
    // magnetic field B and field orientation theta

    expression Jc = min(Jc_c, Jc_ab) + max(0, Jc_ab-Jc_c) / pow((1.0 + abs(theta - getpi()/2)/g), v_);

    Jc = 1e6 * Jc;

    //J = Jc/Ec^(1/n) * norm(E)^((1-n)/n) * E
    //    |_______________________________|
    //                sigma(E)

    double n = 4.0, Ec = 1e-4;
    expression sigmabounded = Jc/( pow(Ec,1.0/n) ) * pow( norm(E) + 1e-50, (1.0-n)/n );

    sigmabounded = ifpositive( abs(sigmabounded) - 1e15, 1e15, sigmabounded );
    sigmabounded = ifpositive( 1.0 - abs(sigmabounded), 1.0, sigmabounded );

    return sigmabounded;
}


expression resistivity_cu(double RRR, expression T, expression B)
{
    // This function computes Copper resitivity [ohm.m]
    // input: RRR,  T [K],  B [T]

    double P1 = 1.171e-17, P2 = 4.49,  P3 = 3841e7, P4 = 1.14,
           P5 = 50.0,      P6 = 6.428, P7 = 0.4531;

    double a0 = -2.662, a1 = 0.3168, a2 = 0.6229, a3 = -0.1839, a4 = 0.01827;

    double rho0 = 1.72e-8 / RRR;
    expression rhoi = P1 * pow(T, P2) / (1 + P1 * P3 * pow(T, P2-P4) * exp(-pow((P5/T), P6))),
           rhoi0 = P7 * (rhoi*rho0) / (rho0 + rhoi),
           rhoN_T_RRR = rho0 + rhoi + rhoi0;

    expression x_B = 1.72e-8 / rhoN_T_RRR * B,
               a_x = a0 + a1*log10(x_B) + a2*pow(log10(x_B), 2)
                        + a3*pow(log10(x_B), 3) + a4*pow(log10(x_B), 4);

    expression rhoN_T_RRR_B = rhoN_T_RRR * (1 + pow(10, a_x));

    return rhoN_T_RRR_B;

}
expression k_cu(double RRR, expression T, expression B)
{
  // This function computes Copper thermal conductivity [W / (K.m)]
  // input: RRR,  T [K],  B [T]

    double betha = 0.634/RRR,
           bethar = betha/0.0003;

    double P1 = 1.754e-8, P2 = 2.763, P3 = 1102, P4 = -0.165,
           P5 = 70,       P6 = 1.756, P7 = 0.838/pow(bethar,0.1661);

    expression W0 = betha/T,
           Wi = P1*pow(T, P2) / (1 + P1*P3* pow(T, P2 + P4)* exp(-pow(P5/T, P6))),
           Wi0 = P7* (Wi*W0)/(Wi+W0),
           kN_T_RRR = 1 / (W0 + Wi + Wi0);

    expression rhoN_T_RRR_B0 = resistivity_cu(RRR, T, 0.5); // @ B = 0 [T]

    expression kN_T_RRR_B = rhoN_T_RRR_B0 * kN_T_RRR / resistivity_cu(RRR, T, B);

    return kN_T_RRR_B;

}

expression cp_cu(expression T)
{    
    // This function computes Copper specific heat [J/(kg.K)]
    // input: T [K]

    double a = -1.91844, b = -0.15973, c = 8.61013, d = -18.996,
           e =  21.9661, f = -12.7328, g = 3.54322, h = -0.3797;

    expression cp = pow(10, a + b*log10(T) + c*pow(log10(T), 2) + d*pow(log10(T), 3)
                          + e*pow(log10(T), 4) + f*pow(log10(T), 5) + g*pow(log10(T), 6)
                          + h*pow(log10(T), 7) );
    return cp;
}

