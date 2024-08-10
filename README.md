# CoreEngine
This library provides essential tools and utilities for game development, including rendering, physics, and input handling.
## Key Features
- Code Organization
- Code Compilation
- OpenGL Graphics Rendering ([GLFW](https://www.glfw.org/))
- Physics Libraries Implementations
- External Inputs Handling
- Audio Management
- Discord API Support
- [Dear ImGui](https://github.com/ocornut/imgui) Support
- 3D Model Importing ([Assimp](https://github.com/assimp/assimp))
- More To Come

## Dependencies
- **GLFW**: OpenGL state management library.
- **GLM**: OpenGL mathematics library.
- **Assimp**: Model importing library.
- **KHR**
- **Glad**


## Environment Variables

On the startup of the library, there are some environment variables that are being set by the ```Utils/EnvironmentVariablesUtils.cpp``` file. Those include:

```
CORE_NAME=CoreEngine
CORE_VERSION=$version
CORE_DEBUG=1 // CORE_DEBUG=1 on Release
CORE_ASSETS_PATH=$documents_folder/CoreEngine/Assets
CORE_FILE_PATH=$documents_folder/CoreEngine
```
## Usage/Examples

```cpp
#include <CoreEngine/CoreEngine.h>
#include <CoreEngine/Scenes/Scene.h>
#include <Camera/Camera.h>
#include <Shaders/Shader.h>
#include <Objects/Cube/Cube.h>
#include <Lights/Light.h>

int main()
{
	CoreEngine engine = CoreEngine(800, 600); // Create a window of 800x600 pixels
	Camera* camera = engine.getCamera(); // Save Camera pointer for later use

    /*
     * Create the engine's default shaders.
     *
     * Note that both testVertex.glsl and testFragment.glsl files are saved
     * in $documents_folder/CoreEngine/Assets/Shaders/VertexShaders/testVertex.glsl
     * and $documents_folder/CoreEngine/Assets/Shaders/VertexShaders/testFragment.glsl
     * respectively.
     *
     * It is crucial for the engine to have the default shader saved before
     * rendering.
    */
	Shader* shader = new Shader("testVertex.glsl", "testFragment.glsl"); // Create a new shader of
	engine.SetDefaultShader(shader);

    /*
     * Create a new Scene object and save it to the engine for easier
     * handling and more efficient memory loading.
     *
     * Note that if you don't call the SetCurrentScene, the default scene will be the
     * the first one to be loaded (index 0)
     *
     * If no scenes are loaded it will throw an error and terminate.
    */
	Scene* scene = new Scene(camera);
	engine.AddScene(scene);
	engine.SetCurrentScene(scene);

    /*
     * Create a Cube Object and save it into the scene.
     * The Cube model file is saved at $documents_folder/CoreEngine/Assets/Resources/Models/Default folder.
     * Proceeds by adding the object into the scene. The default position is (x,y,z)=(0,0,0)
    */
	Cube* cubeObject = new Cube(camera, shader, glm::vec4(1.0f));
	scene->AddObject(cubeObject);

    /*
     * Creates a point light object with color of
     * (r,g,b,a)=(1,1,1,1) with intensity of 1.0f and places it at (x,y,z)=(1,1,1)
    */
	Light* light = new Light(glm::vec4(1.0f), 1.0f);
	light->SetPosition(1.0f, 1.0f, 1.0f);
	scene->AddLight(light);

    /*
     * Starts the Render loop of the engine and terminates when the window should be closed.
    */
	engine.Run();
	engine.Shutdown();
	return 0;
}
```


## Documentation

[Documentation](https://linktodocumentation)


## Installation

Clone the project into your local machine: ```git clone https://github.com/MattZafeiriou/CoreEngine.git```

Download and compile the Dependencies mentioned above and put them into your project.

[How to link GLFW, GLAD and KHR tutorial](https://learnopengl.com/Getting-started/Creating-a-window)

[How to link GLM tutorial](https://learnopengl.com/Getting-started/Transformations)

[How to link Assimp tutorial](https://github.com/assimp/assimp/blob/master/Build.md)

After linking the necessary dependencies succesfully, open the project in your preferred IDE and build the project and run it locally.

The ```main``` function is located at ```Examples/main.cpp```
## Contributing

1. Fork the repository.
2. Create a new branch for your changes.
3. Submit a pull request with a clear description of your changes.
## Authors

- [@MattZafeiriou](https://github.com/MattZafeiriou)
- [@PanosGiann](https://github.com/PanosGiann)
## Acknowledgements

 - [LearnOpenGL.com](https://learnopengl.com/)
## License

[MIT](https://choosealicense.com/licenses/mit/)


## Support

For support, email matuaioszafeiriou@gmail.com or create a pull request addressing your issue.

