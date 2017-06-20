#ifndef game_scene_hpp
#define game_scene_hpp

#include "cocos2d.h"
#include "card_settings.hpp"
#include "start_scene.hpp"
#include "pause_scene.hpp"
#include "rank_scene.hpp"
#include "win_or_lose.hpp"
#include "SimpleAudioEngine.h"
#include <iostream>
using namespace std;
USING_NS_CC;

class GameScene : public cocos2d::Layer{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void pauseCallBack(cocos2d::Ref* pSender);
    void soundCallBack(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    bool doLeft();
    bool doRight();
    bool doUp();
    bool doDown();
    void randomCreate();
    void createCards();
    void gameWin();
    void gameOver();
    void setRank();
    void resetRank();
    CREATE_FUNC(GameScene);
private:
    std::vector<int>rank_;
    int level_;
    int score_ = 0;
    int best_score_ = 0;
    int size_;
    bool sound_;
    CardSprite *cardArr_[6][6];
    CardSprite *cardArrAction_[6][6];
    int offset_x_;
    int offset_y_;
    timeval tv_;
    CCPoint start_;
    CCPoint end_;
    LabelTTF* score_label_;
    LabelTTF* score_value_;
    Menu* sound_icon_;
    MenuItemImage* sound_on_;
    MenuItemImage* sound_off_;
};

#endif /* game_scene_hpp */
