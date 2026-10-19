
lgB = 45;   // Longeur totale du bouton
ep = 12;    // Diametre central du bouton
nb=6;       // Nombre de bordures
dC=2.8;     // diametre bordure helicoidale

rX=ep/2;
alpha = 360/nb;
hZC = 8; // hauteur Zone Centrale (multiple de 2 pour les trous !)
hL  = (lgB-hZC)/2;
stepH = 4*hL;
beta = 360/stepH;

module A() {
    circle(d=dC, $fn=100);
}

module B() {
    helicoidal_extrude($fn=100, xOffset=rX, step=stepH, angle=90, xOffsetEnd=rX/2, xRotate=90, xScalEnd=0.5, yScalEnd=0.5) { A();}
}

module C() {
    for(i=[0:nb-1]) {
        rotate([0,0,i*alpha]){
            translate([0,0, hZC/2]) rotate([0,0,hZC*beta])B();
            translate([0,0,-hZC/2])helicoidal_extrude($fn=100, xOffset=rX, step=stepH, angle=hZC*beta,  xRotate=90) { A();}
            translate([0,0,-hZC/2])rotate([180,0,0]){B();}
        }
    }
}
module D() {
    x1 = rX*tan(alpha);
    x2 = rX/2*tan(alpha);
    rC = rX+dC/2*cos(alpha);
    difference() {
        cylinder(r1=rC, r2=rC/2, h=hL, $fn=100);
        union() {
        for(i=[0:nb-1])
            rotate([0,0,i*alpha+alpha/2])
                helicoidal_extrude($fn=100, xOffset=x1, step=stepH, angle=90, xOffsetEnd=x2, xRotate=90, xScalEnd=0.5, yScalEnd=0.5) { circle(r=rX-dC/2, $fn=100);}
        }
    }
}

module E() {
    C();
    translate([0,0,hZC/2])rotate([0,0,hZC*beta])D();
    rotate([180,0,0])translate([0,0,hZC/2]) D();
    difference() {
        cylinder(d=ep+dC/2, center = true, h=hZC, $fn=100);
        rotate([0,0,(nb/2-1)*beta])translate([0,0,-2])rotate([90,0,0]) cylinder(d=1.8,h=20, center=true, $fn=50);
        rotate([0,0,nb*beta])translate([0,0,2])rotate([90,0,0]) cylinder(d=1.8,h=20, center=true, $fn=50);
        for(i=[0:nb-1]) {
            rotate([0,0,i*alpha+alpha/2+hZC*beta])translate([rX*tan(alpha),0,hZC/2]) scale([1,1,0.32])sphere(r=rX-dC/2, $fn=200);
        
            rotate([0,0,i*alpha+alpha/2])translate([rX*tan(alpha),0,-hZC/2]) scale([1,1,0.32])sphere(r=rX-dC/2, $fn=200);
        }
        
    }
    translate([0,0, hL+hZC/2])sphere(d=rX+dC/2, $fn=100);
    translate([0,0,-hL-hZC/2])sphere(d=rX+dC/2, $fn=100);
}


E();

