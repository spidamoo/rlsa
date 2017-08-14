#ifdef GUIITEM_H
#ifndef GUIINPUT_H
#define GUIINPUT_H


class GUIInput : public GUIItem {
    public:
        GUIInput(Game* _game);
        virtual ~GUIInput();

        stringw GetValue();

        void Draw();

    protected:
        stringw text;
        Game* game;

    private:
};

#endif // GUIINPUT_H
#endif // GUIITEM_H
