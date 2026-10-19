## 🌐 Languages / Langues

[🇫🇷 Français](README.md) ·
[🇬🇧 English](README.en.md) ·
[🇩🇪 Deutsch](README.de.md) ·
[🇪🇸 Español](README.es.md)

[Original Readme](README_ORIGIN.md)
---

# PythonSCAD — Version personnalisée

Bienvenue dans mon fork de PythonSCAD !

J'avais ajouté dans une version personalisée d'OpenSCAD depuis quelques années une nouvelle fonctionnalité helicoidal_extrude(). Constatant qu'il y a une nouvelle mouture intégrant python, j'ai donc décidé de faire la même chose et intégrer cette fonctionnalité dans PythonSCAD.  
Compte tenu que cette fonction possède de nombreux paramètres, j'ai décidé de la mettre en oeuvre dans l'environnement python avec uniquement des paramètres nommés.  
Certaines personnes pourraient rétorqués que l'ajout d'un vecteur dans rotate_extrude() dans pythonSCAD fait la même chose, mais il n'en ai rien.  
Surtout pour réaliser des filetages, car le profil d'un filetage est défini dans le plan perpemdiculaire à l'axe, alors que l'introduction du vecteur dans pythonSCAD pour donner un effet d'helice entraine une inclinaison du motif.  
Je dois enrichir mes exemples d'utilisation que se soit dans openscad que pythonscad de ma nouvelle fonction, afin de convaincre le plus grand nombre de son utilité.  
 
