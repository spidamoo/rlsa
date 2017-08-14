#include "Game.h"

char Game::buffer[1024];

Game::Game(IrrlichtDevice* _device) {
    device = _device;
    
    window_width  = 800;
    window_height = 600;
    half_window_width  = window_width * 0.5f;
    half_window_height = window_height * 0.5f;

    mouse_x = half_window_width;
    mouse_y = half_window_height;
    mouse_buttons_state = 0;
    mouse_wheel = 0.0f;
    memset(key_is_down,    0, KEY_KEY_CODES_COUNT);
    memset(key_state,      0, KEY_KEY_CODES_COUNT);
    memset(prev_key_state, 0, KEY_KEY_CODES_COUNT);
    camera_x = camera_y = 100.0f;
    camera_z = 10.0f;
    
    default_font = device->getGUIEnvironment()->getBuiltInFont();
    arial12 = device->getGUIEnvironment()->getFont("arial12.xml");


    controlled_character = NULL;
    game_status = GAME_STATUS_MAIN_MENU;

    draw_profiler = new Profiler(15);
    draw_profiler->SetStageName(DRAW_STAGE_ALL, "all");
    draw_profiler->SetStageName(DRAW_STAGE_MAP, "map");
    draw_profiler->SetStageName(DRAW_STAGE_MAP_TILES, "tiles");
    draw_profiler->SetStageName(DRAW_STAGE_MAP_TILES_RENDER, "render");
    draw_profiler->SetStageName(DRAW_STAGE_MAP_FOV, "fov");
    draw_profiler->SetStageName(DRAW_STAGE_MAP_OBJECTS, "obj");
    draw_profiler->SetStageName(DRAW_STAGE_CHARACTERS, "char");
    draw_profiler->SetStageName(DRAW_STAGE_LOOTS, "loot");
    draw_profiler->SetStageName(DRAW_STAGE_EFFECTS, "eff");
    draw_profiler->SetStageName(DRAW_STAGE_OVERLAY, "fog");
    draw_profiler->SetStageName(DRAW_STAGE_OVERLAY_CALCULATE, "calc");
    draw_profiler->SetStageName(DRAW_STAGE_OVERLAY_CALCULATE_ANGLES, "angle");
    draw_profiler->SetStageName(DRAW_STAGE_OVERLAY_CALCULATE_RAYS, "ray");
    draw_profiler->SetStageName(DRAW_STAGE_OVERLAY_CALCULATE_SORT, "sort");
    draw_profiler->SetStageName(DRAW_STAGE_OVERLAY_RENDER, "render");
    display_profiler = false;
}

Game::~Game() {
    //dtor
}

IrrlichtDevice* Game::GetDevice() {
    return device;
}
IVideoDriver* Game::GetDriver() {
    return device->getVideoDriver();
}
ISceneManager* Game::GetSceneManager() {
    return device->getSceneManager();
}

