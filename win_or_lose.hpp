#ifndef win_or_lose_hpp
#define win_or_lose_hpp

#include "start_scene.hpp"
#include "game_scene.hpp"
#include "cocos2d.h"

class LoseScene : public cocos2d::Layer{
public:
    virtual bool init();
    static cocos2d::CCScene* scene(CCRenderTexture* sqr);
    void continueCallBack(CCObject* pSender);
    void exitCallBack(CCObject* pSender);
    CREATE_FUNC(LoseScene);
private:
    LabelTTF* continue_label;
    LabelTTF* exit_label;
};

#endif /* win_or_lose_hpp */
