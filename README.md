
# OHRenderingEngine
Inspired by Cherno's abstraction of openGL in his openGL series, I decided to implement a simple rendering engine using openGL, to be able to abstract it later at Runtime/Compile time to choose which graphics API to use.  
This engine is simply a wrapper for openGL API. I implemented what I studied from learnopengl.com in it.  
Currently it doesn't support a lot of features, as I'm still developing it while learning computer graphics concept.  
I also implemented a simple game manager to have a simple game scene as a test environment.  
**Note:** It is just a test scene, it isn't well designed as I haven't put much thought into it.
# Features
- A high level of abstraction for the API and its enums. Custom enums are used instead.
- Supporting different texture types, like CubeMaps and Multisampled textures. This allows us to have Skyboxes and AntiAliasing in the project.
- Supporting drawing to custom textures and/or render buffers. This allows the use of custom frame buffers which opens the possibility of post processing.
- The game manager has a simple light manager to manage setting and drawing the lights with the shaders implemented.
# Scene Controls
**W**: Move forward  
**A**: Move Left  
**S**: Move backward  
**D**: Move right  
**Escape**: Exit game  
# Project structure
The project consists of 2 main folders: The source code folder, and the resources folder.  
The resources folder has the models, textures, and shaders in it.  
The source code folder consists of 2 parts: The renderer and the game scene.  
The renderer folder has all classes needed for rendering and creating a scene using it, while the game folder is responsible for the simple scene generated with the project.
# Dependencies
This project uses imgui for the GUI, stbi for image loading, and assimp for model loading. It also uses GLFW, GLAD and GLM for openGL related stuff.
# How to run
Just download the project and run using visual studio in debug mode/ release mode. Release mode is preferred as it is much faster specially to loading models using assimp.
# Screenshots/GIFS
![image](/Screenshots/1.png?raw=true)
A pink point light, a white directional light from the left side, both reflecting on the ground. Skybox made of a Yokohama panorama picture can be seen.
![image](/Screenshots/2.png?raw=true)
On the left we can see 2 point lights, with 1 green spotlight -thus rotated- on the right side. In the middle there's a bad vending machine model that I just created in blender.
![image](/Screenshots/3.png?raw=true)
Same scene, just behind the poor vending machine.
![image](/Screenshots/4.png?raw=true)
Simple motion blur in effect.
![image](/Screenshots/5.png?raw=true)
Same scene without the motion blur.
![image](/Screenshots/6.png?raw=true)
Yokohama. Beautiful.
![gif](/Screenshots/gif.gif)  
Point lights are actually changing their colors here! Also might notice some of the blur.

