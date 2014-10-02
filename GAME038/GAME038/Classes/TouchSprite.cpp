//
//  TouchSprite
//

#include "TouchSprite.h"

#define AnimationSpeed 1.0f/12.0f

using namespace cocos2d;

TouchSprite* TouchSprite::createBySpriteFrameName(const char *spName){
    TouchSprite *pobSprite = new TouchSprite();
    
    if (pobSprite && pobSprite->initWithSpriteFrameName(spName))
    {
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;
}

TouchSprite* TouchSprite::createByNoImage(CCRect tempScope){
    TouchSprite *pobSprite = new TouchSprite();
    
    pobSprite->init();
    pobSprite->setTextureRect(tempScope);
    pobSprite->setOpacity(0);
    
    if (pobSprite)
    {
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;
}

TouchSprite* TouchSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    TouchSprite *pobSprite = new TouchSprite();
    
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void TouchSprite::myInit(){
//    CCLOG("TouchSprite init");
    
    m_pListener = NULL;
    m_pfnSelector = NULL;
    m_touchEndCallBack = NULL;
    touchMovedCallBack= NULL;
    
    touchable = true;
    isWithAlpha = false;
}

TouchSprite::~TouchSprite(){
//    CCLOG("~TouchSprite ");
    
    m_pListener = NULL;
    m_pfnSelector = NULL;
    m_touchEndCallBack = NULL;
    touchMovedCallBack= NULL;
}

void TouchSprite::onEnter(){
    //因為CCMenu的priority是 -128
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -120, true);
    
    CCNode::onEnter();
}

void TouchSprite::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

bool TouchSprite::ccTouchBegan(CCTouch* touch, CCEvent* event){
    
    if (!touchable)
        return false;
    
    if (this->containsTouchLocation(touch))
    {
        
        //Active
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        
        return true;
    }
    
    return false;
}

void TouchSprite::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
//    CCLog("ccTouchMoved");
    
    if (m_pListener && touchMovedCallBack)
    {
        (m_pListener->*touchMovedCallBack)(this);
    }
    
}

void TouchSprite::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (m_pListener && m_touchEndCallBack)
    {
        (m_pListener->*m_touchEndCallBack)(this);
    }
}
#pragma mark -


bool TouchSprite::containsTouchLocation(CCTouch *touch){
    
//    CCLog("this position = %f, %f", this->getPositionX(), this->getPositionY());
    
    CCRect thisRect = CCRectMake(getPositionX() - getContentSize().width * getAnchorPoint().x, getPositionY() - getContentSize().height * getAnchorPoint().y, getContentSize().width  * getScaleX(), getContentSize().height  * getScaleY());
    
    CCPoint position = touch->getLocation();
    
    bool isTouched = thisRect.containsPoint(position);
    
    CCLog("isTouched = %d", isTouched);
    CCLog("isWithAlpha = %d", isWithAlpha);

    if (isWithAlpha)
    {
        if (isTouched) {
            //取得圖片該點的RGBA 做判斷.
            CCPoint nodeLocation = this->convertToNodeSpace(position);
            
            //-----
            u_int8_t data[4];
            CCRenderTexture *renderTexture = CCRenderTexture::create(getContentSize().width, getContentSize().height);
            
            renderTexture->begin();

            this->draw();
            
            glReadPixels(nodeLocation.x, nodeLocation.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
            renderTexture->end();
            
            renderTexture->release();
            
            int alpha = data[3];
            
            if (alpha < 85) {
                
                CCLog("alpha < 85");
                // alpha 小於 85 當做透明.
                //不處理點擊觸碰.
                
                return false;
            }
            
            CCLog("alpha > 85");
        }
    }
    
    return isTouched;
}

void TouchSprite::setAllEventCallBack(CCObject *rec, SEL_MenuHandler selector, SEL_MenuHandler touchEndCallBack, SEL_MenuHandler touchMovedCallBack){
    m_pListener = rec;
    m_pfnSelector = selector;
    m_touchEndCallBack = touchEndCallBack;
    this->touchMovedCallBack = touchMovedCallBack;
}

void TouchSprite::setTouchable(bool touchable)
{
    this->touchable = touchable;
}

void TouchSprite::setWithAlpha(bool isWithAlpha)
{
    this->isWithAlpha = isWithAlpha;
}