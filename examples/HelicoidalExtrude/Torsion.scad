/*
    Torsion en même temps que rotation.
  */
  

      helicoidal_extrude(angle =2*360, step=16,convexity = 2, $fs=20,  $fn=200, xOffset=40, axeRotate=0, zRotate=4*360) {
          circle(r=4, $fn=5);
   }
   
   translate([100,80,0])
 helicoidal_extrude(angle =2*360, step=20,convexity = 2, $fs=20,  $fn=200, xOffset=40, axeRotate=0, zRotate=4*360) {
                union() {
                        translate([2,0])circle(r=4, $fn=5);
                        translate([0,6])circle(r=2, $fn=1);
                 }
   }
   
   
     translate([-100,80,0]) {
     
 color("blue")helicoidal_extrude(angle =2*360, step=20,convexity = 2, $fs=20,  $fn=200, xOffset=40, axeRotate=0, zRotate=4*360) {
                        translate([0,6])circle(r=2, $fn=1);
     }
   
 color("red")  helicoidal_extrude(angle =2*360, step=20,convexity = 2, $fs=20,  $fn=200, xOffset=40, axeRotate=0, zRotate=4*360) {
                        translate([2,0])circle(r=4, $fn=5);
     
   }
   }