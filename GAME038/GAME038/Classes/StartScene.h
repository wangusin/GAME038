//
//  StartScene.h
//  HB00177Game1
//
//  Created by imac07 on 2014/9/1.
//
//

#ifndef __StartScene__
#define __StartScene__

#include "cocos2d.h"

#include "Scene4.h"

using namespace cocos2d;
class StartScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    void menuCloseCallback(CCObject* pSender);
    CREATE_FUNC(StartScene);
    
    int languageType;
    CCSize tempSize;
    CCString * tempString;
    
    CCMenu * LanguageBtnE;
    CCMenu * LanguageBtnC;
    CCMenu * LanguageBtnT;
    CCMenu * languageBtn;
    
    CCSprite *titleSpr;
    CCSprite *touchSpr;
    CCSprite * GameName;
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    
    CCMenu * CreateBtn(int btnType, const char *spName, int languageInt, SEL_MenuHandler selector,int tag);
    
    void languageSetting(CCMenuItem * sender);
    void selectLanguage(CCMenuItem * sender);
    
};
#endif /* defined(__HB00177Game1__StartScene__) */
