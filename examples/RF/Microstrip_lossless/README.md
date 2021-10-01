 
# Impedance of a lossless microstrip transmision line

A microstrip is defined a as track over a reference plane. In between, there is a dielectric with a relative permittivity that is usually not 1.
The impedance of any lossless transmission line Z_0 depends on L and C, respectively the unit inductance and capacitance per unit length. 

![](https://latex.codecogs.com/gif.latex?Z_0&space;=&space;\sqrt{&space;\frac{L}{C}&space;})

The capacitance is easily found using sparselizard by placing a known charge on the track, and computing the voltage:

![](https://latex.codecogs.com/gif.latex?C&space;=&space;\frac{Q}{V})

The inductance is linked to the speed of electromagnetic waves for that transmission line

![](https://latex.codecogs.com/gif.latex?\frac{c}{\sqrt{\epsilon_{eff}}}&space;=&space;\frac{1}{\sqrt{&space;L&space;C&space;}&space;})

( with c being the speed of light in vacuum )

In order to find the effective, we need to run the simulation with and without dielectric, as it is defined as the change of voltage ( or change of capacitance )

A scilab validation code is provided and is based on Wheeler's model. That model is not exact: the error in impedance is usually less than 1%, but always less than 2%.
