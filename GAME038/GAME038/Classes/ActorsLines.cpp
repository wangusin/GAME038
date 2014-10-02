#include "ActorsLines.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

static const int OSTotalTime[6][2] = {{15, 16}, {17, 18}, {18, 22}, {27, 34}, {28, 33}, {28, 38}};

ActorsLines::ActorsLines(): wordsMoveDownAction(NULL), imageCache(NULL), wordsLayer(NULL), word(NULL), phoneticAlphabetButton(NULL), phoneticAlphabetSpriteItem(NULL), backgroundMenu(NULL)
{
    for (int i = 0; i < ChapterCount; i++)
    {
        //wordsLayer[i] = NULL;
        wordsMoveUpAction[i] = NULL;
    }
    
    target = NULL;
    OpeningClickedCallBack = NULL;
    backgroundClickedCallBack = NULL;
}
ActorsLines::~ActorsLines()
{
    target = NULL;
    OpeningClickedCallBack = NULL;
    backgroundClickedCallBack = NULL;
}

// on "init" you need to initialize your instance
bool ActorsLines::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    chapterIndex = 0;
    nowPlayingOSId = 0;
    languageNameG = "";
    isClosing = true;
    isPlayingOS = false;
    isWordsClosing = false;
    isBackgroundClicked = false;
    isActionRunning = false;
    isShowPhonetic = false;
    languageTypeWord = TRADITIONAL_CHINESE;
    
    //因應壓縮
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    
    imageCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    imageCache->addSpriteFramesWithFile("ActorsLines.plist");
    
    layerAtTopPosition[0] = ccp(0, 200);
    layerAtTopPosition[1] = ccp(0, 235);
    layerAtTopPosition[2] = ccp(0, 235);
    layerAtTopPosition[3] = ccp(0, 275);
    layerAtTopPosition[4] = ccp(0, 310);
    layerAtTopPosition[5] = ccp(0, 310);
    
    phoneticAlphabetPosition[0] = ccp(898, 586);
    phoneticAlphabetPosition[1] = ccp(898, 554);
    phoneticAlphabetPosition[2] = ccp(898, 554);
    phoneticAlphabetPosition[3] = ccp(898, 516);
    phoneticAlphabetPosition[4] = ccp(898, 478);
    phoneticAlphabetPosition[5] = ccp(898, 476);
    
    closePosition = ccp(140, 704);
    
    wordsPosition = ccp(512, 585);
    
    wordsBackgroundZone[0] = CCRectMake(0, 0, 820, 200);
    wordsBackgroundZone[1] = CCRectMake(0, 0, 820, 220);
    wordsBackgroundZone[2] = CCRectMake(0, 0, 820, 220);
    wordsBackgroundZone[3] = CCRectMake(0, 0, 820, 270);
    wordsBackgroundZone[4] = CCRectMake(0, 0, 820, 300);
    wordsBackgroundZone[5] = CCRectMake(0, 0, 820, 310);
    
    wordsBackgroundPosition[0] = ccp(102, 544);
    wordsBackgroundPosition[1] = ccp(102, 509);
    wordsBackgroundPosition[2] = ccp(102, 509);
    wordsBackgroundPosition[3] = ccp(102, 468);
    wordsBackgroundPosition[4] = ccp(102, 432);
    wordsBackgroundPosition[5] = ccp(102, 420);
    
    chapterIndex = 0;
    
    //背景按鈕，按完直接進遊戲
    CCSprite* backgroundButton = CCSprite::create();
    CCRect backgroundMenuSize = CCRectMake(0, 0, 1024, 768);
    backgroundButton->setTextureRect(backgroundMenuSize);
    backgroundButton->setOpacity(0);
    
    CCMenuItemSprite* backgroundSpriteItem = CCMenuItemSprite::create(backgroundButton, backgroundButton, this, menu_selector(ActorsLines::onBackgroundClicked));
    backgroundSpriteItem->setPosition(ccp(0, 0));
    
    backgroundMenu = CCMenu::create(backgroundSpriteItem, NULL);
    backgroundMenu->setTouchPriority(-111);
    this->addChild(backgroundMenu);
    
    backgroundMenu->setVisible(false);
    
    isBackgroundClicked = false;
    
    return true;
}

#pragma mark -
#pragma mark 內部函數

void ActorsLines::setSpriteObjectAtContent(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* content)
{
    target = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(name));
    
    target->setPosition(position);
    
    content->addChild(target, layerLevel);
}

