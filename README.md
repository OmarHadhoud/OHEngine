
# OHRenderingEngine
Inspired by Cherno's abstraction of openGL in his openGL series, I decided to implement a simple "rendering engine" using openGL, to be able to abstract it later at Runtime/Compile time to choose which graphics API to use.  
This engine is simply a wrapper for openGL API. I implemented what I studied from learnopengl.com in it.  
Currently it doesn't support a lot of features, as I'm still developing it while learning computer graphics concepts.  
I also implemented a simple game manager to have a simple game scene as a test environment.  
**Note:** It is just a test scene, it isn't well designed as I haven't put much thought into it.  
**Note:** Ignore the name. Didn't know what to name it, so called it **O**mar **H**adhoud **rendering engine**. Just Ignore. It is also a very simple engine. I don't even know what's the difference between a "Real" rendering engine and this. This is mainly done for openGL at the moment.
# Features
- A high level of abstraction for the API and its enums. Custom enums are used instead.
- Supporting different texture types, like CubeMaps and Multisampled textures. This allows us to have Skyboxes and AntiAliasing in the project.
- Supporting drawing to custom textures and/or render buffers. This allows the use of custom frame buffers which opens the possibility of post processing.  
- Implemented simple blending & culling functionalities. **Note:** Semi transparent objects should be drawn in the end, starting from the furthest object.  
- Implemented simple ray picking functionality to pick the vending machine and move it.  
- Implemented simple geomtry shader to make an explosion effect using normal maps. Added also another debug shader to draw the normal vectors loaded for the model.  
- The game manager has a simple light manager to manage setting and drawing the lights with the shaders implemented.  
- Supports Gamma Correction.  
- Supports Directional and Spot Lights shadows. **Note:** For now, only 1 light can cast shadows. Will support having more than 1 when refactoring the light manager.  
# Features to be implemented
- Implement shadows support for Point Lights using cubemaps.  
- Implement a sorting system for semi transparent objects to draw them automatically with the furthest first.  
- ~~Implement a class to manage instanced drawing of same object.~~ [Delayed, not that important for now, also needs the current code to be refactored.]  
# Scene Controls
**W**: Move forward  
**A**: Move Left  
**S**: Move backward  
**D**: Move right  
**Escape**: Exit game  
**Left click**: Pick the vending machine model [In test scene only]  
**Right click**: Drop the vending machine model [In test scene only]  
# Project structure
The project consists of 2 main folders: The source code folder, and the resources folder.  
The resources folder has the models, textures, and shaders in it.  
The source code folder consists of 2 parts: The renderer and the game scene.  
The renderer folder has all classes needed for rendering and creating a scene using it, while the game folder is responsible for the simple scene generated with the project.
# Dependencies
This project uses imgui for the GUI, stbi for image loading, and assimp for model loading. It also uses GLFW, GLAD and GLM for openGL related stuff.
# How to run
Just download the project and run using visual studio in debug mode/ release mode. Release mode is preferred as it is much faster specially to loading models using assimp.  
Note: As the 3D models size is too large, you can download it from here: https://drive.google.com/file/d/114mm2LcC46iRoOX_wXUAiKO8ff6BONml/view?usp=sharing and extract it in the res folder.  
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
![image](/Screenshots/7.png?raw=true)
Explosion effect in action. Simply moving every fragment in the direction of the normal.    
![image](/Screenshots/8.png?raw=true)
Transparent grass, semi transparent windows.  
![gif](/Screenshots/gif1.gif)  
Point lights are actually changing their colors here! Also might notice some of the blur.
![gif](/Screenshots/gif2.gif)  
Ray picking the vending machine, and moving it around.  
![gif](/Screenshots/gif3.gif)  
Explosion of the backpack.  
