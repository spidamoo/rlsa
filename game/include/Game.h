#ifndef GAME_H
#define GAME_H

#include <Lib.h>

const char GAME_STATUS_MAIN_MENU     = 0;
const char GAME_STATUS_PLAYING       = 1;

const char GAME_EVENT_START = 1;

const std::string KEY_NAMES[0x7C] = {
    "",
    "LMB",
    "RMB",
    "",
    "Wheel",
    "",
    "",
    "",
    "BACKSPACE",
    "TAB",
    "",
    "",
    "",
    "ENTER",
    "",
    "",
    "SHIFT",
    "CTRL",
    "ALT",
    "PAUSE",
    "CAPSLOCK",
    "",
    "",
    "",
    "",
    "",
    "",
    "ESCAPE",
    "",
    "",
    "",
    "",
    "SPACE",
    "PGUP",
    "PGDN",
    "END",
    "HOME",
    "LEFT",
    "UP",
    "RIGHT",
    "DOWN",
    "",
    "",
    "",
    "",
    "INSERT",
    "DELETE",
    "",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "LWIN",
    "RWIN",
    "APPS",
    "",
    "",
    "NUMPAD0",
    "NUMPAD1",
    "NUMPAD2",
    "NUMPAD3",
    "NUMPAD4",
    "NUMPAD5",
    "NUMPAD6",
    "NUMPAD7",
    "NUMPAD8",
    "NUMPAD9",
    "MULTIPLY",
    "ADD",
    "",
    "SUBTRACT",
    "DECIMAL",
    "DIVIDE",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12"
};


const unsigned short DRAW_STAGE_ALL                      = 1;
const unsigned short DRAW_STAGE_MAP                      = 2;
const unsigned short DRAW_STAGE_MAP_TILES                = 3;
const unsigned short DRAW_STAGE_MAP_TILES_RENDER         = 4;
const unsigned short DRAW_STAGE_MAP_FOV                  = 5;
const unsigned short DRAW_STAGE_MAP_OBJECTS              = 6;
const unsigned short DRAW_STAGE_CHARACTERS               = 7;
const unsigned short DRAW_STAGE_LOOTS                    = 8;
const unsigned short DRAW_STAGE_EFFECTS                  = 9;
const unsigned short DRAW_STAGE_OVERLAY                  = 10;
const unsigned short DRAW_STAGE_OVERLAY_CALCULATE        = 11;
const unsigned short DRAW_STAGE_OVERLAY_CALCULATE_ANGLES = 12;
const unsigned short DRAW_STAGE_OVERLAY_CALCULATE_RAYS   = 13;
const unsigned short DRAW_STAGE_OVERLAY_CALCULATE_SORT   = 14;
const unsigned short DRAW_STAGE_OVERLAY_RENDER           = 15;

const char ALIGN_LEFT   = 0;
const char ALIGN_CENTER = 1;
const char ALIGN_RIGHT  = 2;

class Game {
    public:
        Game(IrrlichtDevice* device);
        virtual ~Game();

        IrrlichtDevice* GetDevice();
        IVideoDriver* GetDriver();
        ISceneManager* GetSceneManager();

        void Initialize();

        void GenerateLevel(std::string template_filename);

        void Control();
        virtual bool Update(float dt);

        void Draw();
        char DrawMainMenu(float dt);

        void _printf(float x, float y, char align, const char *format, ...);
        void DrawLine(float x1, float y1, float x2, float y2, DWORD color);
        void DrawRect(float left, float top, float right, float bottom, DWORD color);
        void DrawRect(float left, float top, float right, float bottom, DWORD color, DWORD fill_color);
        void DrawRect(float left, float top, float right, float bottom, DWORD color, DWORD fill_color, float z);
        void DrawCircle(float cx, float cy, float r, DWORD color);
        void DrawSector(float cx, float cy, float r, float sa, float ea, DWORD color);

        float CastRay(float x, float y, float angle, float _default);

        Animation* GetUtilSprite(std::string index);
        Animation* GetAnimation(std::string filename, char anchor);
        Animation* GetAnimation(std::string filename);
        Animation* GetAnimation(TiXmlElement* xml, char anchor);
        // HTEXTURE GetTexture(std::string filename);
        void StretchUtilSpriteOnNode(std::string name, ISceneNode* node);
        irr::gui::IGUIFont* GetFont();

        void SetMousePos(float x, float y);
        void SetMouseButtonsState(u32 state);
        void SetMouseWheel(float wheel);
        void SetKeyState(EKEY_CODE key, bool down);
        void PushInputChar(wchar_t _char);
        wchar_t PopInputChar();
        float GetMouseX(), GetMouseY(), GetWorldMouseX(), GetWorldMouseY();
        bool IsLeftMBPressed() const { return 0 != ( mouse_buttons_state & EMBSM_LEFT ); }
        bool IsRightMBPressed() const { return 0 != ( mouse_buttons_state & EMBSM_RIGHT ); }
        bool IsMiddleMBPressed() const { return 0 != ( mouse_buttons_state & EMBSM_MIDDLE ); }
        bool GetKeyState(EKEY_CODE key);
        bool IsKeyDown(EKEY_CODE key);

        float GetCameraX(), GetCameraY();
        void SetCamera(float x, float y);
        short GetWindowWidth(), GetWindowHeight(), GetHalfWindowWidth(), GetHalfWindowHeight();

        void SetControlledCharacter(Character* character);
        Character* GetControlledCharacter();
        void SetGameStatus(char game_status);
        char GetGameStatus();

        void FireEvent(char event);
        bool DebugOn();
    protected:
        IrrlichtDevice* device;

        std::unordered_map<std::string, Animation*> animations;
        // std::unordered_map<std::string, HTEXTURE> textures;

        std::unordered_map<std::string, ITexture*>   util_textures;
        std::unordered_map<std::string, Animation*>  util_sprites;
        // hgeQuad quad; hgeTriple triple;
        SMaterial* default_material;

        scene::ISceneNode* sun;

        bool debug;
        bool display_profiler;
        Profiler* draw_profiler;

        irr::gui::IGUIFont* default_font;
        irr::gui::IGUIFont* arial12;
        static char buffer[1024];

        // short* tile_animations;
        // std::forward_list<MapObject>* tile_objects;
        // Animation** map_animations;
        // scene::IMeshSceneNode** map_cubes;
        // video::ITexture**   map_textures;

        float dt;
        bool key_state[KEY_KEY_CODES_COUNT];
        bool prev_key_state[KEY_KEY_CODES_COUNT];
        bool key_is_down[KEY_KEY_CODES_COUNT];
        std::queue<wchar_t> input_chars;
        float mouse_x; float mouse_y; float world_mouse_x, world_mouse_y;
        u32 mouse_buttons_state;
        float mouse_wheel;
        ICameraSceneNode* camera;
        float camera_x; float camera_y; float camera_z;

        short window_width;
        short window_height;
        short half_window_width;
        short half_window_height;

        GUIWindowMainMenu* main_menu_window;

        Character* controlled_character;
        char game_status;
    private:
};

char* copyString(const char* original);
void  copyString(const char* from, char* to);
bool compareStrings(char* first, char* second);

#endif // GAME_H
