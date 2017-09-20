# -*- coding: utf-8 -*-
"""
Created on Tue Sep 19 20:42:20 2017

@author: Danny
"""

import math;

def getArmAngle( xp, yp, zp, eq = 'n', DegreePrecision = 'n' ):

    #***********************Robotic arm's fixed data**************************
    deltaz = 0;
    deltay = 4.5;
    r1 = 99;
    r2 = 159;
    r3 = 104.5;
    r4 = 93.535;

    #Jtemp3 = 0;  # Store previous J[3] (Seems to be useless?)

    #***********************Angle Calculations******************
    
    J = [0, 0, 0, 0, 0, 0];  # each servo's angle
    
    J[0] = math.asin((deltay + yp)/r1);
    J[1] = -J[0];
    
    parallel = 1;            # Parellel to the floor
    
    if parallel:
        deltaz = 134.2 - 20;
        rx = xp - r4 - r1 * math.cos(J[1]);
        rz = zp - deltaz;
    else:
        pass;
        
    r = math.sqrt(math.pow(rx,2) + math.pow(rz,2));
    theta = math.atan( rz/rx );
    a1 = r*math.cos(theta);
    b1 = math.acos((math.pow(r,2) + math.pow(r2,2) - math.pow(r3,2)) / (2*r2*r));
    
    J[2] = math.pi/2 - b1 - theta;
    
    
    if eq == 'n':
        J[3] = J[2] + math.pi/2 - math.asin( ( a1 - 159 * math.sin(J[2]) ) / 104.5 );
    else:
        import sympy;
        j3 = sympy.symbols('j3');    
        eqs = ( 159 * sympy.sin(J[2]) + 104.5 * sympy.sin( math.pi/2 - j3 + J[2] ) - a1 );
        j3Result = sympy.solve( eqs, j3 );    
        J[3] = sympy.re(j3Result[0]);
        
    # Seems to be useless?
    '''
    if abs( j3Result[1] - Jtemp3 ) < 0.1:
        J[3] = sympy.re(j3Result[1]);
    else:
        J[3] = sympy.re(j3Result[0]);
    '''
    
    
    #Jtemp3 = J[3];  # Seems to be useless?
    
    if parallel:
        J[4] = J[3] - J[2];
    if not parallel:
        J[4] = J[3] - J[2] + math.pi/3;
    
    if DegreePrecision == 'n':
         return [ round(math.degrees(d)) for d in J ];
    else:
        return [ round(math.degrees(d), DegreePrecision) for d in J ];
    
    

########################====Main Function====####################
        
'''
xp = 300;
zp = 270;

f = open('../TempData/pySampleData1.txt', 'w');

for yp in range(-70, 71): 
    degree = getArmAngle( xp, yp, zp );
    f.write(",".join(str(d) for d in degree)+", "+ str(yp) + "\n");
f.close();
''' 

xp = 300;
yp = 0;
zp = 270;

f = open('../TempData/pySampleData2.txt', 'w');

for i in range( zp, zp-166, -1 ):
    degree = getArmAngle( xp, yp, i );
    f.write(",".join(str(d) for d in degree)+",\n");
    
f.close();
    
'''
xp = 300;
zp = 270;

f = open('../TempData/.txt', 'w');

for yp in range(-70, 71): 
    degree = getArmAngle( xp, yp, zp );
    f.write(",".join(str(d) for d in degree)+",\n");
    
f.write("*********************\n");

yp = 0;

for i in range( zp, zp-166, -1 ):
    degree = getArmAngle( xp, yp, i );
    f.write(",".join(str(d) for d in degree)+",\n");
    
f.close();
'''

'''
xp = 300;
yp = 70;
zp = 270;

degree = getArmAngle( xp, yp, zp );
print(degree);
'''