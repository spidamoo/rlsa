#include "Character.h"

Character::Character(Game* _game) {
    game = _game;

    x = y = 0.0f;

    node = game->GetSceneManager()->addCubeSceneNode(
        1, 0, -1,
        core::vector3df(x, y, 0.04f),
        core::vector3df(0, 0, 0),
        core::vector3df(1, 1, 1)
    );
    node->addShadowVolumeSceneNode();
    // node->setMaterialFlag(video::EMF_LIGHTING, false);

}

Character::~Character() {
    game->GetSceneManager()->addToDeletionQueue(node);
    // printf("~Character %d\n", id);
    // billboard->remove();
}

void Character::Update(float dt) {
    node->setPosition( vector3df(x, y, 0) );
}

void Character::Control(float dt) {
    // printf("Control Character\n");
    
    // printf("can_move %d\n", can_move);
    if (true) {
        char new_direction = 0;

        if ( game->GetKeyState(KEY_KEY_W) ) {
            if ( game->GetKeyState(KEY_KEY_A) ) {
                new_direction = CHARACTER_MOVE_DIRECTION_LEFTUP;
            } else if ( game->GetKeyState(KEY_KEY_D) ) {
                new_direction = CHARACTER_MOVE_DIRECTION_RIGHTUP;
            } else {
                new_direction = CHARACTER_MOVE_DIRECTION_UP;
            }
        } else if ( game->GetKeyState(KEY_KEY_S) ) {
            if ( game->GetKeyState(KEY_KEY_A) ) {
                new_direction = CHARACTER_MOVE_DIRECTION_LEFTDOWN;
            } else if ( game->GetKeyState(KEY_KEY_D) ) {
                new_direction = CHARACTER_MOVE_DIRECTION_RIGHTDOWN;
            } else {
                new_direction = CHARACTER_MOVE_DIRECTION_DOWN;
            }
        } else if ( game->GetKeyState(KEY_KEY_A) ) {
            new_direction = CHARACTER_MOVE_DIRECTION_LEFT;
        } else if ( game->GetKeyState(KEY_KEY_D) ) {
            new_direction = CHARACTER_MOVE_DIRECTION_RIGHT;
        }

        if (new_direction) {
            x += CHARACTER_DIRECTION_SPEED_C[new_direction][0] * 2 * dt;
            y += CHARACTER_DIRECTION_SPEED_C[new_direction][1] * 2 * dt;
        }
    }
    // printf("end Control Character\n");
}

void Character::Draw() {

}


float Character::GetX() {return x;}
float Character::GetY() {return y;}
