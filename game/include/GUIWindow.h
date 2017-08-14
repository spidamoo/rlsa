#ifndef GUIWINDOW_H
#define GUIWINDOW_H


class GUIWindow
{
    public:
        GUIWindow(Game* _game);
        virtual ~GUIWindow();

        void AddChild(GUIItem* child);

        virtual void Draw();

    protected:
        Game* game;
        float x, y;

        std::forward_list<GUIItem*> children;

    private:
};

#endif // GUIWINDOW_H
