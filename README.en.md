## 🌐 Languages / Langues

[🇫🇷 Français](README.md) ·
[🇬🇧 English](README.en.md) ·
[🇩🇪 Deutsch](README.de.md) ·
[🇪🇸 Español](README.es.md)

[Original Readme](README_ORIGIN.md)
---

*** This text was translated by an AI; the original version is in [French](README.MD). ***  

# Welcome to my PythonSCAD fork!

A few years ago, I added a new helicoidal_extrude() feature to a customized version of OpenSCAD. Having noticed that a new version integrating Python is now available, I decided to do the same and integrate this feature into PythonSCAD.

Since this function has many parameters, I decided to implement it in the Python environment using keyword arguments only.

Some people might argue that adding a vector to rotate_extrude() in PythonSCAD achieves the same result, but this is not the case.

This is particularly important when creating screw threads, because a thread profile is defined in a plane perpendicular to the axis, whereas introducing a vector in PythonSCAD to produce a helical effect causes the profile to be tilted.

I still need to expand my usage examples for this new function in both OpenSCAD and PythonSCAD in order to convince as many people as possible of its usefulness.

