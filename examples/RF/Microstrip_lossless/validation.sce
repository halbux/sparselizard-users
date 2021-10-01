clear
Z0 = 376.730313668;
h = 0.2e-3;
w = 0.2e-3;
t = 0.035e-3;

er = 4.4;

weff = w + ( t/%pi ) * log( 4 *exp(1) / ( sqrt( ( t/h ).^2 + (t/(w*%pi+1.1*t*%pi ) ).^2 ))) * ( er+1) / (2*er);
X1 = 4*( 14 * er + 8 )/( 11*er )*h/weff;
X2 = sqrt( 16*(h/weff).^2*( ( 14*er+8 )/(11*er)).^2 + ( er +1 )/( 2*er ) * %pi*%pi )

Z = Z0 / ( 2*%pi *sqrt( 2* ( 1 +er ) ) ) * log ( 1 + 4*h/weff *( X1 +X2) )
