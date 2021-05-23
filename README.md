

# OHEngine
OHEngine started as OHRenderingEngine that was inspired by Cherno's abstraction of openGL in his openGL series. I decided to implement a simple "rendering engine" using openGL, to be able to abstract it later at Runtime/Compile time to choose which graphics API to use. I implemented what I studied from learnopengl.com in it. After a while, things started to get messy, and the whole abstraction point felt meaningless as I still haven't learnt other graphics API.  
At this point, I decided to start implementing it as a custom game engine for a FPS game. It is not a generic game engine, but I tried to make things flexible to be able to add more features whenever I like. I also decided to implement it in an Entity Component System engine type. It is still a custom engine, with some simple "hacks" here and there that fit my game (or generally a FPS game).  
I also implemented a simple game manager to have a simple game scene as a test environment.  The scene is loaded from a simple ".lvl" file that contains the entities in the scene, along with their components and the values of it.
**Note:** It is just a test scene, it isn't well designed as I haven't put much thought into it.  Also, the code still needs refactoring, as I'm still developing it.
**Note:** Ignore the name. Didn't know what to name it, so called it **O**mar **H**adhoud **engine**. Just Ignore. It is also a very simple engine. There are still many missing features that I plan to implement to learn more.
# Features
- ~~A high level of abstraction for the API and its enums. Custom enums are used instead.~~ This is scrapped, the game now uses openGL enums in the code directly. The openGL functions are still made using the renderer class functions.
- Supporting different texture types, like CubeMaps and Multisampled textures. This allows us to have Skyboxes and AntiAliasing in the project.
- Supporting drawing to custom textures and/or render buffers. This allows the use of custom frame buffers which opens the possibility of post processing.  
- Implemented simple blending & culling functionalities.
- Implemented simple ray picking functionality.  
- Implemented simple geometry shader to make an explosion effect. Added also another debug shader to draw the normal vectors loaded for the model.  
- Supports Gamma Correction.  
- Supports Directional, Point and Spot Lights shadows using shadow mapping.
- Supports HDR and Bloom post process effect.
- A simple ECS system based on SoA. It does allocate the maximum memory the game can reach before the start of the game though, to minimize allocate/de-allocating in the game. It also has a lot of hacks in that part, like having an entity ID value for every component to be able to access that entity, and an array mapping all components indices to the entities IDs.
- The game engine supports a ".lvl" text files that contain the details for the scene, by listing every entity and its components.
- The game engine supports adding any number of light sources using the ".lvl" file. There is a maximum though defined in the game engine.
- The game engine supports adding a certain number of light shadows for Directional and Spot Lights. There is also another maximum number of shadows for Point lights, as they are usually more heavy due to using cubemaps in shadow-mapping.
- The game supports a simple shooting mechanic using the ray-picking previously mentioned.
- The game has a 3D sniper drawn in FPS, with a cross-hair indicating the aiming. There is also a simple recoil effect, gunfire effect when shooting.
- The game engine also checks for collision of the player when moving by casting a specific number of rays depending on the height of the player in the direction of movement.
- The game also triggers the explosion effect when an object with a box collider is shot.
- The game also draws simple debugging lines for box colliders if _LEVEL_EDITOR_ is defined in the CONFIG.h file.
- The systems in the engine communicate using an event class, to send events between each other. 
- The GUI system uses ImGUI for now.
# Features to be implemented
- Player component.
- Health component.
- Static Shadows/Reflections.
- Parent/Child relations for entities.
- Particle effects.
- Fog effect.
- Simple hit boxes.
- Text rendering for score.
- Create a decent scene that is designed fairly well.
- Add audios for the game.
- Create simple enemy movement.
- Deferred rendering.
- SSAO.
- Level editor?
- PBR?
# Scene Controls
**W**: Move forward  
**A**: Move Left  
**S**: Move backward  
**D**: Move right  
**Escape**: Exit game  
**Left click**: Shoot
# Project structure
The project consists of 2 main folders: The source code folder, and the resources folder.  
The resources folder has the models, textures, scenes ".lvl" files and shaders in it.  
The source code folder consists of 2 parts: The renderer and the game engine. The game engine folder has old files that were used before implementing the ECS, that are left for reference only. They **SHOULD** be deleted later on. The game engine folder has the ECS folder that contains the implementation for the entities, components, systems and events. 
The renderer folder has all classes needed for rendering and creating a scene using it.
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