void ActorsLines::setWordsAction()
{
    for (int i = 0; i < ChapterCount; i++)
    {
        CCActionInterval* moveUp = CCMoveTo::create(0.5f, layerAtTopPosition[i]);
        
        wordsMoveUpAction[i] = CCSequence::create(moveUp, CCCallFuncN::create(this, callfuncN_selector(ActorsLines::wordsMovedUp)), NULL);
        
        wordsMoveUpAction[i]->retain();
    }

    CCActionInterval* moveDown = CCMoveTo::create(0.5f, ccp(0, 0));
    
    wordsMoveDownAction = CCSequence::create(moveDown, CCCallFuncN::create(this, callfuncN_selector(ActorsLines::wordsMoveDown)), NULL);
    
    wordsMoveDownAction->retain();
}

void ActorsLines::playOS()
{
    isPlayingOS = true;
    
    const char* languageName = "";
    
    int languageIndex = 0;
    
    if (languageTypeWord == TRADITIONAL_CHINESE || languageTypeWord == SIMPLE_CHINESE)
    {
        languageName = "T";
        languageIndex = 0;
    }
    else if (languageTypeWord == ENGLISH)
    {
        languageName = "E";
        languageIndex = 1;
    }
    
    CCLog("function chapterIndex = %d", chapterIndex);
    
    const char* OSName = CCString::createWithFormat(OS, languageName, chapterIndex + 1)->getCString();
    
//    nowPlayingOSId = playSoundEffect(OSName, false);
    
    nowPlayingOSId = ShareMethod::getInstance()->playLongSound(OSName);
    
    this->scheduleOnce(schedule_selector(ActorsLines::whenOSPlayCompleted), OSTotalTime[chapterIndex][languageIndex]);
}

int ActorsLines::playSoundEffect(const char *soundName, bool isRepeat)
{
    int soundId =  SimpleAudioEngine::sharedEngine()->playEffect(soundName, isRepeat);
    
    return soundId;
}

void ActorsLines::stopOS()
{
    this->unschedule(schedule_selector(ActorsLines::whenOSPlayCompleted));
    
    isPlayingOS = false;
    
//    SimpleAudioEngine::sharedEngine()->stopEffect(nowPlayingOSId);
    
    ShareMethod::getInstance()->stopLongSound(nowPlayingOSId);
    
    nowPlayingOSId = 0;
}

void ActorsLines::setBackgroundMenuVisible(bool visible)
{
    backgroundMenu->setVisible(visible);
}

void ActorsLines::setWord(int chapterIndex)
{    
    languageNameG = getLanguageName(false);
    
    wordsLayer = CCLayer::create();
    
    wordsLayer->setContentSize(CCSizeMake(1024, 768));
    
    wordsLayer->setPosition(layerAtTopPosition[chapterIndex - 1]);
    
    word = CCSprite::create(CCString::createWithFormat(WORDS_PICTURE, chapterIndex, languageNameG)->getCString());
    
    (ShareData::getInstance())->setPictureName(CCString::createWithFormat(WORDS_PICTURE, chapterIndex, languageNameG)->getCString());
    
    (ShareData::getInstance())->setPictureNameTs("");
    
    word->setPosition(wordsPosition);
    
    wordsLayer->addChild(word, -1);
    
    word->setTag(AL_WORDS_NUMBER + chapterIndex - 1);
    
    //按鈕
    
    phoneticAlphabetButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(PHONETIC_ALPHABET_PICTURE));
    phoneticAlphabetButton->setPosition(ccp(0, 0));
    
    phoneticAlphabetSpriteItem = CCMenuItemSprite::create(phoneticAlphabetButton, phoneticAlphabetButton, this, menu_selector(ActorsLines::onPhoneticAlphabetClicked));
    phoneticAlphabetSpriteItem->setPosition(phoneticAlphabetPosition[chapterIndex - 1]);
    
    phoneticAlphabetSpriteItem->setTag(AL_PHONETIC_ALPHABET_NUMBER);
    
    CCSprite* closeButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(CLOSE_BUTTON));
    closeButton->setAnchorPoint(ccp(0, 0));
    closeButton->setPosition(ccp(0, 0));
    
    CCMenuItemSprite* closeSpriteItem = CCMenuItemSprite::create(closeButton, closeButton, this, menu_selector(ActorsLines::onCloseClicked));
    closeSpriteItem->setPosition(closePosition);
    
    CCSprite* wordsBackgroundButton = CCSprite::create();
    wordsBackgroundButton->setTextureRect(wordsBackgroundZone[chapterIndex - 1]);
    wordsBackgroundButton->setPosition(ccp(0, 0));
    wordsBackgroundButton->setOpacity(0);
    
    CCMenuItemSprite* wordsBackgroundSpriteItem = CCMenuItemSprite::create(wordsBackgroundButton, wordsBackgroundButton, this, menu_selector(ActorsLines::onWordsBackgroundClicked));
    wordsBackgroundSpriteItem->setPosition(wordsBackgroundPosition[chapterIndex - 1]);
    wordsBackgroundSpriteItem->setAnchorPoint(ccp(0, 0));
    
    if ((ShareData::getInstance())->getLanguageOfScene() == TRADITIONAL_CHINESE)
    {
        phoneticAlphabetSpriteItem->setVisible(true);
    }
    else
    {
        phoneticAlphabetSpriteItem->setVisible(false);
    }
    
    CCMenu* actorsLineMenu = CCMenu::create(phoneticAlphabetSpriteItem, closeSpriteItem, wordsBackgroundSpriteItem, NULL);
    
    actorsLineMenu->setPosition(ccp(0, 0));
    
    wordsLayer->addChild(actorsLineMenu);
    
    this->addChild(wordsLayer, 10);
}

