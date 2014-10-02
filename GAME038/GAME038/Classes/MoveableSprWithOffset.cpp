//
//  MoveableSprWithOffset
//

#include "MoveableSprWithOffset.h"

#define AnimationSpeed 1.0f/12.0f

using namespace cocos2d;

MoveableSprWithOffset* MoveableSprWithOffset::createBySpriteFrameName(const char *spName){
//    MoveableSprWithOffset *pobSprite = new MoveableSprWithOffset();
//    
//    if (pobSprite && pobSprite->initWithSpriteFrameName(spName))
//    {
//        pobSprite->myInit();
//        pobSprite->autorelease();
//        return pobSprite;
//    }
//    CC_SAFE_DELETE(pobSprite);
//    
//    return NULL;
    
    int touchPriority = -120;
    
    return createBySpriteFramNameWithTouchPriority(spName, touchPriority);
}

MoveableSprWithOffset* MoveableSprWithOffset::createBySpriteFramNameWithTouchPriority(const char *spName, int touchPriority){
    MoveableSprWithOffset *pobSprite = new MoveableSprWithOffset();
    
    if (pobSprite && pobSprite->initWithSpriteFrameName(spName))
    {
        pobSprite->touchPriority = touchPriority;
        pobSprite->myInit();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    
    return NULL;
}

void MoveableSprWithOffset::myInit(){
//    CCLOG("MoveableSprWithOffset init");
    m_pListener = NULL;
    m_touchBeganCallBack = NULL;
    m_touchEndCallBack = NULL;
    
    moveable = true;
    alphaOnOff = true;
    
}
MoveableSprWithOffset::~MoveableSprWithOffset(){
//    CCLOG("~MoveableSprWithOffset");
    m_pListener = NULL;
    m_touchBeganCallBack = NULL;
    m_touchEndCallBack = NULL;
    
}

void MoveableSprWithOffset::onEnter(){
    //因為CCMenu的priority是 -128
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, touchPriority, true);
    
    CCNode::onEnter();
}

void MoveableSprWithOffset::onExit(){
    CCLog("MoveableSprWithOffset tag = %d", this->getTag());
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

#pragma mark -
#pragma mark TouchDelegate
bool MoveableSprWithOffset::ccTouchBegan(CCTouch* touch, CCEvent* event){
    
    if (!moveable)
        return false;
    
    CCPoint starlocation=touch->getStartLocationInView();
    starlocation = CCDirector::sharedDirector()->convertToGL(starlocation);
    
    if (this->containsTouchLocation(touch))
    {
        originalPoint = this->getPosition();
        touchOffset = ccpSub(originalPoint, starlocation);
        
        if (m_pListener && m_touchBeganCallBack)
        {
            (m_pListener->*m_touchBeganCallBack)(this);
        }
        
        return true;
    }
    
    return false;
}
void MoveableSprWithOffset::ccTouchMoved(CCTouch* touch, CCEvent* event){
    
    CCPoint touchLocation = touch->getLocationInView();
    
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    CCPoint moveOffset = ccpAdd(touchOffset, touchLocation);
    this->setPosition(moveOffset);
    
}

void MoveableSprWithOffset::ccTouchEnded(CCTouch* touch, CCEvent* event){
    if (m_pListener && m_touchEndCallBack)
    {
        (m_pListener->*m_touchEndCallBack)(this);
    }
}

#pragma mark -

bool MoveableSprWithOffset::containsTouchLocation(CCTouch *touch){
    
    CCPoint touchLocation = touch->getLocationInView();
    
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    CCRect thisRect = CCRectMake(getPositionX() - getContentSize().width*getAnchorPoint().x,
                                 getPositionY() - getContentSize().height*getAnchorPoint().y,
                                 getContentSize().width, getContentSize().height);
    
    bool isTouched = thisRect.containsPoint(touchLocation);
    
    if (isTouched) {
        
        if (!alphaOnOff)
            return true;
        
        //取得圖片該點的RGBA 做判斷.
        CCPoint nodeLocation = this->convertToNodeSpace(touchLocation);
        
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
            // alpha 小於 85 當做透明.
            //不處理點擊觸碰.
            
            return false;
        }
    }
    
    return isTouched;
}

void MoveableSprWithOffset::setTarget(CCObject *rec, SEL_MenuHandler selector, SEL_MenuHandler touchEndCallBack){
    m_pListener = rec;
    m_touchBeganCallBack = selector;
    m_touchEndCallBack = touchEndCallBack;
}

void MoveableSprWithOffset::setMoveable(bool moveable)
{
    this->moveable = moveable;
}

void MoveableSprWithOffset::setWithAlpha(bool alphaOnOff)
{
    this->alphaOnOff = alphaOnOff;
}