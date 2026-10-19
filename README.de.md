## 🌐 Languages / Langues

[🇫🇷 Français](README.md) ·
[🇬🇧 English](README.en.md) ·
[🇩🇪 Deutsch](README.de.md) ·
[🇪🇸 Español](README.es.md)

[Original Readme](README_ORIGIN.md)
---
*** Dieser Text wurde von einer KI übersetzt; die Originalversion ist auf [Französisch](README.md). ***  
# Willkommen bei meinem PythonSCAD-Fork!  

Vor einigen Jahren habe ich einer angepassten Version von OpenSCAD eine neue Funktion namens helicoidal_extrude() hinzugefügt. Da ich festgestellt habe, dass inzwischen eine neue Version verfügbar ist, die Python integriert, habe ich beschlossen, dasselbe zu tun und diese Funktion auch in PythonSCAD zu integrieren.  

Da diese Funktion zahlreiche Parameter besitzt, habe ich mich entschieden, sie in der Python-Umgebung ausschließlich mit benannten Argumenten (Keyword-Argumenten) zu implementieren.  

Manche könnten behaupten, dass das Hinzufügen eines Vektors zu rotate_extrude() in PythonSCAD dasselbe bewirkt. Das ist jedoch nicht der Fall.  

Dies ist insbesondere bei der Erstellung von Schraubengewinden wichtig, da das Gewindeprofil in einer Ebene senkrecht zur Achse definiert wird, während die Einführung eines Vektors in PythonSCAD, um einen Helixeffekt zu erzeugen, zu einer Neigung des Profils führt.  

Ich muss meine Anwendungsbeispiele für diese neue Funktion sowohl in OpenSCAD als auch in PythonSCAD noch erweitern, um möglichst viele Anwender von ihrem Nutzen zu überzeugen.