void ActorsLines::showPhoneticOrNot()
{
    isShowPhonetic = (ShareData::getInstance())->getPhoneticAlphabet();
    
    wordsLayer->removeChild(word);
    
    if (isShowPhonetic)
    {
//        isShowPhonetic = false;
        
        word = CCSprite::create(CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, "Ts")->getCString());
        
        (ShareData::getInstance())->setPictureNameTs(CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, "Ts")->getCString());
        
        phoneticAlphabetButton->setDisplayFrame(imageCache->spriteFrameByName(NO_PHONETIC_ALPHABET_PICTURE));
        
    }
    else
    {
//        isShowPhonetic = true;
        
        word = CCSprite::create(CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, "T")->getCString());
        
        phoneticAlphabetButton->setDisplayFrame(imageCache->spriteFrameByName(PHONETIC_ALPHABET_PICTURE));
    }
    
    word->setPosition(wordsPosition);
    
    wordsLayer->addChild(word, -1);
    
    word->setTag(AL_WORDS_NUMBER + chapterIndex - 1);
}

#pragma mark -
#pragma mark 事件函數

void ActorsLines::onCloseClicked()
{
    if (isActionRunning)
        return;
    
    isActionRunning = true;
    
    stopOS();
    
    setBackgroundMenuVisible(false);
    
    wordsLayer->runAction(wordsMoveUpAction[chapterIndex]);
    
}

void ActorsLines::onBackgroundClicked()
{
    CCLog("onBackgroundClicked()");
    
    if (isActionRunning)
        return;
    
    isBackgroundClicked = true;
    
    onCloseClicked();
}

void ActorsLines::onPhoneticAlphabetClicked(cocos2d::CCObject *pSender)
{
    CCLog("onPhoneticAlphabetClicked");
    
    if (isActionRunning)
        return;
    
    if (!isClosing)
    {
        stopOS();
        playOS();
        
        isShowPhonetic = (ShareData::getInstance())->getPhoneticAlphabet();
        
        (ShareData::getInstance())->setPhoneticAlphabet(!isShowPhonetic);

        showPhoneticOrNot();
    }
}

void ActorsLines::onWordsBackgroundClicked()
{
    CCLog("onWordsBackgroundClicked()");
    
    if (isActionRunning)
        return;
    
    if (isClosing)
    {
        
        if (target && OpeningClickedCallBack) {
            
            (target->*OpeningClickedCallBack)();
        }
        setBackgroundMenuVisible(true);
        playWordsMoveDown();
    }
    else
    {
        if (isPlayingOS)
        {
            stopOS();
        }
        else
        {
            playOS();
        }
    }
}

void ActorsLines::wordsMoveDown()
{
    CCLog("wordsMoveDown()");
    
    isClosing = false;
    
    setBackgroundMenuVisible(true);
    
    playOS();
    
    isActionRunning = false;
}

