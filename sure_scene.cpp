#include "sure_scene.hpp"
USING_NS_CC;
// 新建界面 //
CCScene* SureScene::scene(CCRenderTexture* sqr)
{
    
    CCScene *scene = CCScene::create();
    SureScene *layer = SureScene::create();
    scene->addChild(layer,1);
    // 获取界面大小 //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 将截取界面作为背景 //
    CCSprite *back_spr = CCSprite::createWithTexture(sqr->getSprite()->getTexture());
    back_spr->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    back_spr->setFlipY(true);
    // 背景暗化 //
    back_spr->setColor(cocos2d::ccGRAY);
    scene->addChild(back_spr);
    
    return scene;
}

bool SureScene::init()
{
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    // 得到窗口的大小 //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 问句标题 //
    auto title = LabelTTF::create("Are you sure?", "Arial", 20);
    title->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 + 25));
    this->addChild(title);
    // 继续游戏按钮 //
    continue_label = LabelTTF::create("NO", "Arial", 20);
    auto menu1 =CCMenuItemLabel::create(continue_label,this,menu_selector(SureScene::continueCallBack));
    menu1->setPosition(ccp(origin.x + visibleSize.width/2 + 25,origin.y + visibleSize.height/2 - 25));
    // 退出按钮 //
    exit_label = LabelTTF::create("YES", "Arial", 20);
    auto menu2 =CCMenuItemLabel::create(exit_label,this,menu_selector(SureScene::exitCallBack));
    menu2->setPosition(ccp(origin.x + visibleSize.width/2 - 25,origin.y + visibleSize.height/2 - 25));
    // 将按钮加入层 //
    CCMenu* pMenu = CCMenu::create(menu1, menu2,NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 2);
    
    return true;
}
// 继续游戏 //
void SureScene::continueCallBack(CCObject* pSender)
{
    //点击动画
    auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    continue_label->runAction(Sequence::create(scale1, scale2, NULL));
    //返回原界面
    CCDirector::sharedDirector()->popScene();
    
}
// 退出游戏 //
void  SureScene::exitCallBack(CCObject* pSender)
{
    //点击动画
    auto scale1 = CCScaleTo ::create(0.1f, 0.8f, 0.8f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    exit_label->runAction(Sequence::create(scale1, scale2, NULL));
    //退出程序
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}