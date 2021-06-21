
l = 301e-3; lh = 5e-3; w = 5e-3;

thbotcu = 20e-6; thtopcu = 20e-6; thsubstr = 100e-6; threbco = 2.0e-6;

botcu = 1; substrate = 2; rebco = 3; topcu = 4; heater = 5; in = 6; out = 7;

Mesh.CharacteristicLengthMax = 1.5e-3;

SetFactory("OpenCASCADE");

Box(1) = {0,0,0, l,w,thbotcu};
Box(2) = {0,0,thbotcu, l,w,thsubstr};
Box(3) = {0,0,thbotcu+thsubstr, l,w,threbco};
Box(4) = {0,0,thbotcu+thsubstr+threbco, l,w,thtopcu};
Box(5) = {0.5*l-0.5*lh,0,0, lh,w,thbotcu+thsubstr+threbco};
    
Coherence;

Physical Volume(botcu) = {5,6,7};
Physical Volume(substrate) = {8,9,10};
Physical Volume(rebco) = {11,12,13};
Physical Volume(topcu) = {4};
Physical Volume(heater) = {12};
Physical Surface(in) = {1,17,30,43};
Physical Surface(out) = {14,27,40,47};
