#include "load_scene.hpp"
USING_NS_CC;
using namespace CocosDenshion;
// 新建场景 //
Scene* LoadScene::createScene(){
    auto scene = Scene::create();
    auto layer = LoadScene::create();
    scene->addChild(layer);
    return scene;
}
// 初始化场景 //
bool LoadScene::init(){
    if (!Layer::create()) {
        return false;
    }
    // 获取界面大小 //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // Loading...文字 //
    auto load_label = LabelTTF::create("Loading...","Arial",20);
    load_label->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 + 30));
    this->addChild(load_label,1);
    // 进度条边框 //
    auto load_start = CCSprite::create("LoadStart.png");
    load_start->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2));
    load_start->setScale(3.0f);
    this->addChild(load_start,1);
    // 进度条 //
    auto load_over = CCProgressTimer::create(CCSprite::create("LoadOver.png"));
    load_over->setPercentage(1.0f);
    load_over->setType(ProgressTimer::Type::BAR);
    // 加载动画 //
    load_over->setBarChangeRate(ccp(1, 0));
    load_over->setMidpoint(ccp(0, 0));
    load_over->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2));
    load_over->setScale(3.0f);
    this->addChild(load_over,2);
    auto action = CCProgressTo::create(2, 100);
    load_over->runAction( CCRepeatForever::create(action) );
    // 调用加载成功函数 //
    this->scheduleOnce(schedule_selector(LoadScene::loadSuccess), 2);
    return true;
}
// 加载成功的操作 //
void LoadScene::loadSuccess(float pFloat){
    //加载音乐
    SimpleAudioEngine::getInstance()->preloadEffect("get.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("gamewin.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("gameover.mp3");
    //切换场景
    Director::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, StartScene::createScene(), ccColor3B(255, 255, 255)));
}