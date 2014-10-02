//
//  TouchSprite
//
//  CCSprite做觸碰判斷 , 點到非透明處才有作用
//

#ifndef __Cinderella_X__TouchSprite__
#define __Cinderella_X__TouchSprite__

#include "cocos2d.h"

using namespace cocos2d;

class TouchSprite : public CCSprite ,public CCTouchDelegate
{

public:
    // === create Function ===
    static TouchSprite* createBySpriteFrameName(const char *spName);
    static TouchSprite* createByNoImage(CCRect tempScope);
    
    static TouchSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
    
    void myInit();
    virtual ~TouchSprite();
    
    //======
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);

    //----
    // 檢查點擊是否在sprite範圍內.
    bool containsTouchLocation(CCTouch *touch);

    // 設定觸碰判斷通過時時要執行的callback    
    void setAllEventCallBack(CCObject *rec, SEL_MenuHandler touchBeganCallBack, SEL_MenuHandler touchEndCallBack, SEL_MenuHandler touchMovedCallBack);

    void setTouchable(bool touchable);
    
    //是否要判斷透明度
    void setWithAlpha(bool isWithAlpha);
    
    //===

protected:
    CCObject*       m_pListener;
    SEL_MenuHandler    m_pfnSelector;
    SEL_MenuHandler m_touchEndCallBack;
    SEL_MenuHandler touchMovedCallBack;
    
    bool touchable;
    bool isWithAlpha;
    
};

#endif /* defined(__Cinderella_X__TouchSprite__) */
