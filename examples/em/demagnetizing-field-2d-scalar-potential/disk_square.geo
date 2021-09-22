Mesh.CharacteristicLengthFactor = 0.25;

SetFactory("OpenCASCADE");

rdom = 1;
rdisk = 0.2;

Disk(1) = {0,0,0,rdom};

Rectangle(2) = {-rdisk/Sqrt(2), -rdisk/Sqrt(2), 0, rdisk*Sqrt(2), rdisk*Sqrt(2)};

Coherence;

Physical Curve(1) = {1}; // external boundary
Physical Surface(2) = {2}; // material
Physical Surface(3) = {3}; // air