void Game::Initialize() {
    util_textures["basic"] = GetDriver()->getTexture("imgs/util.png");
    util_sprites["heart"]  = new Animation(this, util_textures["basic"], 64.0f, 0.0f, 16.0f, 16.0f);
    // util_sprites["heart"]->SetHotSpot(8.0f, 8.0f);
    // util_sprites["heart"]->SetBlendMode(BLEND_DEFAULT);
    // util_sprites["heart"]->SetZ(0.0f);
    util_sprites["target"]  = new Animation(this, util_textures["basic"], 80.0f, 0.0f, 64.0f, 64.0f);
    // util_sprites["target"]->SetHotSpot(32.0f, 32.0f);
    // util_sprites["target"]->SetBlendMode(BLEND_DEFAULT);
    // util_sprites["target"]->SetZ(0.99f);
    util_sprites["small_target"]  = new Animation(this, util_textures["basic"], 63.0f, 47.0f, 17.0f, 17.0f);
    // util_sprites["small_target"]->SetHotSpot(8.0f, 8.0f);
    // util_sprites["small_target"]->SetBlendMode(BLEND_DEFAULT);
    // util_sprites["small_target"]->SetZ(0.99f);
    // util_sprites["small_target"]->SetColor(0x88000000);
    util_sprites["default_tile"]  = new Animation(this, util_textures["basic"], 144.0f, 32.0f, 16.0f, 16.0f);
    // util_sprites["default_tile"]->SetHotSpot(0.0f, 0.0f);
    // util_sprites["default_tile"]->SetBlendMode(BLEND_DEFAULT);
    // util_sprites["default_tile"]->SetZ(1.0f);

    // util_textures["tile_types"] = hge->Texture_Load("imgs/tile_types.png");
    // for (char type = 1; type < 4; type++) {
    //     util_sprites["tile_" + TILE_TYPE_NAMES[type]] = new hgeSprite(util_textures["tile_types"], 16.0f * (type - 1), 0.0f, 16.0f, 16.0f);
    //     util_sprites["tile_" + TILE_TYPE_NAMES[type]]->SetBlendMode(BLEND_DEFAULT);
    //     util_sprites["tile_" + TILE_TYPE_NAMES[type]]->SetZ(0.7f);
    // }


    // arial12 = new hgeFont("arial12.fnt");
    // arial12->SetZ(0.0f);
    // arial12->SetBlendMode(BLEND_DEFAULT);

    GetSceneManager()->setShadowColor(video::SColor(150,0,0,0));

    // GetDriver()->setTextureCreationFlag(ETCF_ALWAYS_32_BIT);
    // GetDriver()->setTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL, false);

    default_material = new SMaterial();

    camera = GetSceneManager()->addCameraSceneNode( 0, vector3df(32, 32, 10), vector3df(32, 32, 0) );
    camera->bindTargetAndRotation(true);
    camera->setUpVector( vector3df(0,-1,0) );

    sun = GetSceneManager()->addLightSceneNode(0, core::vector3df(10,10,30),
    video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);

    main_menu_window = new GUIWindowMainMenu(this);
    GUIButton* go_button = new GUIButton(this, "GO", [&](){
        game_status = GAME_STATUS_PLAYING;
    });
    go_button->SetPosition(half_window_width, half_window_height);
    main_menu_window->AddChild(go_button);

    controlled_character = new Character(this);

    GenerateLevel("levels/test.xml");
}

void Game::GenerateLevel(std::string template_filename) {
    TiXmlDocument doc( template_filename.c_str() );
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        TiXmlElement* xml = doc.FirstChildElement("level");

        std::vector< std::vector<std::string> > sector_sets;
        TiXmlElement* set_element = xml->FirstChildElement("set");
        while (set_element) {
            std::vector<std::string> set_options;
            TiXmlElement* option_element = set_element->FirstChildElement("option");
            while (option_element) {
                set_options.push_back( std::string( option_element->Attribute("filename") ) );
                option_element = option_element->NextSiblingElement("option");
            }
            sector_sets.push_back(set_options);
            set_element = set_element->NextSiblingElement("set");
        }

        TiXmlElement* sector_element = xml->FirstChildElement("sector");
        while (sector_element) {
            char set_index = atoi( sector_element->Attribute("set") );
            IAnimatedMeshSceneNode* node = device->getSceneManager()->addAnimatedMeshSceneNode(
                device->getSceneManager()->getMesh( sector_sets[set_index][0].c_str() ),
                0,
                -1,
                core::vector3df(atof( sector_element->Attribute("x") ), atof( sector_element->Attribute("y") ), 0),
                core::vector3df(90, 0, 0),
                core::vector3df(1, 1, 1)
            );
            // node->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
            node->addShadowVolumeSceneNode();

            sector_element = sector_element->NextSiblingElement("sector");
        }
    }
}


