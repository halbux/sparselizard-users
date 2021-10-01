

H = 0.2e-3;
W = 0.2e-3;
T = 0.035e-3;
tsmall = T/10;
tbig = H/3;

boxx = 10*W;
boxy = 10*H;

p = newp;
Point(p) = {-boxx, 0,0, tbig};
Point(p+1) = {-boxx, -T,0, tbig};
Point(p+2) = {boxx, -T,0, tbig};
Point(p+3) = {boxx, 0,0, tbig};

l = newl;

Line( l ) = {p, p+1};
Line( l+1 ) = {p+1, p+2};
Line( l+2 ) = {p+2, p+3};
Line( l+3 ) = {p+3, p};

ll = newll;
llplane = ll;
Line Loop(ll) = {l, l+1, l+2, l+3};
s = news;
Plane Surface(s) = {ll};
Physical Surface("PLANE") = {s};
splane = s;

p = newp;
ptrack = p;
Point(p) = {-W/2, H,0,tsmall};
Point(p+1) = {-W/2, H+T,0,tsmall};
Point(p+2) = {W/2, H+T,0,tsmall};
Point(p+3) = {W/2, H,0,tsmall};

l = newl;

Line( l ) = {p, p+1};
Line( l+1 ) = {p+1, p+2};
Line( l+2 ) = {p+2, p+3};
Line( l+3 ) = {p+3, p};

ll = newll;
lltrack = ll;
Line Loop(ll) = {l, l+1, l+2, l+3};
s = news;
Plane Surface(s) = {ll};
Physical Surface("TRACK") = {s};
strack = s;

p = newp;
pdiel = p;
Point(p) = {-boxx, 0,0, tbig};
Point(p+1) = {-boxx, H,0, tbig};
Point(p+2) = {-W/2, H,0, tsmall};
Point(p+3) = {W/2, H,0, tsmall};
Point(p+4) = {boxx, H,0, tbig};
Point(p+5) = {boxx, 0,0, tbig};

l = newl;

Line( l ) = {p, p+1};
Line( l+1 ) = {p+1, p+2};
Line( l+2 ) = {p+2, p+3};
Line( l+3 ) = {p+3, p+4};
Line( l+4 ) = {p+4, p+5};
Line( l+5 ) = {p+5, p};

ll = newll;
lldielectric = ll;
Line Loop(ll) = {l, l+1, l+2, l+3, l+4, l+5};
s = news;
Plane Surface(s) = {ll};
Physical Surface("DIELECTRIC") = {s};
sdielectric= s;


p = newp;

Point(p) = {-boxx, boxy,0, tbig};
Point(p+1) = {-boxx, H,0, tbig};
Point(p+2) = {-W/2, H,0, tsmall};
Point(p+3) = {-W/2, H+T,0, tsmall};
Point(p+4) = {W/2, H+T,0, tsmall};
Point(p+5) = {W/2, H,0, tsmall};
Point(p+6) = {boxx, H,0, tbig};
Point(p+7) = {boxx, boxy,0, tbig};

l = newl;

Line( l ) = {p, p+1};
Line( l+1 ) = {p+1, p+2};
Line( l+2 ) = {p+2, p+3};
Line( l+3 ) = {p+3, p+4};
Line( l+4 ) = {p+4, p+5};
Line( l+5 ) = {p+5, p+6};
Line( l+6 ) = {p+6, p+7};
Line( l+7 ) = {p+7, p};

ll = newll;
Line Loop(ll) = {l, l+1, l+2, l+3, l+4, l+5, l+6, l+7};
s = news;
sair =s;
Plane Surface(s) = {ll};
Physical Surface("AIR") = {s};


