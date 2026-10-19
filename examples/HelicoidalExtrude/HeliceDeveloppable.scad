/*
     Helicoide developpable
     forme très particulière, permettant de découper dans une feuille un disque pouvant se poser à la surface de l'hélice.
     Pour construire une telle forme, il faut que la surface développée dans la rotation, soit tangente au cylindre
  */
  
  // axeRotate permet en plus du cas spécifique de 90° de choisir d'autres angles, dont je n'ai aucune idée de l'interet
      helicoidal_extrude(angle =360, convexity = 2, $fs=20, step=40, $fn=200, xOffset=40, axeRotate=90) {
          rotate([0,0,-20])  square([60,20], center = true);
   }