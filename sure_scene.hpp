#ifndef sure_scene_hpp
#define sure_scene_hpp

#include "start_scene.hpp"
#include "cocos2d.h"

class SureScene : public cocos2d::Layer{
public:
    virtual bool init();
    static cocos2d::CCScene* scene(CCRenderTexture* sqr);
    void continueCallBack(CCObject* pSender);
    void exitCallBack(CCObject* pSender);
    CREATE_FUNC(SureScene);
private:
    LabelTTF* continue_label;
    LabelTTF* exit_label;
};
#endif /* sure_scene_hpp */
