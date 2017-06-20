#define UserDefault CCUserDefault::sharedUserDefault()
#include "rank_scene.hpp"
USING_NS_CC;
// 新建界面 //
Scene* RankScene::createScene(){
    auto scene = Scene::create();
    auto layer = RankScene::create();
    scene->addChild(layer);
    return scene;
}
// 初始化界面 //
bool RankScene::init(){
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
    // 添加标题 //
    auto title = LabelTTF::create("Rank", "Arial", 50);
    title->setColor(ccColor3B(0, 0, 0));
    title->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2 + 100));
    this->addChild(title);
    // 设置名次label //
    auto first = LabelTTF::create("1st", "Arial", 15);
    first->setColor(Color3B(255,215,0));
    first->setPosition(ccp(origin.x + visibleSize.width/2 - 50,origin.y + visibleSize.height/2 + 30));
    this->addChild(first);
    auto second = LabelTTF::create("2nd", "Arial", 15);
    second->setColor(Color3B(255,69,0));
    second->setPosition(ccp(origin.x + visibleSize.width/2 - 50,origin.y + visibleSize.height/2 + 5));
    this->addChild(second);
    auto third = LabelTTF::create("3rd", "Arial", 15);
    third->setColor(Color3B(0,100,0));
    third->setPosition(ccp(origin.x + visibleSize.width/2 - 50,origin.y + visibleSize.height/2 - 20));
    this->addChild(third);
    auto forth = LabelTTF::create("4th", "Arial", 15);
    forth->setColor(Color3B(0,0,0));
    forth->setPosition(ccp(origin.x + visibleSize.width/2 - 50,origin.y + visibleSize.height/2 - 45));
    this->addChild(forth);
    auto fifth = LabelTTF::create("5th", "Arial", 15);
    fifth->setColor(Color3B(0,0,0));
    fifth->setPosition(ccp(origin.x + visibleSize.width/2 - 50,origin.y + visibleSize.height/2 - 70));
    this->addChild(fifth);
    // 返回键 //
    back_label_ = MenuItemImage::create("Back.png", "Back.png", CC_CALLBACK_1(::RankScene::back, this));
    back_label_->setPosition(ccp(origin.x + visibleSize.width - 60, 40));
    auto Button = Menu::create(back_label_, NULL);
    Button->setPosition(CCPointZero);
    this->addChild(Button);
    // 显示分数 //
    for(int i = 0;i < 5;i++)
    {
        rank_label_[i] = LabelTTF::create("0", "Arial", 20);
        rank_label_[i]->setColor(Color3B(0,0,0));
        rank_label_[i]->setPosition(ccp(origin.x + visibleSize.width/2 + 50,origin.y + visibleSize.height/2 - 25 * i + 30));//位置与名次对齐
        this->addChild(rank_label_[i]);
    }
    //如果有本地纪录 就刷新数据
    if(UserDefault->getBoolForKey("game", false))
    {
        fresh();
    }
    return true;
}
// 返回操作 //
void RankScene::back(CCObject* pSender)
{
    //点击动画
    auto scale1 = CCScaleTo ::create(0.1f, 0.7f, 0.7f);
    auto scale2 = CCScaleTo ::create(0.1f, 1.0f, 1.0f);
    back_label_->runAction(Sequence::create(scale1, scale2, NULL));
    //转入开始界面
    CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInL::create(0.5f, StartScene::createScene()));
}
// 刷新数据 //
void RankScene::fresh(){
    //获取前五名的数据 并分别写入label数组
    int score = UserDefault->getIntegerForKey("first");
    rank_label_[0]->setString(String::createWithFormat("%d", score)->getCString());
    score = UserDefault->getIntegerForKey("second");
    rank_label_[1]->setString(String::createWithFormat("%d", score)->getCString());
    score = UserDefault->getIntegerForKey("third");
    rank_label_[2]->setString(String::createWithFormat("%d", score)->getCString());
    score = UserDefault->getIntegerForKey("forth");
    rank_label_[3]->setString(String::createWithFormat("%d", score)->getCString());
    score = UserDefault->getIntegerForKey("fifth");
    rank_label_[4]->setString(String::createWithFormat("%d", score)->getCString());
}