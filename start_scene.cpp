#define UserDefault CCUserDefault::sharedUserDefault()

#include "start_scene.hpp"
#include <iostream>
#include <cstdlib>
USING_NS_CC;
// 新建界面 //
Scene* StartScene::createScene(){
    auto scene = Scene::create();
    auto layer = StartScene::create();
    scene->addChild(layer);
    return scene;
}
// 初始化界面 //
bool StartScene::init(){
    if ( !Layer::init() )
    {
        return false;
    }
    // 设置背景颜色 //
    auto background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);
    // 获取界面大小 //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 标题 //
    auto title = Sprite::create("Title.png");
    title->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 130));
    this->addChild(title, 1);
    // 显示阶数 //
    level_label_ = LabelTTF::create("4", "Arial", 20);
    level_label_->setColor(Color3B(0, 0, 0));
    level_label_->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 + 50));
    level_ = UserDefault->getIntegerForKey("level");
    level_label_->setString(String::createWithFormat("%d", level_)->getCString());
    auto menu0 =CCMenuItemLabel::create(level_label_);
    // 调整阶数 //
    up_ = MenuItemImage::create("LevelUp.png","LevelUp.png",
                                 CC_CALLBACK_1(::StartScene::upCallBack, this));
    up_->setPosition(ccp(origin.x + visibleSize.width/2 + 30,origin.y + visibleSize.height/2 + 50));
    down_ = MenuItemImage::create("LevelDown.png","LevelDown.png",
                                   CC_CALLBACK_1(::StartScene::downCallBack, this));
    down_->setPosition(ccp(origin.x + visibleSize.width/2 - 30,origin.y + visibleSize.height/2 + 50));
    // 开始按钮 //
    start_ = LabelTTF::create("START", "Arial", 20);
    start_->setColor(Color3B(0, 0, 0));
    auto menu1 =CCMenuItemLabel::create(start_,this,menu_selector(StartScene::goToStart));
    menu1->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2));
    // 排行榜按钮 //
    rank_ = LabelTTF::create("RANK", "Arial", 20);
    rank_->setColor(Color3B(0, 0, 0));
    auto menu2 =CCMenuItemLabel::create(rank_,this,menu_selector(StartScene::goToRank));
    menu2->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 - 50));
    // 退出按钮 //
    exit_ = LabelTTF::create("EXIT", "Arial", 20);
    exit_->setColor(Color3B(0, 0, 0));
    auto menu3 =CCMenuItemLabel::create(exit_,this,menu_selector(StartScene::goToExit));
    menu3->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 - 100));
    // 加入层 //
    auto pMenu = CCMenu::create(up_, down_, menu0, menu1, menu2, menu3, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 2);
    
    return true;
}
// 阶数调高 //
void StartScene::upCallBack(cocos2d::Ref* pSender){
    if(level_ < 6)//最大阶数为6
    {
        //点击动画
        auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
        auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
        up_->runAction(Sequence::create(scale1, scale2, NULL));
        level_ += 1;
        //将阶数转换成字符显示出来
        char buf[8];
        sprintf(buf, "%d", level_);
        level_label_->setString(buf);
    }
}
// 阶数调低 //
void StartScene::downCallBack(cocos2d::Ref* pSender){
    if(level_ > 4)//最小阶数为4
    {
        //点击动画
        auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
        auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
        down_->runAction(Sequence::create(scale1, scale2, NULL));
        level_ -= 1;
        //将阶数转换成字符显示出来
        char buf[8];
        sprintf(buf, "%d", level_);
        level_label_->setString(buf);
    }
}
// 开始游戏 //
void StartScene::goToStart(Ref* pSender){
    //阶数存入本地数据 游戏初始化时引用
    UserDefault->setIntegerForKey("level",level_);
    //点击动画
    auto scale1 = CCScaleTo ::create(0.1f, 0.8f, 0.8f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    start_->runAction(Sequence::create(scale1, scale2, NULL));
    //转入游戏界面
    Director::sharedDirector()->replaceScene(TransitionMoveInR::create(0.5f,GameScene::createScene()));
}
// 进入排行榜 //
void StartScene::goToRank(Ref* pSender){
    //点击动画
    auto scale1 = CCScaleTo ::create(0.1f, 0.8f, 0.8f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    rank_->runAction(Sequence::create(scale1, scale2, NULL));
    //转入排行榜界面
    Director::sharedDirector()->replaceScene(TransitionMoveInR::create(0.5f,RankScene::createScene()));
}
// 退出游戏 //
void StartScene::goToExit(Ref* pSender){
    //截取画面 进入确定界面
    CCRenderTexture *renderTexture = CCRenderTexture::create(320,480);
    renderTexture->begin();
    this->getParent()->visit();
    renderTexture->end();
    CCDirector::sharedDirector()->pushScene(SureScene::scene(renderTexture));
}