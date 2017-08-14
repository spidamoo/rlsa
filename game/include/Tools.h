#ifndef TOOLS_H
#define TOOLS_H

#include <Lib.h>

const DWORD PROFILER_COLORS[16] = {
    0xFFFF66, 0xFFCC33, 0xCC9966, 0xFF6600, 0xFF6666, 0xFF0066, 0xFF33CC, 0x9933FF,
    0x3300CC, 0x0099FF, 0x339999, 0x66FFFF, 0x33FFCC, 0x33FF66, 0x66CC66, 0xCCFF33
};

class Profiler {
    public:
        Profiler(unsigned short max_stages);
        virtual ~Profiler();

        void SetStageName(unsigned short stage, std::string name);
        void StartStage(unsigned short stage);
        void StopStage(unsigned short stage);
        void Reset();
        double GetStageSum(unsigned short stage);
        double GetStageAverage(unsigned short stage);
        void Draw(Game* game, float x, float y);
    protected:
        unsigned short capacity;

        std::chrono::steady_clock::time_point* stage_points;
        double* stage_totals;
        unsigned long* stage_counts;
        unsigned short* stage_parents;
        std::string* stage_names;

        std::stack<short> running_stages;
};

class MyEventReceiver : public IEventReceiver {
    public:
        // This is the one method that we have to implement
        virtual bool OnEvent(const SEvent& event);
        
        MyEventReceiver() {
            game = NULL;
        }

        void SetGame(Game* _game) {
            game = _game;
        }

    private:
        Game* game;
};

const char ANIMATION_ANCHOR_AUTO          = 0;
const char ANIMATION_ANCHOR_MIDDLE_CENTER = 1;
const char ANIMATION_ANCHOR_BOTTOM_CENTER = 2;
const char ANIMATION_ANCHOR_TOP_LEFT      = 3;

class Animation {
    public:
        Animation(Game* game, video::ITexture* texture, float x, float y, float w, float h, unsigned char frames = 0, float fps = 0.0f);
        virtual ~Animation();

        void SetAnchor(float x, float y);

        float GetWidth();
        float GetHeight();
        float GetAnchorX();
        float GetAnchorY();

        void Draw(float x, float y, video::SColor color, float sx, float sy);
        void Draw(float x, float y, video::SColor color = 0xFFFFFFFF, float sx = 1.0f);
        void StretchOnNode(ISceneNode* node);
    protected:
        Game* game;
        video::ITexture* texture;
        float x, y, w, h, ax, ay;
        unsigned char frames;
        float fps;
};


#endif // TOOLS_H