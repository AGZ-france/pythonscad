 /* Construction d'un noeud de mobius en 3D */
 module mobius(r1, r2, nb) {
    if(nb <3) {
        nb = 2;
        helicoidal_extrude(angle =360, convexity = 2, $fn=200,  xOffset=r1, zRotate=360/nb) {
                hull() {
                    translate([-r2,0,0])circle(d=1, $fn=100);
                    translate([ r2,0,0])circle(d=1, $fn=100);
                }
        }        
    } else {
        helicoidal_extrude(angle =360, convexity = 2, $fs=10,  xOffset=r1, zRotate=360/nb) {
                circle(r=r2, $fn=nb);
        }
    }
 }
 
mobius(40, 3, 3, $fn=200);
translate([0,0,20])mobius(30, 4, 6, $fn=100);
translate([0,0,-20])mobius(30, 2, 2, $fn=100);
   