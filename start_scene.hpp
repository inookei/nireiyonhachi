#ifndef start_scene_hpp
#define start_scene_hpp

#include "cocos2d.h"
#include "game_scene.hpp"
#include "sure_scene.hpp"

class StartScene : public cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void goToStart(cocos2d::Ref* pSender);
    void goToRank(cocos2d::Ref* pSender);
    void goToExit(cocos2d::Ref* pSender);
    void upCallBack(cocos2d::Ref* pSender);
    void downCallBack(cocos2d::Ref* pSender);
    CREATE_FUNC(StartScene);
private:
    cocos2d::MenuItemImage* up_;
    cocos2d::MenuItemImage* down_;
    cocos2d::LabelTTF* start_;
    cocos2d::LabelTTF* rank_;
    cocos2d::LabelTTF* exit_;
    cocos2d::LabelTTF* level_label_;
    int level_;
};

#endif /* start_scene_hpp */
