#ifndef FIRSTINIT_H
#define FIRSTINIT_H


#define FIRSTINIT_CONFIG_PATH "/config/init.xml"

class firstinit
{
public:
    static bool getIsFirstInit();
    static void updateInitNode(bool isInit);
};

#endif // FIRSTINIT_H
