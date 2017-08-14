#include "Lib.h"

GUIButton::GUIButton(Game* _game, stringw _text, std::function<void(void)> _onpress) : GUIItem(_game) {
    text = _text;
    onpress = _onpress;
}

GUIButton::~GUIButton() {
}

void GUIButton::Draw() {
    //    GUIItem::Draw();
    video::SColor color = video::SColor(255, 255, 255, 255);
    if (
        game->GetMouseX() > x - half_width && game->GetMouseY() > y - half_height
        && game->GetMouseX() < x + half_width && game->GetMouseY() < y + half_height
    ) {
        color = video::SColor(255, 0, 0, 255);
        if ( game->IsKeyDown(KEY_LBUTTON) ) {
            onpress();
        }
    }
    game->GetFont()->draw(
        text,
        core::rect<s32>(x - half_width, y - half_height, x + half_width, y + half_height),
        color,
        true, true
    );
    // if (focused) {

    // }
}


