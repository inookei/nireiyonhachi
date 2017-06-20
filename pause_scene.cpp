#include "pause_scene.hpp"
USING_NS_CC;
// 新建界面 //
CCScene* PauseScene::scene(CCRenderTexture* sqr)
{
    CCScene *scene = CCScene::create();
    PauseScene *layer = PauseScene::create();
    scene->addChild(layer,1);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //将截取界面作为背景
    CCSprite *back_spr = CCSprite::createWithTexture(sqr->getSprite()->getTexture());
    back_spr->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    back_spr->setFlipY(true);
    //背景暗化
    back_spr->setColor(cocos2d::ccGRAY);
    scene->addChild(back_spr);
    
    return scene;
}
// 初始化界面 //
bool PauseScene::init()
{
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    // 获取界面大小 //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 继续游戏按钮 //
    continue_label = LabelTTF::create("BACK", "Arial", 20);
    auto menu1 =CCMenuItemLabel::create(continue_label,this,menu_selector(PauseScene::continueCallBack));
    menu1->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 + 50));
    // 重新开始游戏按钮 //
    restart_label = LabelTTF::create("RESTART", "Arial", 20);
    auto menu2 =CCMenuItemLabel::create(restart_label,this,menu_selector(PauseScene::restart));
    menu2->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2));
    // 返回主界面按钮 //
    back_label = LabelTTF::create("MENU", "Arial", 20);
    auto menu3 =CCMenuItemLabel::create(back_label,this,menu_selector(PauseScene::back));
    menu3->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 - 50));
    
    CCMenu* pMenu = CCMenu::create(menu1, menu2, menu3,NULL);
    pMenu->setPosition(CCPointZero);  
    this->addChild(pMenu, 2); 
    
    return true;  
}
// 继续游戏 //
void PauseScene::continueCallBack(CCObject* pSender)
{   auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    continue_label->runAction(Sequence::create(scale1, scale2, NULL));
    CCDirector::sharedDirector()->popScene();  
    
}  
// 重新开始游戏 //
void  PauseScene::restart(CCObject* pSender)
{
    auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    restart_label->runAction(Sequence::create(scale1, scale2, NULL));
    CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(0.2f,GameScene::createScene()));
}  
// 返回主界面 //
void  PauseScene::back(CCObject* pSender)
{
    auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    back_label->runAction(Sequence::create(scale1, scale2, NULL));
    CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInL::create(0.5f, StartScene::createScene()));
}