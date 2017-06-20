#ifndef pause_scene_hpp
#define pause_scene_hpp

#include "game_scene.hpp"
#include "start_scene.hpp"
#include "cocos2d.h"

class PauseScene : public cocos2d::Layer{
public:
    virtual bool init();
    static cocos2d::CCScene* scene(CCRenderTexture* sqr);
    void continueCallBack(CCObject* pSender);
    void restart(CCObject* pSender);
    void back(CCObject* pSender);
    CREATE_FUNC(PauseScene);
private:
    LabelTTF* continue_label;
    LabelTTF* restart_label;
    LabelTTF* back_label;
};

#endif /* pause_scene_hpp */
