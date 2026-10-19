## 🌐 Languages / Langues

[🇫🇷 Français](README.md) ·
[🇬🇧 English](README.en.md) ·
[🇩🇪 Deutsch](README.de.md) ·
[🇪🇸 Español](README.es.md)

[Original Readme](README_ORIGIN.md)
---

*** Este texto ha sido traducido por una IA; la versión original está en [francés](README.md). ***  

# ¡Bienvenido a mi fork de PythonSCAD!

Hace unos años, añadí una nueva funcionalidad llamada helicoidal_extrude() a una versión personalizada de OpenSCAD. Al darme cuenta de que ahora existe una nueva versión que integra Python, decidí hacer lo mismo e integrar esta funcionalidad en PythonSCAD.

Dado que esta función tiene numerosos parámetros, decidí implementarla en el entorno Python utilizando únicamente argumentos con nombre.

Algunas personas podrían argumentar que añadir un vector a rotate_extrude() en PythonSCAD produce el mismo resultado, pero no es así.

Esto es especialmente importante a la hora de crear roscas, ya que el perfil de una rosca se define en un plano perpendicular al eje, mientras que la introducción de un vector en PythonSCAD para generar un efecto helicoidal provoca una inclinación del perfil.

Todavía tengo que ampliar mis ejemplos de uso de esta nueva función, tanto en OpenSCAD como en PythonSCAD, para convencer al mayor número posible de personas de su utilidad.  

