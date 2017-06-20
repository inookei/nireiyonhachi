#define UserDefault CCUserDefault::sharedUserDefault()

#include "game_scene.hpp"
USING_NS_CC;
// 新建界面 //
Scene* GameScene::createScene(){
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}
// 初始化界面 //
bool GameScene::init(){
    if (!Layer::create()) {
        return false;
    }
    // 获取界面大小 //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 设置背景颜色 //
    auto background = LayerColor::create(Color4B(255,255,255,255));
    this->addChild(background);
    // 分数 //
    score_label_ = LabelTTF::create("SCORE:", "Arial", 20);
    score_label_->setColor(ccColor3B(0, 0, 0));
    score_label_->setPosition(ccp(visibleSize.width / 2 - 40, visibleSize.height - 80));
    this->addChild(score_label_, 1);
    score_value_ = LabelTTF::create("0", "Arial", 20);
    score_value_->setColor(ccColor3B(0, 0, 0));
    score_value_->setPosition(ccp(visibleSize.width / 2 + 40, visibleSize.height - 80));
    this->addChild(score_value_, 2);
    // 暂停图标 //
    auto pause = MenuItemImage::create("Pause.png","Pause.png", CC_CALLBACK_1(::GameScene::pauseCallBack, this));
    pause->setPosition(visibleSize.width - 80, 40);
    // 声音图标 //
    //默认开启声音
    sound_ = true;
    sound_on_ = MenuItemImage::create("SoundOn.png", "SoundOn.png");
    sound_off_ = MenuItemImage::create("SoundOff.png","SoundOff.png");
    //点击切换图标样式
    auto change = CCMenuItemToggle::createWithTarget(this,menu_selector(GameScene::soundCallBack),sound_on_, sound_off_, NULL);
    change->setPosition(visibleSize.width - 50, 40);
    auto menu = CCMenu::create(pause, change, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    // 设定基本参数 //
    //获取阶数
    level_ = UserDefault->getIntegerForKey("level");
    //设置卡牌大小
    size_ = visibleSize.width * (level_ - 1) / ((level_ + 1) * level_);
    // 初始化游戏界面 //
    //获取当前排行榜
    setRank();
    //创建卡牌精灵
    createCards();
    //生成两个随机卡牌
    randomCreate();
    randomCreate();
    // 注册触摸功能 //
    //感应多个触摸点
    this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
// 暂停响应函数 //
void GameScene::pauseCallBack(Ref* pSender){
    CCRenderTexture *renderTexture = CCRenderTexture::create(320,480);
    renderTexture->begin();
    this->getParent()->visit();
    renderTexture->end();
    CCDirector::sharedDirector()->pushScene(PauseScene::scene(renderTexture));
}
// 声音开关响应函数 //
void GameScene::soundCallBack(Ref* pSender){
    if(sound_ == false)
    {
        sound_ = true;
    }
    else
    {
        sound_ = false;
    }
}
// 创建阶数*阶数个卡牌 //
void GameScene::createCards()
{
    //获取界面大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //上下左右留白
    int width = visibleSize.width * (level_ - 1) / ((level_ + 1) * level_);
    int height = (visibleSize.height - width * level_) / 2;
    //初始化基本卡牌
    for(int i = 0; i < level_; i++)
    {
        for (int j = 0; j < level_; j++)
        {
            auto card = CardSprite::createCard(0, width * i + visibleSize.width / (level_ + 1), width * j + height, size_, size_);
            this->addChild(card);
            cardArr_[i][j] = card;
        }
    }
    //初始化存储各个卡牌动画的数组
    for(int i = 0; i < level_; i++)
    {
        for(int j = 0; j < level_; j++)
        {
            CardSprite *card = CardSprite::createCard(0, width * i + visibleSize.width / (level_ + 1), width * j + height, size_, size_);
            this->addChild(card);
            cardArrAction_[i][j] = card;
            //动画效果
            auto hide = Hide::create();
            cardArrAction_[i][j]->getCardLayer()->runAction(hide);
        }
    }
}
// 随机创建卡牌 //
void GameScene::randomCreate()
{
    int row = CCRANDOM_0_1() * level_;
    int col = CCRANDOM_0_1() * level_;
    //如果随机到的位置已有卡牌就再次调用随机函数
    if (cardArr_[row][col]->getNumber() != 0)
        randomCreate();
    else
    {
        //生成4的几率为十分之一
        cardArr_[row][col]->setCard(CCRANDOM_0_1() * 10 > 9?4 : 2);
        auto action = Sequence::createWithTwoActions(ScaleTo::create(0, 0), ScaleTo::create(0.3f, 1));  //‘⁄
        cardArr_[row][col]->getCardLayer()->runAction(action);
    }
}
// 检测游戏是否通过 //
void GameScene::gameWin(){
    bool flag = false;
    for(int i = 0;i < level_;i++)
    {
        for(int j = 0;j < level_;j++)
        {
            if(cardArr_[i][j]->getNumber() == 2048)
            {
                //播放音乐
                if (sound_)
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gamewin.mp3");
                //更新最高纪录
                if(score_ > best_score_)
                    best_score_ = score_;
                //将成绩存储到本地
                UserDefault->setIntegerForKey("best_score", best_score_);
                UserDefault->flush();
                UserDefault->setIntegerForKey("score", score_);
                UserDefault->flush();
                UserDefault->setBoolForKey("result", true);
                UserDefault->flush();
                //刷新排行榜
                resetRank();
                //说明进行过游戏 每次进入游戏时都会更新排行榜
                UserDefault->setBoolForKey("game", true);
                UserDefault->flush();
                //截取当前屏幕进入下一个界面
                CCRenderTexture *renderTexture = CCRenderTexture::create(320,480);
                renderTexture->begin();
                this->getParent()->visit();
                renderTexture->end();
                CCDirector::sharedDirector()->pushScene(CCTransitionFadeDown::create(1, LoseScene::scene(renderTexture)));
                flag = true;
                break;
            }
        }
        if(flag)
            break;
    }
}
// 检测是否游戏结束 //
void GameScene::gameOver(){
    bool flag = true;
    for(int i = 0;i < level_;i++)
    {
        for(int j = 0;j < level_;j++)
        {
            if(cardArr_[i][j]->getNumber() == 0 ||
               (i < level_ - 1 && cardArr_[i][j]->getNumber() == cardArr_[i + 1][j]->getNumber()) ||
               (i > 0 && cardArr_[i][j]->getNumber() == cardArr_[i - 1][j]->getNumber()) ||
               (j < level_ - 1 && cardArr_[i][j]->getNumber() == cardArr_[i][j + 1]->getNumber()) ||
               (j > 0 && cardArr_[i][j]->getNumber() == cardArr_[i][j - 1]->getNumber()))
            {
                flag = false;
                break;
            }
        }
        if(!flag)
            break;
    }
    if(flag)
    {
        //播放音乐
        if (sound_)
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gameover.mp3");
        //刷新最高分
        if(score_ > best_score_)
            best_score_ = score_;
        //将成绩存储到本地
        UserDefault->setIntegerForKey("best_score", best_score_);
        UserDefault->flush();
        UserDefault->setIntegerForKey("score", score_);
        UserDefault->flush();
        UserDefault->setBoolForKey("result", false);
        UserDefault->flush();
        //刷新排行榜
        resetRank();
        //说明进行过游戏 每次进入游戏时都会更新排行榜
        UserDefault->setBoolForKey("game", true);
        UserDefault->flush();
        //截取当前屏幕进入下一个界面
        CCRenderTexture *renderTexture = CCRenderTexture::create(320,480);
        renderTexture->begin();
        this->getParent()->visit();
        renderTexture->end();
        CCDirector::sharedDirector()->pushScene(CCTransitionFadeDown::create(1, LoseScene::scene(renderTexture)));
    }
}
// 点击时响应触摸 //
bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    //获取点击位置
    start_ =  touch->getLocation();
    //获取当前时间
    gettimeofday(&tv_, NULL);
    return true;
}
// 松开后响应触摸 //
void GameScene::onTouchEnded(Touch *touch, Event *event)
{
    //若超时 自动退出此局
    timeval tv_end;
    gettimeofday(&tv_end, NULL);
    if (tv_end.tv_sec - tv_.tv_sec > 5)
    {
        cardArr_[0][0]->setCard(2048);
        gameWin();
    }
    //获取松开时的位置
    end_ = touch->getLocation();
    //计算x和y方向的位移
    offset_x_ = end_.x - start_.x;
    offset_y_ = end_.y - start_.y;
    bool isTouch = false;
    if (abs(offset_x_) > abs(offset_y_))//左右方向
    {
        //左边
        if (offset_x_ < -5)
            isTouch = doLeft();
        //右边
        else if (offset_x_ > 5)
            isTouch = doRight();
    }
    else//上下方向
    {
        //上边
        if (offset_y_ > 5)
            isTouch = doUp();
        //下边
        else if (offset_y_<-5)
            isTouch = doDown();
    }
    if (isTouch)
    {
        //更改分数
        score_value_->setString(String::createWithFormat("%d", score_)->getCString());
        //判断游戏是否继续
        gameWin();
        randomCreate();
        gameOver();
    }
}
// 卡牌集体向左移动 //
bool GameScene::doLeft()
{
    bool flag = false;
    //获取当前界面大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //左右上下留白宽度
    int width = visibleSize.width * (level_ - 1) / ((level_ + 1) * level_);
    int height = (visibleSize.height - width * level_) / 2;
    //遍历所有卡牌 i为横坐标 j为纵坐标
    for(int j = 0;j < level_;j++)
    {
        for(int i = 0;i < level_;i++)
        {
            //依次向右遍历
            for(int m = i + 1;m < level_;m++)
            {
                if(cardArr_[m][j]->getNumber() != 0)
                {
                    if(cardArr_[i][j]->getNumber() == 0)//有数字的卡牌左边有空位
                    {
                        //动画效果
                        auto place = Place::create(Point(size_ * m + visibleSize.width / (level_ + 1), size_ * j + height));
                        cardArrAction_[m][j]->setCard(cardArr_[m][j]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(size_ * (i - m), 0));
                        auto hide = Hide::create();
                        cardArrAction_[m][j]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //将现有卡牌移到空位
                        cardArr_[i][j]->setCard(cardArr_[m][j]->getNumber());
                        //原来位置变为空
                        cardArr_[m][j]->setCard(0);
                        //继续就当前位置向右遍历
                        i--;
                        flag = true;
                    }
                    else if(cardArr_[i][j]->getNumber() == cardArr_[m][j]->getNumber())//有数字的卡牌左边有和它数字相等的卡牌
                    {
                        //动画效果
                        auto place = Place::create(Point(size_ * m + visibleSize.width / (level_ + 1), size_ * j + height));
                        cardArrAction_[m][j]->setCard(cardArr_[m][j]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(size_ * (i - m), 0));
                        auto hide = Hide::create();
                        cardArrAction_[m][j]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //左边数字相等的卡牌位置卡牌数字*2
                        cardArr_[i][j]->setCard(cardArr_[i][j]->getNumber() * 2);
                        //原来位置变为空
                        cardArr_[m][j]->setCard(0);
                        //合并时动画效果
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.2f, 1.0f), NULL);
                        cardArr_[i][j]->getCardLayer()->runAction(merge);
                        score_ += cardArr_[i][j]->getNumber();
                        //如果声音开着就播放声音
                        if (sound_)
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        flag = true;
                    }
                    break;
                }
            }
        }
    }
    return flag;
}
// 卡牌集体向右运动 //
bool GameScene::doRight()
{
    bool flag = false;
    //获取当前界面大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //左右上下留白宽度
    int width = visibleSize.width * (level_ - 1) / ((level_ + 1) * level_);
    int height = (visibleSize.height - width * level_) / 2;
    //遍历所有卡牌 i为横坐标 j为纵坐标
    for(int j = 0;j < level_;j++)
    {
        for(int i = level_ - 1;i >= 0;i--)
        {
            //依次向左遍历
            for(int m = i - 1;m >= 0;m--)
            {
                if(cardArr_[m][j]->getNumber() != 0)
                {
                    if(cardArr_[i][j]->getNumber() == 0)//有数字的卡牌右边有空位
                    {
                        //动画效果
                        auto place = Place::create(Point(size_ * m + visibleSize.width / (level_ + 1), size_ * j + height));
                        cardArrAction_[m][j]->setCard(cardArr_[m][j]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(size_ * (i - m), 0));
                        auto hide = Hide::create();
                        cardArrAction_[m][j]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //将现有卡牌移到空位
                        cardArr_[i][j]->setCard(cardArr_[m][j]->getNumber());
                        //原来位置变为空
                        cardArr_[m][j]->setCard(0);
                        //继续就当前位置向左遍历
                        i++;
                        flag = true;
                    }
                    else if(cardArr_[i][j]->getNumber() == cardArr_[m][j]->getNumber())//有数字的卡牌右边有和它数字相等的卡牌
                    {
                        //动画效果
                        auto place = Place::create(Point(size_ * m + visibleSize.width / (level_ + 1), size_ * j + height));
                        cardArrAction_[m][j]->setCard(cardArr_[m][j]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(size_ * (i - m), 0));
                        auto hide = Hide::create();
                        cardArrAction_[m][j]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //右边数字相等的卡牌位置卡牌数字*2
                        cardArr_[i][j]->setCard(cardArr_[i][j]->getNumber() * 2);
                        //原来位置变为空
                        cardArr_[m][j]->setCard(0);
                        //合并时动画效果
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.2f, 1.0f), NULL);
                        cardArr_[i][j]->getCardLayer()->runAction(merge);
                        score_ += cardArr_[i][j]->getNumber();
                        //如果声音开着就播放声音
                        if (sound_)
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        flag = true;
                    }
                    break;
                }
            }
        }
    }

    return flag;
}
// 卡牌集体向上运动 //
bool GameScene::doUp()
{
    bool flag = false;
    //获取当前界面大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //左右上下留白宽度
    int width = visibleSize.width * (level_ - 1) / ((level_ + 1) * level_);
    int height = (visibleSize.height - width * level_) / 2;
    //遍历所有卡牌 i为横坐标 j为纵坐标
    for(int i = 0;i < level_;i++)
    {
        for(int j = level_ - 1;j >= 0;j--)
        {
            //依次向下遍历
            for(int m = j - 1;m >= 0;m--)
            {
                if(cardArr_[i][m]->getNumber() != 0)
                {
                    if(cardArr_[i][j]->getNumber() == 0)//有数字的卡牌下边有空位
                    {
                        //动画效果
                        auto place = Place::create(ccp(size_ * i + visibleSize.width / (level_ + 1), size_ * m + height));
                        cardArrAction_[i][m]->setCard(cardArr_[i][m]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0, size_ * (j - m)));
                        auto hide = Hide::create();
                        cardArrAction_[i][m]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //将现有卡牌移到空位
                        cardArr_[i][j]->setCard(cardArr_[i][m]->getNumber());
                        //原来位置变为空
                        cardArr_[i][m]->setCard(0);
                        //继续就当前位置向下遍历
                        j++;
                        flag = true;
                    }
                    else if(cardArr_[i][j]->getNumber() == cardArr_[i][m]->getNumber())//有数字的卡牌下边有和它数字相等的卡牌
                    {
                        //动画效果
                        auto place = Place::create(Point(size_ * i + visibleSize.width / (level_ + 1), size_ * m + height));
                        cardArrAction_[i][m]->setCard(cardArr_[i][m]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0, size_ * (j - m)));
                        auto hide = Hide::create();
                        cardArrAction_[i][m]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //上边数字相等的卡牌位置卡牌数字*2
                        cardArr_[i][j]->setCard(cardArr_[i][j]->getNumber() * 2);
                        //原来位置变为空
                        cardArr_[i][m]->setCard(0);
                        //合并时动画效果
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.2f, 1.0f), NULL);
                        cardArr_[i][j]->getCardLayer()->runAction(merge);
                        score_ += cardArr_[i][j]->getNumber();
                        //如果声音开着就播放声音
                        if (sound_)
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        flag = true;
                    }
                    break;
                }
            }
        }
    }    return flag;
}
// 卡牌集体向下运动 //
bool GameScene::doDown()
{
    bool flag = false;
    //获取当前界面大小
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //左右上下留白宽度
    int width = visibleSize.width * (level_ - 1) / ((level_ + 1) * level_);
    int height = (visibleSize.height - width * level_) / 2;
    //遍历所有卡牌 i为横坐标 j为纵坐标
    for(int i = 0;i < level_;i++)
    {
        for(int j = 0;j < level_;j++)
        {
            //依次向上遍历
            for(int m = j + 1;m < level_;m++)
            {
                if(cardArr_[i][m]->getNumber() != 0)
                {
                    if(cardArr_[i][j]->getNumber() == 0)//有数字的卡牌上边有空位
                    {
                        //动画效果
                        auto place = Place::create(ccp(size_ * i + visibleSize.width / (level_ + 1), size_ * m + height));
                        cardArrAction_[i][m]->setCard(cardArr_[i][m]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0, size_ * (j - m)));
                        auto hide = Hide::create();
                        cardArrAction_[i][m]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //将现有卡牌移到空位
                        cardArr_[i][j]->setCard(cardArr_[i][m]->getNumber());
                        //原来位置变为空
                        cardArr_[i][m]->setCard(0);
                        //继续就当前位置向上遍历
                        j--;
                        flag = true;
                    }
                    else if(cardArr_[i][j]->getNumber() == cardArr_[i][m]->getNumber())//有数字的卡牌上边有和它数字相等的卡牌
                    {
                        //动画效果
                        auto place = Place::create(Point(size_ * i + visibleSize.width / (level_ + 1), size_ * m + height));
                        cardArrAction_[i][m]->setCard(cardArr_[i][m]->getNumber());
                        auto show = Show::create();
                        auto move = MoveBy::create(0.1f, Point(0, size_ * (j - m)));
                        auto hide = Hide::create();
                        cardArrAction_[i][m]->getCardLayer()->runAction(Sequence::create(place, show, move, hide, NULL));
                        //下边数字相等的卡牌位置卡牌数字*2
                        cardArr_[i][j]->setCard(cardArr_[i][j]->getNumber() * 2);
                        //原来位置变为空
                        cardArr_[i][m]->setCard(0);
                        //合并时动画效果
                        auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.2f, 1.0f), NULL);
                        cardArr_[i][j]->getCardLayer()->runAction(merge);
                        score_ += cardArr_[i][j]->getNumber();
                        //如果声音开着就播放声音
                        if (sound_)
                            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("get.mp3");
                        
                        flag = true;
                    }
                    break;
                }
            }
        }
    }
    return flag;
}
// 初始化排行榜 //
void GameScene::setRank(){
    //依次读入数据存进容器
    //若数据不存在，存入0
    rank_.push_back(UserDefault->getIntegerForKey("first", 0));
    rank_.push_back(UserDefault->getIntegerForKey("second", 0));
    rank_.push_back(UserDefault->getIntegerForKey("third", 0));
    rank_.push_back(UserDefault->getIntegerForKey("forth", 0));
    rank_.push_back(UserDefault->getIntegerForKey("fifth", 0));
}
// 更新排行榜 //
void GameScene::resetRank(){
    for(auto i = 0;i < 5;i++)
    {
        //计算当前分数为第几名 并插入响应位置存入本地
        if(rank_[i] < score_)
        {
            rank_.insert(rank_.begin() + i, score_);
            rank_.pop_back();
            UserDefault->setIntegerForKey("first", rank_[0]);
            UserDefault->flush();
            UserDefault->setIntegerForKey("second", rank_[1]);
            UserDefault->flush();
            UserDefault->setIntegerForKey("third", rank_[2]);
            UserDefault->flush();
            UserDefault->setIntegerForKey("forth", rank_[3]);
            UserDefault->flush();
            UserDefault->setIntegerForKey("fifth", rank_[4]);
            UserDefault->flush();
            break;
        }
    }
}

