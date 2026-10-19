from pythonscad import *

tMetric = {}
class Metrique(object):
    label=""
    diam =0
    pas=0
    sKey=0
    hVis=0
    hEcrou=0
    def __init__(self, label, diam, pas, sKey=0,hV=0,hE=0):
        self.label = label
        self.diam  = diam
        self.pas   = pas
        self.sKey  = sKey
        self.hVis  = hV
        self.hEcrou  = hE
        tMetric[label] = self
    def get(ref):
        info = tMetric.get(ref, None)
        if info == None:
            info = tMetric.get(f"M{ref}", None)
        if info == None:
            print(f"Error {ref} unknown")
        return info

        

Metrique( "M3",  3, 0.5 , 5.5,  2.0,  2.4)
Metrique( "M4",  4, 0.7 ,  7 ,  2.8,  3.2)
Metrique( "M5",  5, 0.8 ,  8 ,  3.5,  4.0)
Metrique( "M6",  6, 1.0 , 10 ,  4.0,  5.0)
Metrique( "M8",  8, 1.25, 13 ,  5.3,  6.5)
Metrique("M10", 10, 1.5 , 17 ,  6.4,  8.0)
Metrique("M12", 12, 1.75, 19 ,  7.5, 10.0)
Metrique("M14", 14, 2.0 , 22 ,  8.8, 11.0)
Metrique("M16", 16, 2.0 , 24 , 10.0, 13.0)
Metrique("M18", 18, 2.5 , 27 , 11.5, 15.0)
Metrique("M20", 20, 2.5 , 30 , 12.5, 16.0)
Metrique("M22", 22, 2.5 , 32 , 14.0, 18.0)
Metrique("M24", 24, 3.0 , 36 , 15.0, 19.0)
Metrique("M27", 27, 3.0 , 41 , 17.0, 22.0)
Metrique("M30", 30, 3.5 , 46 , 18.7, 24.0)
Metrique("M33", 33, 3.5 , 50 , 21.0, 26.0)
Metrique("M36", 36, 4.0 , 55 , 22.5, 29.0)
Metrique("M39", 39, 4.0 )
Metrique("M42", 42, 4.5 )
Metrique("M45", 45, 4.5 )
Metrique("M48", 48, 5.0 )
Metrique("M52", 52, 5.0 )
Metrique("M56", 56, 5.5 )
Metrique("M60", 60, 5.5 )
Metrique("M64", 64, 6.0 )
def trapezeA(h, l) :
    return polygon([[0,-l/2-0.05], [h, -0.05], [h, 0.05], [0, l/2+0.05] ]);


def trapezeB(h, l) :
    return polygon([[0,-0.05], [h, -l/2-0.05], [h, l/2+0.05], [0, 0.05]]);
 
def filetage(hauteur, dMajor, pas, bExtern) :
    rMajor = dMajor/2;
    hP = 0.5413*pas; # 5/8 * sqrt(3/2)
    l = 5*pas / 8;
    if(bExtern):
        a = 360*(hauteur-pas)/pas;
        rH = rMajor-hP;
        trA=trapezeA(hP, l)
        h1= helicoidal_extrude(trA,  angle=a, step=pas, fn=100, xOffset=rH, xScaleEnd=1, yScaleEnd=1)
        h2= trA.helicoidal_extrude( angle=120, convexity = 2, fs=20, step=pas, fn=60, xOffset=rH, xScaleEnd=0.2, yScaleEnd=0.1).translate([0,0,hauteur-pas/2])
        h2=h2.rotate([0,0,360*(hauteur-pas)/pas])
        h2=h2.color("red")   
        h3=trA.helicoidal_extrude( angle = -120, convexity = 2, fs=20, step=-pas, fn=60, xOffset=rH, xScaleEnd=0.2, yScaleEnd=0.1) 
        h3=h3.color("blue") 
        
        return h1.translate([0,0,pas/2]) | h2| h3.translate([0,0,pas/2]) | cylinder(h=hauteur, r=rH, fn=300);
    else:
        a = 360*(hauteur+0.1-pas)/pas;
        rH = rMajor+0.15-hP;
        
        trB=trapezeB(hP, l)
        h1=trB.helicoidal_extrude(angle =a, step=pas, fn=300, xOffset=rH)
        h1.translate([0,0,pas/2])
        h2=trB.helicoidal_extrude(angle=120, convexity = 2, fs=20, step=pas, fn=60, xOffset=rH, xOffsetEnd=rH+hP*0.9,xScaleEnd=0.2, yScaleEnd=0.1) 
        h2=h2.translate([0,0,hauteur-pas/2])
        h2=h2.rotate([0,0,360*(hauteur-pas)/pas])
        h2=h2.color("red")
        h3=trB.helicoidal_extrude(angle = -120, convexity = 2, fs=20, step=-pas, fn=60, xOffset=rH, xOffsetEnd=rH+hP*0.9, xScaleEnd=0.2, yScaleEnd=0.1)        
        h3=h3.translate([0,0,pas/2]).color("blue")
        return difference(
            cylinder(h=hauteur+l, r=rMajor+0.15, fn=300),
            h1,h2,h3).translate([0,0,-0.1])
 
       

def filetMetrique(ref, hauteur, bExtern):
    info = Metrique.get(ref)
    if info == None:
         print(f"Error {ref} unknown")
         return None
    return filetage(hauteur, info.diam, info.pas, bExtern)
        
def vis(ref, hauteur):
    info = Metrique.get(ref)
    if info == None:
         print(f"Error {ref} unknown")
         return None
    f = filetMetrique(info.diam, hauteur, True)
    c = cylinder(h=info.hVis, d=(info.sKey*1.155), fn=6)
    return  c | f.translate([0,0,info.hVis])

def ecrou(ref, hauteur=0):
    info = Metrique.get(ref)
    if info == None:
         print(f"Error {ref} unknown")
         return None
    if hauteur <=0:
        hauteur = info.hEcrou
    f = filetMetrique(info.diam, hauteur, False)
    c = cylinder(h=hauteur, d=(info.sKey*1.155), fn=6)
    return  c - f

#show(vis("M6", 20))
#show(ecrou("M6").translate([20,0,0]))