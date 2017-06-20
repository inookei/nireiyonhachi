#ifndef card_settings_hpp
#define card_settings_hpp

#include "cocos2d.h"
USING_NS_CC;

class CardSprite : public Sprite
{
public:
    void cardInit(int num, float point_x, float point_y, int width, int height);
    static CardSprite* createCard(int num, float point_x, float point_y, int width, int height);
    virtual bool init();
    CREATE_FUNC(CardSprite);
    void setCard(int num);
    int getNumber();
    LayerColor* getCardLayer();
private:
    int number;
    LabelTTF *card_label;
    LayerColor *card_color;
};

#endif /* card_settings_hpp */
