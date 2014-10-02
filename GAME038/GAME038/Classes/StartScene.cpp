//
//  StartScene.cpp
//  HB00177Game1
//
//  Created by imac07 on 2014/9/1.
//
//

#include "StartScene.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;



CCScene* StartScene::scene()
{
    CCScene *scene = CCScene::create();
    
    StartScene *layer = StartScene::create();
    
    scene->addChild(layer);
    
    return scene;
}
bool StartScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCUserDefault *dataRecord = CCUserDefault::sharedUserDefault();
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache *cache=CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("startscene.plist");
    
    languageType = dataRecord->getIntegerForKey("Record_Language");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("BGM.mp3", true);

    CCSprite *backGround=CCSprite::createWithSpriteFrameName("hb_Game.png");
    backGround->cocos2d::CCNode::setPosition(size.width / 2, size.height / 2);
    this -> addChild(backGround);
    
    CCSprite *iconSpr=CCSprite::create("iTunesArtwork.png");
    iconSpr -> setPosition(ccp(279, 360));
    iconSpr -> setScale(0.55);
    backGround -> addChild(iconSpr);
    
    CCSprite * iconFrame=CCSprite::createWithSpriteFrameName("hb_Game01.png");
    iconFrame -> setPosition(ccp(279, 360));
    backGround -> addChild(iconFrame);
    
    tempString = CCString::createWithFormat("hb_GameWord%04d.png",languageType + 1);
    titleSpr=CCSprite::createWithSpriteFrameName(tempString -> getCString());
    titleSpr->cocos2d::CCNode::setPosition(765, 552);
    backGround->addChild(titleSpr);
    
    tempString = CCString::createWithFormat("hb_StartBtn%04d.png",languageType + 1);
    touchSpr=CCSprite::createWithSpriteFrameName(tempString -> getCString());
    touchSpr->cocos2d::CCNode::setPosition(515, 83);
    backGround->addChild(touchSpr);
    
    CCFadeOut * fadOut=CCFadeOut::create(1);
    CCFadeIn * fadIn=CCFadeIn::create(1);
    CCActionInterval *seq=CCSequence::create(fadOut,fadIn,NULL);
    touchSpr->runAction(CCRepeatForever::create(seq));

    languageBtn = this -> CreateBtn(1, "hb_GameLanguage", languageType, menu_selector(StartScene::languageSetting), 0);
    languageBtn -> setPosition(ccp(871, 85));
    backGround -> addChild(languageBtn,10);
    
    LanguageBtnT = this -> CreateBtn(2, "hb_Language", 0, menu_selector(StartScene::selectLanguage), 0);
    LanguageBtnT -> setPosition(ccp(languageBtn -> getPositionX(), languageBtn -> getPositionY()));
    LanguageBtnT -> setEnabled(false);
    backGround -> addChild(LanguageBtnT,8);
    
    LanguageBtnC = this -> CreateBtn(2, "hb_Language", 1, menu_selector(StartScene::selectLanguage), 1);
    LanguageBtnC -> setPosition(ccp(languageBtn -> getPositionX(), languageBtn -> getPositionY()));
    LanguageBtnC -> setEnabled(false);
    backGround -> addChild(LanguageBtnC,6);
    
    LanguageBtnE = this -> CreateBtn(2, "hb_Language", 2, menu_selector(StartScene::selectLanguage), 2);
    LanguageBtnE -> setPosition(ccp(languageBtn -> getPositionX(), languageBtn -> getPositionY()));
    LanguageBtnE -> setEnabled(false);
    backGround -> addChild(LanguageBtnE,4);
    
    
    CCSprite * GameNameBg = CCSprite::createWithSpriteFrameName("hb_Game04.png");
    GameNameBg -> setPosition(ccp(152, 85));
    backGround -> addChild(GameNameBg);
    tempSize = GameNameBg -> boundingBox().size;
    
    //=======================================
    //遊戲名稱 要換的地方
    if (languageType == 0) {
        tempString = CCString::createWithFormat("TwWord.png");
    }else if (languageType == 1) {
        tempString = CCString::createWithFormat("CnWord.png");
    }else if (languageType == 2) {
        tempString = CCString::createWithFormat("EnWord.png");
    }
    
    GameName = CCSprite::create(tempString -> getCString());
    GameName -> setPosition(ccp(tempSize.width / 2, tempSize.height / 2));
    GameNameBg -> addChild(GameName);
    //=======================================

    return true;
}
CCMenu * StartScene::CreateBtn(int btnType, const char *spName, int languageInt, SEL_MenuHandler selector,int tag)
{
    tempString = CCString::createWithFormat("hb_GameBtn%02d.png",btnType);
    CCSprite * BtnSpr = CCSprite::createWithSpriteFrameName(tempString -> getCString());
    
    tempString = CCString::createWithFormat("%s%04d.png",spName ,languageInt + 1);
    tempSize = BtnSpr -> boundingBox().size;
    CCSprite * languageWord = CCSprite::createWithSpriteFrameName(tempString -> getCString());
    languageWord -> setPosition(ccp(tempSize.width / 2, tempSize.height / 2));
    languageWord -> setTag(5);
    BtnSpr -> addChild(languageWord);
    
    BtnSpr -> cocos2d::CCNode::setScale(0.9);
    
    CCMenuItemSprite * btnItem = CCMenuItemSprite::create(BtnSpr, NULL);
    CCMenu *btnMenu = CCMenu::createWithItem(btnItem);
    btnItem -> setTarget(this, selector);
    btnItem -> setTag(tag);

    return btnMenu;
}
void StartScene::languageSetting(CCMenuItem * sender)
{
    LanguageBtnT -> stopAllActions();
    LanguageBtnC -> stopAllActions();
    LanguageBtnE -> stopAllActions();
    if (sender -> getTag() == 0) {
        sender -> setTag(1);
        float x = languageBtn -> getPositionX();
        float y = languageBtn -> getPositionY();
        LanguageBtnT -> runAction(CCMoveTo::create(0.3, ccp(x, y + 115 * 1)));
        LanguageBtnC -> runAction(CCMoveTo::create(0.3, ccp(x, y + 115 * 2)));
        LanguageBtnE -> runAction(CCMoveTo::create(0.3, ccp(x, y + 115 * 3)));
        
        LanguageBtnT -> setEnabled(true);
        LanguageBtnC -> setEnabled(true);
        LanguageBtnE -> setEnabled(true);
    }else{
        sender -> setTag(0);
        float x = languageBtn -> getPositionX();
        float y = languageBtn -> getPositionY();
        LanguageBtnT -> runAction(CCMoveTo::create(0.3, ccp(x, y)));
        LanguageBtnC -> runAction(CCMoveTo::create(0.3, ccp(x, y)));
        LanguageBtnE -> runAction(CCMoveTo::create(0.3, ccp(x, y)));
        
        LanguageBtnT -> setEnabled(false);
        LanguageBtnC -> setEnabled(false);
        LanguageBtnE -> setEnabled(false);
    }
    
}
void StartScene::selectLanguage(CCMenuItem * sender)
{
    CCUserDefault *dataRecord = CCUserDefault::sharedUserDefault();
    CCSpriteFrameCache *cache=CCSpriteFrameCache::sharedSpriteFrameCache();
    languageType = sender -> getTag();
    dataRecord->setIntegerForKey("Record_Language", languageType);   //語言類別.
    dataRecord->flush();
    
    
    tempString = CCString::createWithFormat("hb_GameWord%04d.png",languageType + 1);
    titleSpr -> setDisplayFrame(cache -> spriteFrameByName(tempString -> getCString()));
    
    tempString = CCString::createWithFormat("hb_StartBtn%04d.png",languageType + 1);
    touchSpr -> setDisplayFrame(cache -> spriteFrameByName(tempString -> getCString()));
    
    CCMenuItemSprite * tempItem = (CCMenuItemSprite*)((CCArray*)languageBtn -> getChildren()) -> objectAtIndex(0);
    CCSprite * tempSpr = (CCSprite*)tempItem -> getNormalImage();
    CCSprite * WordSpr = (CCSprite*)tempSpr -> getChildByTag(5);
    tempString = CCString::createWithFormat("hb_GameLanguage%04d.png",languageType + 1);
    WordSpr -> setDisplayFrame(cache -> spriteFrameByName(tempString -> getCString()));
    
    
    if (languageType == 0) {
        tempString = CCString::createWithFormat("TwWord.png");
    }else if (languageType == 1) {
        tempString = CCString::createWithFormat("CnWord.png");
    }else if (languageType == 2) {
        tempString = CCString::createWithFormat("EnWord.png");
    }
    CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage(tempString -> getCString());
    GameName->setTexture(tex);

    
    this -> languageSetting(tempItem);
    
}
void StartScene::onEnter(){
    //因為CCMenu的priority是 -128
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    
    CCNode::onEnter();
}

void StartScene::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

bool StartScene::ccTouchBegan(CCTouch* touch, CCEvent* event){
    
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->playEffect("PLAYBTN.wav");
    
    //TODO: 修改成遊戲Scene
    CCScene *toScene = Scene4::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.3f, toScene, ccBLACK));
    
    return true;
}


void StartScene::StartScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}