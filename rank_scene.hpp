#ifndef rank_scene_hpp
#define rank_scene_hpp

#include "start_scene.hpp"
#include "game_scene.hpp"
#include "cocos2d.h"

class RankScene : public cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void goToStart(cocos2d::Ref* pSender);
    void back(CCObject* pSender);
    void fresh();
    CREATE_FUNC(RankScene);
private:
    LabelTTF* rank_label_[5];
    MenuItemImage* back_label_;
};

#endif /* rank_scene_hpp */
