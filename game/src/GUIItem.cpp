#include "Lib.h"

GUIItem::GUIItem(Game* _game) {
    game = _game;
    width = 100;
    height = 20;
    half_width = width * 0.5f;
    half_height = height * 0.5f;
    focused = false;
}

GUIItem::~GUIItem() {
    //dtor
}

void GUIItem::Draw() {

}

void GUIItem::SetPosition(float _x, float _y) {x = _x; y = _y;}

void GUIItem::FocusIn() {focused = true;}
void GUIItem::FocusOut() {focused = false;}
bool GUIItem::IsFocused() {return focused;}
