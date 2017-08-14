#include "Lib.h"

GUIInput::GUIInput(Game* _game) : GUIItem(_game) {
    focused = false;
    game = _game;
}

GUIInput::~GUIInput() {
}

void GUIInput::Draw() {
    //    GUIItem::Draw();
    game->GetFont()->draw(
        text,
        core::rect<s32>(x - half_width, y - half_height, x + half_width, y + half_height),
        video::SColor(255,255,255,255),
        true, true
    );
    if (focused) {
        core::dimension2d<u32> size = game->GetFont()->getDimension( text.c_str() );
        game->GetFont()->draw(
            L"│",
            core::rect<s32>(x + size.Width * 0.5f, y, x + size.Width * 0.5f, y),
            video::SColor(255,255,255,255),
            false, true
        );

        wchar_t input_char = game->PopInputChar();
        if ( input_char ) {
            // printf("%d\n", input_char);
            switch (input_char) {
                case 8:
                    if ( !text.empty() ) {
                        text.erase(text.size() - 1);
                    }
                break;
                case 9:
                case 13:
                break;
                default:
                    text.append(input_char);
                break;
            }
        }
    }
}

core::stringw GUIInput::GetValue() {return text;}
