#include "Lib.h"
#include <time.h>

#include "driverChoice.h"

/*
To be able to use the Irrlicht.DLL file, we need to link with the Irrlicht.lib.
We could set this option in the project settings, but to make it easy, we use a
pragma comment lib for VisualStudio. On Windows platforms, we have to get rid
of the console window, which pops up when starting a program with main(). This
is done by the second pragma. We could also use the WinMain method, though
losing platform independence then.
*/
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


Game* game = NULL;
float general_dt, fps;


bool FrameFunc() {
    // printf("frame\n");

    // printf("game Update\n");
    game->Update(general_dt);
    game->Control();

    // printf("end frame\n");

    return false;
}
bool RenderFunc() {
    // printf("render\n");

    game->GetDriver()->beginScene(true, true, SColor(255,100,101,140));

    
    // guienv->drawAll();
    
    if (game->GetGameStatus() == GAME_STATUS_PLAYING) {
        game->Draw();
        game->DrawRect(0.0f, 0.0f, 150.0f, 40.0f, 0xFF000000);
    }
    else if (game->GetGameStatus() == GAME_STATUS_MAIN_MENU) {
        if (game->DrawMainMenu(general_dt) == GAME_EVENT_START) {

        }
    }

    if (game->GetGameStatus() == GAME_STATUS_PLAYING) {
        game->_printf(0.0f, 0.0f,  ALIGN_LEFT, "%f;%f", game->GetControlledCharacter()->GetX(), game->GetControlledCharacter()->GetY());
        game->_printf(0.0f, 20.0f, ALIGN_LEFT, "fps: %.1f", fps);
    }

    game->GetDriver()->endScene();
    
    // printf("end render\n");
    return false;
}

std::string IniGetString(std::string section, std::string name, std::string def_val) {
    // std::string buff = std::string(hge->Ini_GetString(section.c_str(), name.c_str(), "}}bad{{"));
    // if (buff.compare("}}bad{{") == 0) {
    //     hge->Ini_SetString(section.c_str(), name.c_str(), def_val.c_str());
    //     return def_val;
    // }
    // return buff;
    return "sorry";
}

int main() {

    MyEventReceiver receiver;
    /*
    The most important function of the engine is the createDevice()
    function. The IrrlichtDevice is created by it, which is the root
    object for doing anything with the engine. createDevice() has 7
    parameters:

    - deviceType: Type of the device. This can currently be the Null-device,
       one of the two software renderers, D3D8, D3D9, or OpenGL. In this
       example we use EDT_SOFTWARE, but to try out, you might want to
       change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8,
       EDT_DIRECT3D9, or EDT_OPENGL.

    - windowSize: Size of the Window or screen in FullScreenMode to be
       created. In this example we use 640x480.

    - bits: Amount of color bits per pixel. This should be 16 or 32. The
       parameter is often ignored when running in windowed mode.

    - fullscreen: Specifies if we want the device to run in fullscreen mode
       or not.

    - stencilbuffer: Specifies if we want to use the stencil buffer (for
       drawing shadows).

    - vsync: Specifies if we want to have vsync enabled, this is only useful
       in fullscreen mode.

    - eventReceiver: An object to receive events. We do not want to use this
       parameter here, and set it to 0.

    Always check the return value to cope with unsupported drivers,
    dimensions, etc.
    */
    video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;

    IrrlichtDevice *device =
        createDevice( driverType, dimension2d<u32>(800, 600), 32,
            false, true, false, &receiver);

    if (!device)
        return 1;

    /*
    Set the caption of the window to some nice text. Note that there is an
    'L' in front of the string. The Irrlicht Engine uses wide character
    strings when displaying text.
    */
    device->setWindowCaption(L"rlsa");

    /*
    Get a pointer to the VideoDriver, the SceneManager and the graphical
    user interface environment, so that we do not always have to write
    device->getVideoDriver(), device->getSceneManager(), or
    device->getGUIEnvironment().
    */
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    game = new Game(device);
    // game->SetResPath("../../res");
    game->Initialize();

    receiver.SetGame(game);

    u32 then = device->getTimer()->getTime();

    /*
    Ok, now we have set up the scene, lets draw everything: We run the
    device in a while() loop, until the device does not want to run any
    more. This would be when the user closes the window or presses ALT+F4
    (or whatever keycode closes a window).
    */
    while( device->run() ) {
        /*
        Anything can be drawn between a beginScene() and an endScene()
        call. The beginScene() call clears the screen with a color and
        the depth buffer, if desired. Then we let the Scene Manager and
        the GUI Environment draw their content. With the endScene()
        call everything is presented on the screen.
        */
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        general_dt = (f32)(now - then) / 1000.f; // Time in seconds
        fps = 1.0f / general_dt;
        then = now;
        FrameFunc();
        RenderFunc();
    }

    /*
    After we are done with the render loop, we have to delete the Irrlicht
    Device created before with createDevice(). In the Irrlicht Engine, you
    have to delete all objects you created with a method or function which
    starts with 'create'. The object is simply deleted by calling ->drop().
    See the documentation at irr::IReferenceCounted::drop() for more
    information.
    */
    device->drop();

    delete game;

    // Disconnect(false);

    return 0;

}


