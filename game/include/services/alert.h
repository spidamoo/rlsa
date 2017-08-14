#ifndef PROJECT_ALERT_H
#define PROJECT_ALERT_H

class Alert {
    public:
        static VisualGame *game;

        static void Init(VisualGame *game);
        static void Error(std::string title, std::string msg);
};

#endif //PROJECT_ALERT_H
