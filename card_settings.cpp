#include "card_settings.hpp"
USING_NS_CC;
// 新建卡牌精灵 //
bool CardSprite::init(){
    if (!Sprite::init()) {
        return false;
    }
    else{
        return true;
    }
}
// 创建卡牌 分配内存 //
CardSprite* CardSprite::createCard(int num, float point_x, float point_y, int width, int height){
    //分配内存
    CardSprite *card = new CardSprite();
    if (!card->init())
    {
        CC_SAFE_RELEASE(card);
        return NULL;
    }
    //避免内存泄露
    card->autorelease();
    //初始化卡牌
    card->cardInit(num, point_x, point_y, width, height);
    return card;
}
// 初始化卡牌 //
void CardSprite::cardInit(int num, float point_x, float point_y, int width, int height){
    //设定数字
    number = num;
    //设定默认卡牌底色
    card_color = LayerColor::create(Color4B(245,245,245,245), width - 5, height - 5);
    card_color->setPosition(ccp(point_x, point_y));
    if(number > 0)//数字不为0
    {
        //将数字转换为字符
        char buf[8];
        sprintf(buf, "%d", number);
        card_label = LabelTTF::create(buf, "Arial", 30);
        //在卡牌中央显示数字
        card_label->setPosition(card_color->getContentSize().width / 2, card_color->getContentSize().height / 2);
        //将Label添加到底色上
        card_color->addChild(card_label, 1);
    }
    else//数字为0
    {
        card_label = LabelTTF::create("", "Arial", 30);
        //在卡牌中央显示数字
        card_label->setPosition(card_color->getContentSize().width / 2, card_color->getContentSize().height / 2);
        //将label添加到底色上
        card_color->addChild(card_label, 1);
    }
    this->addChild(card_color);
}
// 更改卡牌数字 //
void CardSprite::setCard(int num){
    number = num;
    //更改显示的数字
    if (number > 0)
        card_label->setString(String::createWithFormat("%d", number)->getCString());
    else
        card_label->setString("");
    //底色和字号随数字变化
    switch (number) {
        case 0:
        {
            card_color->setColor(Color3B(245,245,245));
        }
        case 2:
        {
            card_label->setFontSize(30);
            card_color->setColor(Color3B(235,235,235));
            break;
        }
        case 4:
        {
            card_label->setFontSize(30);
            card_color->setColor(Color3B(220,220,220));
            break;
        }
        case 8:
        {
            card_label->setFontSize(30);
            card_color->setColor(Color3B(200,200,200));
            break;
        }
        case 16:
        {
            card_label->setFontSize(25);
            card_color->setColor(Color3B(180,180,180));
            break;
        }
        case 32:
        {
            card_label->setFontSize(25);
            card_color->setColor(Color3B(150,150,150));
            break;
        }
        case 64:
        {
            card_label->setFontSize(25);
            card_color->setColor(Color3B(120,120,120));
            break;
        }
        case 128:
        {
            card_label->setFontSize(20);
            card_color->setColor(Color3B(100,100,100));
            break;
        }
        case 256:
        {
            card_label->setFontSize(20);
            card_color->setColor(Color3B(80,80,80));
            break;
        }
        case 512:
        {
            card_label->setFontSize(20);
            card_color->setColor(Color3B(50,50,50));
            break;
        }
        case 1024:
        {
            card_label->setFontSize(15);
            card_color->setColor(Color3B(25,25,25));
            break;
        }
        case 2048:
        {
            card_label->setFontSize(15);
            card_color->setColor(Color3B(0,0,0));
            break;
        }
        default:
            break;
    }
}
// 获得卡牌的数字 //
int CardSprite::getNumber()
{
    return number;
}
// 获得卡牌的底色 //
LayerColor* CardSprite::getCardLayer(){
    return card_color;
}