#include "sparselizard.h"
#define VACUUM_PERMITTIVITY 8.8541878128e-12
#define SPEED_LIGHT 299792458
#define COPPER_RESISTIVITY 1.68e-8

using namespace sl;


int main(void)
{	
    int plane = 1, track = 2, dielectric = 3, air = 4;
    mesh mymesh("microstrip.msh");

    field V( "h1", { 1 } );
    field A( "hcurl", {1 } );
    
    parameter epsilon;
    
    int dielectrics = selectunion( { dielectric, air } );
    int conductors = selectunion( { track, plane } );
    int wholedomain = selectall();
    
    epsilon | air = VACUUM_PERMITTIVITY * 1;
    epsilon | dielectric = VACUUM_PERMITTIVITY * 1;
    
    V.setorder(wholedomain, 2);
    A.setorder(wholedomain, 2);
    
    // First simulation to get C whithout dielectric
    formulation equations1;
    
    expression E = -grad( V );

    // Put a 1 nC charge on the track
    port voltage, charge;
    V.setport(track, voltage, charge);
    
    equations1 += charge + 1e-9;
    // Reference plane is 0V
    V.setconstraint( plane, 0 );
    // Gauss law
    equations1 += integral( dielectrics, epsilon * grad( dof( V ) ) * grad( tf( V ) ) );
    // Gauss law
    equations1.solve();
    
    double V0 = voltage.getvalue();
    epsilon | dielectric = VACUUM_PERMITTIVITY * 4.4;
    
    // Second simulation to get C with dielectric and thus the effective permittivity
    equations1.solve();
    
    double V1 = voltage.getvalue();
    double epsilon_eff = V0/V1;
    double C = -charge.getvalue()/V1;
    double Z = sqrt( epsilon_eff ) /  ( C *SPEED_LIGHT );

    std::cout << "Effective relative permittivity : " << epsilon_eff << std::endl;
    std::cout << "Impedance : " << Z << std::endl;
    V.write( wholedomain, "V.pos", 2 );
    
    // Code validation line. Can be removed.
    std::cout << "Impedance correct ? " << ((std::abs(Z - 66.559747)/66.559747 < 0.01 ) ? "OK": "NOT OK" ) << std::endl;
    // Wheeler equation was used to determine the reference value.
    // The Wheeler equation is usually less than 1%, always less than 2%
}

