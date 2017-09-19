# -*- coding: utf-8 -*-
"""
Created on Tue Sep 19 17:11:27 2017

Transfer Matlab script RobotDynamic.m(Coded by Chrislu) to python.
Robotic arm moving from yp = -70 to 70, from zp = 270 to 105( => zp - 165).

Variables' differences:
    RobotDynamic.m      J[i]    Jtemp4  J4
    RobotDynamic.py     J[i-1]  Jtemp3  j3
    
@author: Danny
"""
import math;
import sympy;

DegreePrecision = 2;    # Degree's rounding precision. ('n' for no decimal.)

f = open('../TempData/PyAngle1.txt', 'w');

#***********************Input( Needed position )**************************
xp = 300;
yp = 0;
zp = 270;

#***********************Robotic arm's fixed data**************************
deltaz = 0;
deltay = 4.5;
r1 = 99;
r2 = 159;
r3 = 104.5;
r4 = 93.535;

Jtemp3 = 0;  #Store previous J[3] (Seems to be useless?)

#***********************Angle Calculations******************

for yp in range(-70, 71):    #yp ranging from -70 to 70
    
    J = [0, 0, 0, 0, 0, 0];  #each servo's angle
    
    J[0] = math.asin((deltay + yp)/r1);
    J[1] = -J[0];
    
    parallel = 1;            #Parrellel to the floor
    
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
    
    J[2] = 1.5707 - b1 - theta;
    
    j3 = sympy.symbols('j3');
    eqs = ( 159 * sympy.sin(J[2]) + 104.5 * sympy.sin(1.5707 - j3 + sympy.Abs(J[2])) - a1 );
    j3Result = sympy.solve( eqs, j3 );


    # Seems to be useless?
    '''
    if abs( j3Result[1] - Jtemp3 ) < 0.1:
        J[3] = sympy.re(j3Result[1]);
    else:
        J[3] = sympy.re(j3Result[0]);
    '''
    J[3] = sympy.re(j3Result[0]);   # Just need this result.
    
    Jtemp3 = J[3];  # Seems to be useless?
    
    if parallel:
        J[4] = J[3] - J[2];
    if not parallel:
        J[4] = J[3] - J[2] + math.pi/3;
    
    if DegreePrecision == 'n':
        degree = [ round(math.degrees(d)) for d in J ];
    else:
        degree = [ round(math.degrees(d), DegreePrecision) for d in J ];

    f.write(",".join(str(d) for d in degree)+",\n");


f.write("*********************\n");


yp = 0;
Jtemp3 = 0;

for i in range( zp, zp-166, -1 ):        # zp ranging from zp(270) to zp-165(105)
    J = [0, 0, 0, 0, 0, 0];  #each servo's angle
    
    J[0] = math.asin((deltay + yp)/r1);
    J[1] = -J[0];
    
    parallel = 1;            #Parrellel to the floor
    
    if parallel:
        deltaz = 134.2 - 20;
        rx = xp - r4 - r1 * math.cos( J[1] ); 
        rz = i - deltaz;     ##Difference from yp's calculation.
    else:
        pass;
    
    r = math.sqrt( math.pow(rx, 2) + math.pow(rz, 2) );
    theta = math.atan( rz/rx );
    a1 = r * math.cos( theta );
    b1 = math.acos(( math.pow(r, 2) + math.pow(r2, 2) - math.pow(r3, 2)) / ( 2*r2*r ) );

    J[2] = 1.5707 - b1 - theta;
    
    j3 = sympy.symbols('j3');
    ##Difference from yp's calculation.
    eqs = ( 159 * sympy.sin(J[2]) + 104.5 * sympy.sin(1.5707 + j3 - sympy.Abs(J[2])) - a1 );
    j3Result = sympy.solve( eqs, j3 );


    # Seems to be useless?
    '''
    if abs( j3Result[1] - Jtemp3 ) < 0.1:
        J[3] = sympy.re(j3Result[1]);        
    else:
        J[3] = sympy.re(j3Result[0]);
    '''

    J[3] = sympy.re(j3Result[0]);    # Just need this result.
    
    
    Jtemp3 = J[3]; # Seems to be useless?
    
    if parallel:
        J[4] = J[3] - J[2];
    if not parallel:
        J[4] = J[3] - J[2] + math.pi/3;
    
    if DegreePrecision == 'n':
        degree = [ round(math.degrees(d)) for d in J ];
    else:
        degree = [ round(math.degrees(d), DegreePrecision) for d in J ];
        
    f.write(",".join(str(d) for d in degree)+",\n");

f.close();
