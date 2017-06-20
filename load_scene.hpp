#ifndef load_scene_hpp
#define load_scene_hpp

#include "cocos2d.h"
#include "start_scene.hpp"
#include "SimpleAudioEngine.h"

class LoadScene : cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void loadSuccess(float pFloat);
    CREATE_FUNC(LoadScene);
};

#endif /* load_scene_hpp */
