/*
     Possibilité de création de spirales et autres formes complexes
  */
  // delta permet de s'éloigner de l'axe progressivement
  // yScale permet d'agrandir la forme 2d selon axe Y
  // xScale permet d'agrandir la forme 2d selon axe X
  helicoidal_extrude(angle =3600, convexity = 2, $fs=20, step=10, delta=15, $fn=200, xOffset=40,  yScalEnd=1.8, xScalEnd = 3) {
          rotate([0,0,-20])  square([30,2], center = true);
   }
   
   translate([500,0,0])
   helicoidal_extrude(angle =3600, convexity = 2, $fs=20, step=5, delta=10, $fn=200, xOffset=40, xScalEnd = 3) {
          square([20,2], center = true);
   }
   
   // Spirale dans un plan avec step=0 ...
   translate([0,500,0])
   helicoidal_extrude(angle =3600, convexity = 2, $fs=20, delta=40, $fn=200, xOffset=40, xScalEnd = 4) {
          rotate([0,0,45])square([4,4], center = true);
   }