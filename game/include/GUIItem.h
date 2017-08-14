#ifndef GUIITEM_H
#define GUIITEM_H


class GUIItem {
    public:
        GUIItem(Game* _game);
        virtual ~GUIItem();

        virtual void Draw();
        virtual void SetPosition(float x, float y);

        void FocusIn(), FocusOut();
        bool IsFocused();

    protected:

        float x, y, width, height, half_width, half_height;
        bool focused;
        Game* game;

    private:
};

#endif // GUIITEM_H
