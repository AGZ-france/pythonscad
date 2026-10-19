# /* Construction d'un noeud de mobius en 3D */
from pythonscad import *
def mobius(r1, r2, nb, fn=100) :
    if(nb <3):
        nb = 2;
        return hull(
                translate(circle(d=1, fn=100),[-r2,0,0]),
                translate(circle(d=1, fn=100),[ r2,0,0])
            ).helicoidal_extrude(angle =360, convexity = 2, fn=200,  xOffset=r1, zRotate=360/nb) 
    else:
        return circle(r=r2, fn=nb).helicoidal_extrude(angle =360, convexity = 2, fs=10,  xOffset=r1, zRotate=360/nb)
 
 
show(mobius(40, 3, 3, fn=200),translate(mobius(30, 4, 6, fn=100), [0,0,20]), translate(mobius(30, 2, 2, fn=100),[0,0,-20]))
   