void Game::Control() {
    // printf("Control %f %f %f\n", camera_z, mouse_wheel, dt);
    camera_z -= 5.0f * mouse_wheel * dt;
    if (camera_z < 3.0f) {
        camera_z = 3.0f;
    }
    else if (camera_z > 30.0f) {
        camera_z = 30.0f;
    }

    core::vector3df world_mouse_point;
    core::triangle3df world_mouse_triangle;
    core::line3df mouse_ray = GetSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates( core::position2d<s32>(mouse_x, mouse_y) );
    ISceneNode* s = GetSceneManager()->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(
        mouse_ray,
        world_mouse_point,
        world_mouse_triangle
    );
    world_mouse_x = world_mouse_point.X;
    world_mouse_y = world_mouse_point.Y;
    // printf("%p<<(%.2f %.2f %.2f --> %.2f %.2f %.2f)(%.2fx%.2f)\n",
    //     s,
    //     mouse_ray.start.X,  mouse_ray.start.Y,  mouse_ray.start.Z, mouse_ray.end.X,  mouse_ray.end.Y,  mouse_ray.end.Z,
    //     world_mouse_x, world_mouse_y
    // );

    if ( IsKeyDown(KEY_KEY_R) ) {
        if (!display_profiler) {
            draw_profiler->Reset();
        }
        display_profiler = !display_profiler;
    }

    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i) {
        if (key_is_down[i]) {
            key_is_down[i] = false;
        }
        else if (key_state[i] && !prev_key_state[i]) {
            key_is_down[i] = true;
        }
        prev_key_state[i] = key_state[i];
    }

    if (  key_is_down[KEY_KEY_G] ) {
        debug = !debug;
    }


    controlled_character->Control(dt);

}

bool Game::Update(float _dt) {
    dt = _dt;
    controlled_character->Update(dt);
    SetCamera( controlled_character->GetX(), controlled_character->GetY() );
};

void Game::Draw() {
    // printf("Draw\n");
    draw_profiler->StartStage(DRAW_STAGE_ALL);
    GetDriver()->setRenderTarget(0, false, false);

    // draw_profiler->StartStage(DRAW_STAGE_MAP);
    // DrawMap();
    // draw_profiler->StopStage(DRAW_STAGE_MAP);

    device->getSceneManager()->drawAll();

    draw_profiler->StartStage(DRAW_STAGE_CHARACTERS);
    controlled_character->Draw();
    draw_profiler->StopStage(DRAW_STAGE_CHARACTERS);

    if (display_profiler) {
        DrawRect(window_width - 155, 15, window_width - 45, 125, 0xFF000000);
        draw_profiler->Draw(this, window_width - 150, 20);
    }

    draw_profiler->StopStage(DRAW_STAGE_ALL);
    // printf("end Draw\n");
}


char Game::DrawMainMenu(float _dt) {
    // DrawLine(half_window_width - 100.0f, half_window_height, half_window_width + 100.0f, half_window_height, 0xFFFFFFFF);
    return main_menu_window->Draw(_dt);
}


void Game::_printf(float x, float y, char align, const char *format, ...) {
    char *pArg=(char *) &format+sizeof(format);

    vsnprintf(buffer, sizeof(buffer)-1, format, pArg);
    buffer[sizeof(buffer)-1]=0;
    //vsprintf(buffer, format, pArg);
    

    arial12->draw(
        buffer,
        core::rect<s32>(x, y, x, y),
        video::SColor(255,255,255,255),
        align == ALIGN_CENTER, false
    );
}
void Game::DrawLine(float x1, float y1, float x2, float y2, DWORD color) {
    device->getVideoDriver()->draw2DLine( core::position2d<s32>(x1, y1), core::position2d<s32>(x2, y2), SColor(color) );
}
void Game::DrawRect(float left, float top, float right, float bottom, DWORD color) {
    DrawRect(left, top, right, bottom, color, color, 0.0f);
}
void Game::DrawRect(float left, float top, float right, float bottom, DWORD color, DWORD fill_color) {
    DrawRect(left, top, right, bottom, color, fill_color, 0.0f);
}
void Game::DrawRect(float left, float top, float right, float bottom, DWORD color, DWORD fill_color, float z) {
    device->getVideoDriver()->draw2DRectangle( SColor(fill_color), core::rect<s32>(left, top, right, bottom) );
    if (color != fill_color) {
        device->getVideoDriver()->draw2DRectangleOutline( core::rect<s32>(left, top, right, bottom), SColor(color) );
    }

}

