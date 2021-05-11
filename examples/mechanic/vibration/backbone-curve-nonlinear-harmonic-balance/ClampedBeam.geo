sc = 0.03;
Point(1) = {0, 0, 0, sc};
Point(2) = {0.5, 0, 0, sc};
Point(3) = {0.5, 0.015, 0, sc};
Point(4) = {0.5, 0.03, 0, sc};
Point(5) = {0, 0.03, 0, sc};
Point(6) = {0, 0.015, 0, sc};
Point(7) = {1.0, 0, 0, sc};
Point(8) = {1.0, 0.015, 0, sc};
Point(9) = {1.0, 0.03, 0, sc};
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 1};
Line(7) = {6, 3};

//+
Line(8) = {2, 7};
//+
Line(9) = {7, 8};
//+
Line(10) = {8, 9};
//+
Line(11) = {9, 4};
//+
Line(12) = {3, 8};
//+
Curve Loop(1) = {4, 5, 7, 3};
//+
Plane Surface(1) = {1};
//+
Curve Loop(2) = {7, -2, -1, -6};
//+
Plane Surface(2) = {2};
//+
Curve Loop(3) = {12, -9, -8, 2};
//+
Plane Surface(3) = {3};
//+
Curve Loop(4) = {12, 10, 11, -3};
//+
Plane Surface(4) = {4};
//+
Extrude {0, 0, 0.015} {
  Point{1}; Point{9}; Point{8}; Point{7}; Point{6}; Point{5}; Point{4}; Point{3}; Point{2}; Curve{4}; Curve{12}; Curve{11}; Curve{10}; Curve{9}; Curve{8}; Curve{7}; Curve{6}; Curve{5}; Curve{3}; Curve{2}; Curve{1}; Surface{4}; Surface{3}; Surface{2}; Surface{1}; Layers{2}; Recombine;
}
//+
Extrude {0, 0, 0.015} {
  Curve{30}; Curve{34}; Curve{26}; Curve{38}; Curve{42}; Point{13}; Point{12}; Point{11}; Curve{58}; Curve{62}; Surface{113}; Surface{91}; Point{16}; Point{17}; Point{18}; Curve{46}; Curve{22}; Curve{66}; Surface{135}; Surface{157}; Curve{50}; Curve{54}; Point{15}; Point{14}; Point{10}; Layers{2}; Recombine;
}
//+
Physical Volume(1) = {5, 6, 2, 1, 7, 8, 3, 4};
//+
Physical Surface(2) = {276, 57, 262, 53, 165, 173, 41, 37};
//+
Physical Point(3) = {17};
