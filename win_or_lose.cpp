#define UserDefault CCUserDefault::sharedUserDefault()
#include "win_or_lose.hpp"
USING_NS_CC;
// 新建场景 //
CCScene* LoseScene::scene(CCRenderTexture* sqr)
{
    
    CCScene *scene = CCScene::create();
    LoseScene *layer = LoseScene::create();
    scene->addChild(layer,1);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    CCSprite *back_spr = CCSprite::createWithTexture(sqr->getSprite()->getTexture());
    back_spr->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    back_spr->setFlipY(true);
    back_spr->setColor(cocos2d::ccGRAY); //背景暗化
    scene->addChild(back_spr);
    
    return scene;
}
// 初始化场景 //
bool LoseScene::init()
{
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 判断是输了还是赢了 //
    bool result = UserDefault->getBoolForKey("result");
    if(result)//赢了
    {
        auto title = LabelTTF::create("You Win!", "Arial", 50);
        title->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height - 150));
        this->addChild(title);
    }
    else//输了
    {
        auto title = LabelTTF::create("You Lose!", "Arial", 50);
        title->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height - 150));
        this->addChild(title);
    }
    // 显示本局分数 //
    auto score_value = LabelTTF::create("0", "Arial", 30);
    int score = UserDefault->getIntegerForKey("score");//提取分数
    score_value->setString(String::createWithFormat("%d", score)->getCString());//将数字转为字符
    score_value->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 + 30));//设置位置
    addChild(score_value);//加入层
    // 最高分 //
    auto best_label = LabelTTF::create("Best Score:", "Arial", 15);
    best_label->setPosition(ccp(origin.x + visibleSize.width/2 - 60,origin.y + visibleSize.height/2 - 20));//设置位置
    addChild(best_label);//加入层
    auto best_value = LabelTTF::create("0", "Arial", 20);
    int best = UserDefault->getIntegerForKey("best_score");//提出最高分
    best_value->setString(String::createWithFormat("%d", best)->getCString());//将数字转为字符
    best_value->setPosition(ccp(origin.x + visibleSize.width/2 + 40,origin.y + visibleSize.height/2 - 20));//设置位置
    addChild(best_value);
    // 重新开始按钮 //
    continue_label = LabelTTF::create("Restart", "Arial", 20);
    auto menu1 =CCMenuItemLabel::create(continue_label,this,menu_selector(LoseScene::continueCallBack));
    menu1->setPosition(ccp(origin.x + visibleSize.width/2 + 45,origin.y + visibleSize.height/2 - 100));
    // 返回菜单按钮 //
    exit_label = LabelTTF::create("Back", "Arial", 20);
    auto menu2 =CCMenuItemLabel::create(exit_label,this,menu_selector(LoseScene::exitCallBack));
    menu2->setPosition(ccp(origin.x + visibleSize.width/2 - 45,origin.y + visibleSize.height/2 - 100));
    
    CCMenu* pMenu = CCMenu::create(menu1, menu2,NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 2);
    
    return true;
}
// 重新开始 //
void LoseScene::continueCallBack(CCObject* pSender)
{   auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    continue_label->runAction(Sequence::create(scale1, scale2, NULL));
    Director::sharedDirector()->replaceScene(CCTransitionFadeUp::create(0.5f,GameScene::createScene()));//从下面翻入
}
// 返回菜单 //
void  LoseScene::exitCallBack(CCObject* pSender)
{
    auto scale1 = CCScaleTo ::create(0.1f, 0.8f, 0.8f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    exit_label->runAction(Sequence::create(scale1, scale2, NULL));
    Director::sharedDirector()->replaceScene(CCTransitionMoveInL::create(0.5f,StartScene::createScene()));//从左边移入
}
