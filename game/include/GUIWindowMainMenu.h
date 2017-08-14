
#ifndef GUIWINDOWMAINMENU_H
#define GUIWINDOWMAINMENU_H


class GUIWindowMainMenu : public GUIWindow {
    public:
        GUIWindowMainMenu(Game* _game);
        virtual ~GUIWindowMainMenu();

        char Draw(float dt);


    protected:

    private:
};

#endif // GUIWINDOWMAINMENU_H