void Game::DrawCircle(float cx, float cy, float r, DWORD color) {
    device->getVideoDriver()->draw2DPolygon(core::position2d<s32>(cx, cy), r, color);
}
// TODO: использовать draw2DVertexPrimitiveList
void Game::DrawSector(float cx, float cy, float r, float sa, float ea, DWORD color) {
    float angle = sa;
    float step = 0.3f;
    if (step * r < 1.0f) {
        step = 1.0f / r;
    }

    if (angle > ea) {
        angle -= M_PI * 2;
    }
    float prev_x = cx + cosf(angle) * r;
    float prev_y = cy + sinf(angle) * r;
    float x, y;

    device->getVideoDriver()->draw2DLine(core::position2d<s32>(prev_x, prev_y), core::position2d<s32>(cx, cy), color);

    while(angle < ea) {
        angle += step;
        if (angle > ea)
            angle = ea;
        x = cx + cosf(angle) * r;
        y = cy + sinf(angle) * r;
        device->getVideoDriver()->draw2DLine(core::position2d<s32>(prev_x, prev_y), core::position2d<s32>(x, y), color);

        prev_x = x;
        prev_y = y;
    }

    device->getVideoDriver()->draw2DLine(core::position2d<s32>(x, y), core::position2d<s32>(cx, cy), color);
}


void Game::StretchUtilSpriteOnNode(std::string name, ISceneNode* node) {
    util_sprites[name]->StretchOnNode(node);
}

float Game::CastRay(float x, float y, float angle, float _default) {
    // float r_dx = cosf(angle);
    // float r_dy = sinf(angle);
    // float T1, T2;
    // float closest = _default;
    // for ( auto it = vision_blocks.begin(); it != vision_blocks.end(); ++it ) {
    //     float s_dx = it->x2 - it->x1;
    //     float s_dy = it->y2 - it->y1;

    //     // http://ncase.me/sight-and-light/
    //     // Solve for T2!
    //     T2 = (r_dx * (it->y1 - y) + r_dy * (x - it->x1) ) / (s_dx * r_dy - s_dy * r_dx);
    //     // Plug the value of T2 to get T1
    //     T1 = (it->x1 + s_dx * T2 - x) / r_dx;

    //     if (T1 > 0 && T1 < closest && T2 > 0 && T2 < 1) {
    //         closest = T1;
    //     }
    // }
    // return closest;
}



void Game::SetMousePos(float x, float y) {
    mouse_x = x;
    mouse_y = y;
}
void Game::SetMouseButtonsState(u32 state) {
    mouse_buttons_state = state;
    SetKeyState(KEY_LBUTTON, mouse_buttons_state & EMBSM_LEFT);
    SetKeyState(KEY_RBUTTON, mouse_buttons_state & EMBSM_RIGHT);
    SetKeyState(KEY_MBUTTON, mouse_buttons_state & EMBSM_MIDDLE);
}
void Game::SetMouseWheel(float wheel) {
    mouse_wheel = wheel;
}
void Game::SetKeyState(EKEY_CODE key, bool down) {
    key_state[key] = down;
}
void Game::PushInputChar(wchar_t _char) {
    input_chars.push(_char);
}
wchar_t Game::PopInputChar() {
    wchar_t _char = 0;
    if ( !input_chars.empty() ) {
        _char = input_chars.front();
        input_chars.pop();
    }
    return _char;
}
float Game::GetMouseX() {return mouse_x;}
float Game::GetMouseY() {return mouse_y;}
float Game::GetWorldMouseX() {return world_mouse_x;}
float Game::GetWorldMouseY() {return world_mouse_y;}
bool Game::GetKeyState(EKEY_CODE key) {
    return key_state[key];
}
bool Game::IsKeyDown(EKEY_CODE key) {
    return key_is_down[key];
}

