#ifndef CHARACTER_H
#define CHARACTER_H

#include <Lib.h>

const  char CHARACTER_MOVE_DIRECTION_IDLE      = 0;
const  char CHARACTER_MOVE_DIRECTION_RIGHT     = 1;
const  char CHARACTER_MOVE_DIRECTION_RIGHTDOWN = 2;
const  char CHARACTER_MOVE_DIRECTION_DOWN      = 3;
const  char CHARACTER_MOVE_DIRECTION_LEFTDOWN  = 4;
const  char CHARACTER_MOVE_DIRECTION_LEFT      = 5;
const  char CHARACTER_MOVE_DIRECTION_LEFTUP    = 6;
const  char CHARACTER_MOVE_DIRECTION_UP        = 7;
const  char CHARACTER_MOVE_DIRECTION_RIGHTUP   = 8;

const float CHARACTER_DIRECTION_SPEED_C[9][2] = {
    { 0.000f,  0.000f},
    { 1.000f,  0.000f},
    { 0.707f,  0.707f},
    { 0.000f,  1.000f},
    {-0.707f,  0.707f},
    {-1.000f,  0.000f},
    {-0.707f, -0.707f},
    { 0.000f, -1.000f},
    { 0.707f, -0.707f}
};

class Character {
    public:

        Character(Game* game);
        virtual ~Character();

        void Update(float dt);
        void Control(float dt);
        void Draw();

        float GetX();
        float GetY();
    protected:
        Game* game;

        float x, y;

        IMeshSceneNode* node;
    private:
};

#endif // CHARACTER_H
