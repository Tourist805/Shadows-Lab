# COMP3015-CW2-Developer Optimized Tool

For this project, I have decided to create a couple of scenes implementing different computer graphics techniques.

Developer walkthrough: https://youtu.be/i-zlYLIkYzI
GitHub: https://github.com/Tourist805/Shadows-Lab

## Software and Operational Systems
 - Windows 10
 - Microsoft Visual Studio 2019
 - OpenGL 4.3

## First Scene - PBR techniques
First scene uses PBR techniques, as well as spotlight to demonstate a playable demo. The inspriation came from the Star Wars Series intro [cutscene](https://www.youtube.com/watch?v=wLqCgRK_NMI).
And the asset downloaded from [GrabCard](https://grabcad.com/library/arc-trooper-helmet-advanced-recon-commando-trooper-1)

The result for the higher rougness.
![PBR](https://github.com/Tourist805/Shadows-Lab/blob/main/gifs/2022-05-16%2011-16-54.gif)
 
The result for the lower rougness.
![PBR](https://github.com/Tourist805/Shadows-Lab/blob/main/gifs/2022-05-16%2011-54-37.gif)

The scene uses the PBR model from the [LearnOpenGL](https://learnopengl.com/PBR/Theory ) and [OpenGL Shading Cookbook](https://www.amazon.co.uk/OpenGL-4-0-Shading-Language-Cookbook/dp/1849514763).

The initial idea is to use the microfacets surface and calculate the refraction and reflection. So the three parameters that defines my implementation are the surface roughness, scales 0.0 to 1.0, metalicness, and diffuse colour for the surface.

Besides for making the smoother image, especially for the edges, I have used the [Schlick approximation](https://link.springer.com/chapter/10.1007/978-1-4842-7185-8_9?noAccess=true), as well as gamma correction.

## Second Scene - Particle Simulation (Attractors)

The idea of [attractors](https://en.wikipedia.org/wiki/Attractor) is basically the mathematical model for a set of evovling states.
The scene has a three gravitational point around which the set of points are transformating, drawing a unique shapes.
Gravitational point has the gravity, the force that will be applied to each point.
For this simulation the force for each point computed  on each particle and then positions update using kinematic equations.
So v = v0 + a * deltaT;

Each particle at the begining of simulation pipeline has the initial positio, as well as velocity.
For this example I have write all the physics in a compute shader, which leverages the GPU and works in parallel. It is not directly connected to the classic shader pipeline.

In my case I have generated 1000 points.

So for this part I have used:
- Vertex Animation
- Particles

![Particles](https://github.com/Tourist805/Shadows-Lab/blob/main/gifs/2022-05-16%2013-18-26.gif)

## Third scene - Ordered tranparency
![Ordered Transparency](https://github.com/Tourist805/Shadows-Lab/blob/main/gifs/Screenshot%202022-05-16%20132256.png)

For this application, I have only sorted the transparency layer based on the insertion sort
