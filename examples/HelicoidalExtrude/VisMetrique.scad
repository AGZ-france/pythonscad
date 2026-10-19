/* Informations obtenus depuis IA:
Metrique
Angle du profil : 60°
    Pas = P
    Hauteur du triangle fondamental :  H=0,866×P
    Profondeur réelle du filet :       h=0,6134×P
    Méplat au sommet :                   0,125×P
 
Formules pratiques

Pour un filetage mâle :
    Ø extérieur = D
    Ø au fond du filet : Dfond=D−2h

Exemple concret – M33 × 2,5

(Approche proche d’un 1" BSP mais en métrique)
    D = 33 mm
    P = 2,5 mm

Calculs :
    H = 0,866  × 2,5 = 2,165 mm
    h = 0,6134 × 2,5 = 1,533 mm

Donc :
    Ø fond filet =  33−(2×1,533)=29,93mm

Ajustements PETG

Le métrique est plus simple à imprimer que le BSP.

Filetage mâle PETG Réduire Ø extérieur de 0,15 mm
    Garder profondeur théorique
    Ajouter chanfrein entrée 1 mm

Filetage femelle PETG Augmenter Ø nominal de +0,2 à +0,3 mm

Tableau rapide des profondeurs utiles
Pas (P)	Profondeur filet (h)
    1,5 mm	0,92 mm
    2,0 mm	1,23 mm
    2,5 mm	1,53 mm
    3,0 mm	1,84 mm

Diamètre (M)	Pas standard (gros)	Pas fins courants
    M3	0,5 mm	—
    M4	0,7 mm	—
    M5	0,8 mm	—
    M6	1,0 mm	0,75 mm
    M8	1,25 mm	1,0 mm
    M10	1,5 mm	1,25 / 1,0 mm
    M12	1,75 mm	1,5 / 1,25 mm
    M14	2,0 mm	1,5 mm
    M16	2,0 mm	1,5 mm
    M18	2,5 mm	2,0 / 1,5 mm
    M20	2,5 mm	2,0 / 1,5 mm
    M22	2,5 mm	2,0 / 1,5 mm
    M24	3,0 mm	2,0 mm
    M27	3,0 mm	2,0 mm
    M30	3,5 mm	2,0 mm
    M33	3,5 mm	2,0 mm
    M36	4,0 mm	3,0 mm
    M39	4,0 mm	3,0 mm
    M42	4,5 mm	3,0 mm
    M45	4,5 mm	3,0 mm
    M48	5,0 mm	3,0 mm
    M52	5,0 mm	4,0 mm
    M56	5,5 mm	4,0 mm
    M60	5,5 mm	4,0 mm
    M64	6,0 mm	4,0 mm

filetage	Clé (s)	Hauteur tête vis (k)	Hauteur écrou (m)
    M3	5,5 mm	2,0 mm	2,4 mm
    M4	7 mm	2,8 mm	3,2 mm
    M5	8 mm	3,5 mm	4,0 mm
    M6	10 mm	4,0 mm	5,0 mm
    M8	13 mm	5,3 mm	6,5 mm
    M10	17 mm	6,4 mm	8,0 mm
    M12	19 mm	7,5 mm	10,0 mm
    M14	22 mm	8,8 mm	11,0 mm
    M16	24 mm	10,0 mm	13,0 mm
    M18	27 mm	11,5 mm	15,0 mm
    M20	30 mm	12,5 mm	16,0 mm
    M22	32 mm	14,0 mm	18,0 mm
    M24	36 mm	15,0 mm	19,0 mm
    M27	41 mm	17,0 mm	22,0 mm
    M30	46 mm	18,7 mm	24,0 mm
    M33	50 mm	21,0 mm	26,0 mm
    M36	55 mm	22,5 mm	29,0 mm
modéliser un hexagone :
    Distance sur plats = s
    Distance pointe à pointe :        e=s / cos⁡(30°)≈1,155×s

Exemple M10 :
    s = 17 mm
    e ≈ 19,6 mm

Pour impression 3D (PETG)

Pour que la clé rentre bien :
    Augmenter s de +0,2 mm
    Chanfrein 0,5 mm sur arêtes
    4 périmètres minimum si couple important

    Taille 	D1 (mm) 	P (mm)
        M3 	    3 	0,5
        M4 	    4 	0,7
        M5 	    5 	0,8
        M6 	    6 	1
        M8 	    8 	1,25
        M10 	10 	1,5
        M12 	12 	1,75
        M14 	14 	2
        M16 	16 	2
        M18 	18 	2,5
        M20 	20 	2,5
        M22 	22 	2,5
        M24 	24 	3
        M27 	27 	3
        M30 	30 	3,5
        M36 	36 	4
        M42 	42 	4,5
*/
tTypes = [ 
   [ 3, 0.5 ],   [ 4, 0.7 ],   [ 5, 0.8 ],   [ 6, 1.0 ],   [ 8, 1.25],
   [10, 1.5 ],   [12, 1.75],   [14, 2.0 ],   [16, 2.0 ],   [18, 2.5 ],
   [20, 2.5 ],   [22, 2.5 ],   [24, 3.0 ],   [27, 3.0 ],   [30, 3.5 ],
   [33, 3.5 ],   [36, 4.0 ],   [39, 4.0 ],   [42, 4.5 ],   [45, 4.5 ],
   [48, 5.0 ],   [52, 5.0 ],   [56, 5.5 ],   [60, 5.5 ],   [64, 6.0 ]];
   