Animation* Game::GetUtilSprite(std::string index) {
    return util_sprites[index];
}
Animation* Game::GetAnimation(TiXmlElement* element, char anchor) {
    // printf("texture %s\n", element->Attribute("texture"));
    video::ITexture* tex = GetDriver()->getTexture( element->Attribute("texture") );
    // GetDriver()->makeColorKeyTexture(tex, core::position2d<s32>(0,0));
    unsigned char nframes = atoi(element->Attribute("nframes"));
    float fps = atof(element->Attribute("fps"));
    float x = atof(element->Attribute("x"));
    float y = atof(element->Attribute("y"));
    float w = atof(element->Attribute("w"));
    float h = atof(element->Attribute("h"));
    // printf("creating animation %d %f %f %f %f %f... \n", nframes, fps, x, y, w, h);
    Animation* anim = new Animation(this, tex, x, y, w, h, nframes, fps);

    // printf("animation created\n");
    if ( element->Attribute("anchor_x") && element->Attribute("anchor_y") ) {
        anim->SetAnchor(
            atof( element->Attribute("anchor_x") ),
            atof( element->Attribute("anchor_y") )
        );
    }
    else {
        switch (anchor) {
            case ANIMATION_ANCHOR_AUTO:
            case ANIMATION_ANCHOR_MIDDLE_CENTER:
                anim->SetAnchor(w * 0.5f, h * 0.5f);
                break;
            case ANIMATION_ANCHOR_BOTTOM_CENTER:
                anim->SetAnchor(w * 0.5f, h);
                break;
            case ANIMATION_ANCHOR_TOP_LEFT:
                anim->SetAnchor(0, 0);
                break;
        }
    }
    // if ( element->Attribute("blend_mode") ) {
    //     anim->SetBlendMode( atoi( element->Attribute("blend_mode") ) );
    // }
    // else {
    //     anim->SetBlendMode(BLEND_DEFAULT);
    // }
    // anim->SetZ(0.0f);
    // printf("done\n");
    return anim;
}
Animation* Game::GetAnimation(std::string filename, char anchor) {
    if (animations.count(filename) == 1)
        return new Animation(*animations[filename]);

    printf( "loading animation %s ...\n", filename.c_str() );


    TiXmlDocument doc( filename.c_str() );
    bool loadOkay = doc.LoadFile();
    if (loadOkay) {
        // printf("load okay... ");
        TiXmlElement* xml = doc.FirstChildElement("animation");

        Animation* anim = GetAnimation(xml, anchor);
        animations[filename] = anim;

        return new Animation(*anim);
    } else {
        printf( "failed to load animation %s\n", filename.c_str() );
        // printf("\n");
        if (filename.compare("imgs/warning.xml") == 0) {
            return NULL;
        }
        else {
            animations[filename] = GetAnimation("imgs/warning.xml", anchor);
            return new Animation(*animations[filename]);
        }
    }
}
Animation* Game::GetAnimation(std::string filename) {
    return GetAnimation(filename, ANIMATION_ANCHOR_MIDDLE_CENTER);
}

irr::gui::IGUIFont* Game::GetFont() {return arial12;}

float Game::GetCameraX() {return camera_x;}
float Game::GetCameraY() {return camera_y;}

void Game::SetCamera(float x, float y) {
    camera->setPosition( vector3df(x, y + 2, camera_z) );
    camera->setTarget( vector3df(x, y, 0) );
}

short Game::GetWindowWidth() {return window_width;}
short Game::GetWindowHeight() {return window_height;}
short Game::GetHalfWindowWidth() {return half_window_width;}
short Game::GetHalfWindowHeight() {return half_window_height;}


void Game::SetControlledCharacter(Character* character) {
    controlled_character = character;
}
Character* Game::GetControlledCharacter() {
    return controlled_character;
}

void Game::SetGameStatus(char _game_status) {
    game_status = _game_status;
    printf("game status %d\n", game_status);
}
char Game::GetGameStatus() {
    return game_status;
}

void Game::FireEvent(char event) {
    switch (event) {
        // case GAME_EVENT_PASSWORD_INCORRECT:
        //     main_menu_window->SetPasswordError();
        //     break;
    }
}

bool Game::DebugOn() {
    return debug;
}


char* copyString(const char* original) {
    char* newString = new char[256];
    int i = 0;
    while (i < 255 && original[i] != '\0') {
        newString[i] = original[i];
        i++;
    }
    if (original[i - 1] == ' ') {
        i--;
    }
    newString[i] = '\0';

    return newString;
}

bool compareStrings(char* first, char* second) {
    int i = 0;
    while (i < 256) {
        if ( first[i] != second[i] )
            return false;
        if ( first[i] == '\0' )
            return true;
        i++;
    }
    return true;
}
