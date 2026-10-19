use <VisMetrique.scad>

translate([ 0,20,0])vis(8,20);
translate([20,20,0])ecrou(8);
/* */
difference() {
    hull() {
        cylinder(h=8, d=10, $fn=100);
        translate([34,0,0])cylinder(h=8, d=16, $fn=100);
    }
    translate([1,0,0])  filetMetrique( 6, 8, false);
    translate([10,0,0]) filetMetrique( 8, 8, false);
    translate([21,0,0]) filetMetrique(10, 8, false);
    translate([34,0,0]) filetMetrique(12, 8, false);
}

translate([10,-20,0]){
   translate([0,0,24]) filetMetrique( 6, 8, true);
   translate([0,0,16]) filetMetrique( 8, 8, true);
   translate([0,0,8])  filetMetrique(10, 8, true);
   translate([0,0,0])  filetMetrique(12, 8, true);
}