void ActorsLines::wordsMovedUp()
{
    CCLog("wordsMovedUp()");
    
    if (isBackgroundClicked)
    {
        isBackgroundClicked = false;
        
        if (target && backgroundClickedCallBack) {
            
            (target->*backgroundClickedCallBack)();
        }
    }
    
    setBackgroundMenuVisible(false);
    
    isClosing = true;
    
    isActionRunning = false;
}

void ActorsLines::whenOSPlayCompleted()
{
    isPlayingOS = false;
}

#pragma mark -
#pragma mark 外部函數

const char* ActorsLines::getLanguageName(bool isTwoType)
{
    int languageType = (ShareData::getInstance())->getLanguageOfScene();
    const char* languageName = "";
    
    if (isTwoType)
    {
        if (languageType == TRADITIONAL_CHINESE)
        {
            languageName = "T";
        }
        else if (languageType == SIMPLE_CHINESE)
        {
            languageName = "T";
        }
        else
        {
            languageName = "E";
        }
    }
    else
    {
        if (languageType == TRADITIONAL_CHINESE)
        {
            languageName = "T";
        }
        else if (languageType == SIMPLE_CHINESE)
        {
            languageName = "C";
        }
        else
        {
            languageName = "E";
        }
    }
    
    return languageName;
}

void ActorsLines::changeLanguage()
{
    if (!strcmp(languageNameG, ""))
    {
        CCLog("changeLanguage remove texture key = %s", CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, languageNameG)->getCString());
        
        CCTextureCache::sharedTextureCache()->removeTextureForKey(CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, languageNameG)->getCString());
    }
    
    languageTypeWord = (ShareData::getInstance())->getLanguageOfScene();
    
    if (languageTypeWord == TRADITIONAL_CHINESE)
    {
        languageNameG = "T";
    }
    else if (languageTypeWord == SIMPLE_CHINESE)
    {
        languageNameG = "C";
    }
    else
    {
        languageNameG = "E";
    }
    
    if ((ShareData::getInstance())->getLanguageOfScene() == TRADITIONAL_CHINESE)
    {
        showPhoneticOrNot();
    }
    else
    {
        wordsLayer->removeChild(word);
        
        word = CCSprite::create(CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, languageNameG)->getCString());
        
        word->setPosition(wordsPosition);
        
        wordsLayer->addChild(word, -1);
        
        word->setTag(AL_WORDS_NUMBER + chapterIndex - 1);
    }
}

void ActorsLines::setInitialItmes(int chapterIndex, cocos2d::CCObject *target, SEL_CallFunc backgroundClickedCallBack, SEL_CallFunc OpeningClickedCallBack)
{
    this->chapterIndex= chapterIndex - 1;
    this->languageTypeWord = (ShareData::getInstance())->getLanguageOfScene();
    this->target = target;
    this->OpeningClickedCallBack = OpeningClickedCallBack;
    this->backgroundClickedCallBack = backgroundClickedCallBack;
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCString::createWithFormat(OS, "T", chapterIndex)->getCString());
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCString::createWithFormat(OS, "E", chapterIndex)->getCString());
    
    setWordsAction();
    
    setWord(chapterIndex);
}

void ActorsLines::playWordsMoveDown()
{
    isClosing = false;
    
    isActionRunning = true;
    
    wordsLayer->runAction(wordsMoveDownAction);
    
}

void ActorsLines::releaseObject()
{
    ShareMethod::getInstance()->stopLongSound(ANDROID_LONG_SOUND_ID);
    
    for (int i = 0; i < ChapterCount; i++)
    {
        wordsMoveUpAction[i]->release();
    }
    
    wordsMoveDownAction->release();
    
    SimpleAudioEngine::sharedEngine()->unloadEffect(CCString::createWithFormat(OS, "T", chapterIndex)->getCString());
    SimpleAudioEngine::sharedEngine()->unloadEffect(CCString::createWithFormat(OS, "E", chapterIndex)->getCString());
    
    CCLog("remove texture key = %s", CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, languageNameG)->getCString());
    
    CCTextureCache::sharedTextureCache()->removeTextureForKey(CCString::createWithFormat(WORDS_PICTURE, chapterIndex + 1, languageNameG)->getCString());
    
    CCTextureCache::sharedTextureCache()->removeTextureForKey((ShareData::getInstance())->getPictureNameTs());
    
    CCTextureCache::sharedTextureCache()->removeTextureForKey("ActorsLines.pvr.ccz");
}