function GetPasMetrique(diam) = lookup(diam,tTypes);

module trapezeA(h, l) {
    polygon([[0,-l/2-0.05], [h, -0.05], [h, 0.05], [0, l/2+0.05] ]);
}

module trapezeB(h, l) {
    polygon([[0,-0.05], [h, -l/2-0.05], [h, l/2+0.05], [0, 0.05]]);
}

module filetage(hauteur, dMajor, pas, bExtern) {
    rMajor = dMajor/2;
    hP = 0.5413*pas; // 5/8 * sqrt(3/2)
    l = 5*pas / 8;
    if(bExtern) {
       a = 360*(hauteur-pas)/pas;
       rH = rMajor-hP;
       union() {
          translate([0,0,pas/2])
                helicoidal_extrude(angle =a, step=pas, $fn=100, xOffset=rH) {trapezeA(hP, l); }
          color("red")rotate([0,0,360*(hauteur-pas)/pas]) translate([0,0,hauteur-pas/2])
          helicoidal_extrude(angle=120, convexity = 2, $fs=20, step=pas, $fn=60, xOffset=rH, xScalEnd=0.2, yScalEnd=0.1) {
                trapezeA(hP, l);
            }
            
          color("blue")translate([0,0,pas/2])
          helicoidal_extrude(angle = -120, convexity = 2, $fs=20, step=-pas, $fn=60, xOffset=rH, xScalEnd=0.2, yScalEnd=0.1) {
                trapezeA(hP, l);
            }        
            cylinder(h=hauteur, r=rH, $fn=300);
      }
    } else {
        a = 360*(hauteur+0.1-pas)/pas;
        rH = rMajor+0.15-hP;
        translate([0,0,-0.1])
        difference() {
            cylinder(h=hauteur+l, r=rMajor+0.15, $fn=300);
            translate([0,0,pas/2]) 
                helicoidal_extrude(angle =a, step=pas, $fn=300, xOffset=rH) {trapezeB(hP, l);}
          color("red")rotate([0,0,360*(hauteur-pas)/pas]) translate([0,0,hauteur-pas/2])
          helicoidal_extrude(angle=120, convexity = 2, $fs=20, step=pas, $fn=60, xOffset=rH, xOffsetEnd=rH+hP*0.9,xScalEnd=0.2, yScalEnd=0.1) {
                trapezeB(hP, l);
            }
            
          color("blue")translate([0,0,pas/2])
          helicoidal_extrude(angle = -120, convexity = 2, $fs=20, step=-pas, $fn=60, xOffset=rH, xOffsetEnd=rH+hP*0.9, xScalEnd=0.2, yScalEnd=0.1) {
                trapezeB(hP, l);
            }        
         }

    }
       
    
}

module filetMetrique(diam, hauteur, bMale = true) {
    pas = GetPasMetrique(diam);
    filetage(hauteur, diam, pas, bMale);
}
tSizes=[
//  diam, s, 
   [3	, 5.5],   [4	,  7 ],   [5	,  8 ],
   [6	, 10 ],   [8	, 13 ],   [10	, 17 ],
   [12	, 19 ],   [14	, 22 ],   [16	, 24 ],
   [18	, 27 ],   [20	, 30 ],   [22	, 32 ],
   [24	, 36 ],   [27	, 41 ],   [30	, 46 ],
   [33	, 50 ],   [36	, 55 ]];
 tHvis=[
//  diam,  Vis
   [3	,  2.0],   [4	,  2.8],   [5	,  3.5],
   [6	,  4.0],   [8	,  5.3],   [10	,  6.4],
   [12	,  7.5],   [14	,  8.8],   [16	, 10.0],
   [18	, 11.5],   [20	, 12.5],   [22	, 14.0],
   [24	, 15.0],   [27	, 17.0],   [30	, 18.7],
   [33	, 21.0],   [36	, 22.5]];
 tHecrou=[
//  diam, Ecrou
   [3	,  2.4],   [4	,  3.2],   [5	,  4.0],
   [6	,  5.0],   [8	,  6.5],   [10	,  8.0],
   [12	, 10.0],   [14	, 11.0],   [16	, 13.0],
   [18	, 15.0],   [20	, 16.0],   [22	, 18.0],
   [24	, 19.0],   [27	, 22.0],   [30	, 24.0],
   [33	, 26.0],   [36	, 29.0]];
    
module vis(diam, hauteur) {
    dHexa=lookup(diam, tSizes);
    hVis=lookup(diam, tHvis);
    union() {
       cylinder(h=hVis, d=dHexa*1.155, $fn=6);
       translate([0,0,hVis]) {
            filetMetrique(diam, hauteur, true);
       }
    }
}


module ecrou(diam) {
    dHexa=lookup(diam, tSizes);
    hEcrou=lookup(diam, tHecrou);
    difference() {
        cylinder(h=hEcrou,  d=dHexa*1.155, $fn=6);
        filetMetrique(diam, hEcrou, false);
    }
 
}
