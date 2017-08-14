#include "Lib.h"

GUIWindow::GUIWindow(Game* _game)
{
    game = _game;
}

GUIWindow::~GUIWindow()
{
    //dtor
}

void GUIWindow::AddChild(GUIItem* child) {
    children.push_front(child);
}

void GUIWindow::Draw() {
    for ( auto it = children.begin(); it != children.end(); ++it ) {
        (*it)->Draw();
    }
}
