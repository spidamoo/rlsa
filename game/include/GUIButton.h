#ifdef GUIITEM_H
#ifndef GUIBUTTON_H
#define GUIBUTTON_H


class GUIButton : public GUIItem {
    public:
        GUIButton(Game* _game, stringw text, std::function<void(void)> _onpress);
        virtual ~GUIButton();

        void Draw();

    protected:
        stringw text;
        std::function<void(void)> onpress;

    private:
};

#endif // GUIBUTTON_H
#endif // GUIITEM_H
