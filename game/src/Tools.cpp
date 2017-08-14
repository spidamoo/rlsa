#include "Tools.h"

using namespace std::chrono;
Profiler::Profiler(unsigned short _max_stage)  {
    capacity = _max_stage + 1;

    stage_points  = new steady_clock::time_point[capacity];
    stage_totals  = new double[capacity];
    stage_counts  = new unsigned long[capacity];
    stage_parents = new unsigned short[capacity];
    stage_names   = new std::string[capacity];

    Reset();
}

Profiler::~Profiler() {
    delete[] stage_points;
    delete[] stage_totals;
    delete[] stage_counts;
    delete[] stage_parents;
    delete[] stage_names;
}

void Profiler::SetStageName(unsigned short stage, std::string name) {
    stage_names[stage] = name;
}

void Profiler::StartStage(unsigned short stage) {
    stage_points[stage] = steady_clock::now();
    if ( !running_stages.empty() ) {
        stage_parents[stage] = running_stages.top();
    }
    running_stages.push(stage);
}

void Profiler::StopStage(unsigned short stage) {
    steady_clock::time_point stoptime = steady_clock::now();
    stage_totals[stage] += duration_cast<duration<double>>(stoptime - stage_points[stage]).count();
    stage_counts[stage]++;
    running_stages.pop();
}

void Profiler::Reset() {
    memset( stage_totals,  0, sizeof(double)        * capacity );
    memset( stage_counts,  0, sizeof(unsigned long) * capacity );
    memset( stage_parents, 0, sizeof(unsigned short) * capacity );
}

double Profiler::GetStageSum(unsigned short stage) {
    return stage_totals[stage];
}
double Profiler::GetStageAverage(unsigned short stage) {
    return stage_totals[stage] / stage_counts[stage];
}

void Profiler::Draw(Game* game, float x, float y) {
    float parent_x[capacity];
    unsigned char level[capacity];

    memset(parent_x, 0, sizeof(float) * capacity);
    memset(level, 0, sizeof(unsigned char) * capacity);

    float bars_origin_y = 20.0f;
    float bar_height    = 10.0f;
    float max_width     = 100.0f;
    float k             = 0.0f;
    float shift_x       = 0.0f;
    float shift_y       = 0.0f;
    float bar_width     = 0.0f;

    float target_number = 0.0f;
    unsigned short target_stage = 0;

    for (unsigned short i = 1; i < capacity; i++) {
        if (stage_counts[i] == 0) {
            continue;
        }
        // printf("stage %d %d\n", i, stage_parents[i]);
        float number = stage_totals[i] / stage_counts[1];
        if (stage_parents[i]) {
            bar_width = number * k;
            shift_x  = parent_x[ stage_parents[i] ];
            parent_x[ stage_parents[i] ] += bar_width;
            parent_x[i] = shift_x;
            level[i] = level[ stage_parents[i] ] + 1;
            shift_y  = level[i] * bar_height;
        }
        else {
            bar_width = max_width;
            k = bar_width / number;
            shift_x = 0.0f;
            shift_y = 0.0f;
            level[i] = 0;
        }
        DWORD color = 0xFF000000 + PROFILER_COLORS[i % 0xF];
        game->DrawRect(
            x + shift_x, y + bars_origin_y + shift_y,
            x + shift_x + bar_width, y + bars_origin_y + shift_y + bar_height,
            color
        );

        if (
            game->GetMouseX() >= x + shift_x             && game->GetMouseY() >= y + bars_origin_y + shift_y &&
            game->GetMouseX() <= x + shift_x + bar_width && game->GetMouseY() <= y + bars_origin_y + shift_y + bar_height
        ) {
            target_stage  = i;
            target_number = number;
        }
    }

    if (target_stage) {
        if ( stage_names[target_stage].empty() ) {
            game->_printf(x, y, ALIGN_LEFT, "%d: %.6f\n", target_stage, target_number);
        }
        else {
            game->_printf(x, y, ALIGN_LEFT, "%s: %.6f\n", stage_names[target_stage].c_str(), target_number);
        }
    }
}


bool MyEventReceiver::OnEvent(const SEvent& event) {
    switch (event.EventType) {
        case EET_KEY_INPUT_EVENT:
            game->SetKeyState(event.KeyInput.Key, event.KeyInput.PressedDown);
            if (event.KeyInput.Char) {
                game->PushInputChar(event.KeyInput.Char);
            }
            break;
        case EET_MOUSE_INPUT_EVENT:
            game->SetMousePos(event.MouseInput.X, event.MouseInput.Y);
            game->SetMouseButtonsState(event.MouseInput.ButtonStates);
            // printf("mouse %f\n", event.MouseInput.Wheel);
            game->SetMouseWheel(event.MouseInput.Wheel);
            break;
    }
    return false;
}


Animation::Animation(Game* _game, video::ITexture* _texture, float _x, float _y, float _w, float _h, unsigned char _frames, float _fps) {
    game = _game;
    texture = _texture;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    frames = _frames;
    fps = _fps;

    ax = 0.0f;
    ay = 0.0f;
}

Animation::~Animation() {
}

float Animation::GetWidth() {
    return w;
}
float Animation::GetHeight() {
    return h;
}
float Animation::GetAnchorX() {
    return ax;
}
float Animation::GetAnchorY() {
    return ay;
}

void Animation::SetAnchor(float _ax, float _ay) {
    ax = _ax;
    ay = _ay;
}

void Animation::Draw(float _x, float _y, video::SColor color, float sx) {
    Draw(_x, _y, color, sx, sx);
}
void Animation::Draw(float _x, float _y, video::SColor color, float sx, float sy) {
    video::SColor colors[] = {color, color, color, color};
    game->GetDriver()->draw2DImage(
        texture, core::rect<s32>(_x - ax * sx, _y - ay * sy, _x - (ax - w) * sx, _y - (ay - h) * sy),
        core::rect<s32>(x, y, x + w, y + h), 0,
        colors, true
    );
}
void Animation::StretchOnNode(ISceneNode* node) {
    node->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setMaterialTexture(0, texture);
    node->getMaterial(0).setFlag(EMF_BILINEAR_FILTER, false);

    core::dimension2d<u32> tex_size = texture->getOriginalSize();
    float sx = w / tex_size.Width;
    float sy = h / tex_size.Height;
    node->getMaterial(0).getTextureMatrix(0).setTextureScale(sx, sy);

    float u = x / tex_size.Width;
    float v = y / tex_size.Height;
    node->getMaterial(0).getTextureMatrix(0).setTextureTranslate(u, v);
}
