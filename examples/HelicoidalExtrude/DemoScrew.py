from pythonscad import *
from VisMetrique import *

show( translate(vis("M8",20),[ 0,20,0]))
show( translate(ecrou("M8"),[20,20,0]));

a=difference(
    hull(
        cylinder(h=8, d=10, fn=100),
        cylinder(h=8, d=16, fn=100).translate([34,0,0])
    ),
    filetMetrique( "M6", 8, False).translate([1,0,0]),
    filetMetrique( "M8", 8, False).translate([10,0,0]),
    filetMetrique("M10", 8, False).translate([21,0,0]),
    filetMetrique("M12", 8, False).translate([34,0,0]) 
)

b=translate(
   filetMetrique( "M6", 8, True).translate([0,0,24])|
   filetMetrique( "M8", 8, True).translate([0,0,16])|
   filetMetrique("M10", 8, True).translate([0,0,8]) |
   filetMetrique("M12", 8, True).translate([0,0,0]),
 [10,-20,0])  

show(a,b)
