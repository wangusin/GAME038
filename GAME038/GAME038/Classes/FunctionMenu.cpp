#include "FunctionMenu.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


FunctionMenu::FunctionMenu():bell(NULL), preScene(NULL), nextScene(NULL), subMenuLayer(NULL), storyBoard(NULL), speaker(NULL), language(NULL), hint(NULL), imageCache(NULL), bellNormalAction(NULL), mainMenu(NULL), subMenu(NULL), subMenuGoDown(NULL), subMenuMoveUp(NULL), functionCloseMenu(NULL), languageMenuLayer(NULL), languageMenu(NULL), languageTW(NULL), languageCN(NULL), languageEN(NULL), highLight(NULL), selectSceneLayer(NULL), selectSceneBackground(NULL), selectSceneView(NULL), sceneLayer(NULL), backgroundWordsButton(NULL), languageItemCN(NULL), languageItemEN(NULL), languageItemTW(NULL)
{
    target = NULL;
    preSceneCallBackFunc = NULL;
    nextSceneCallBackFunc = NULL;
    choseSceneClickedCallFunc = NULL;
    functionStatusCallBackFunc = NULL;
    languageSelectCallBackFunc = NULL;
    
    for (int i = 0; i < languageCount; i++)
    {
        languageBackground[i] = NULL;
        languageShowAction[i] = NULL;
        languageHideAction[i] =  NULL;
    }
}
FunctionMenu::~FunctionMenu()
{
    target = NULL;
    preSceneCallBackFunc = NULL;
    nextSceneCallBackFunc = NULL;
    choseSceneClickedCallFunc = NULL;
    functionStatusCallBackFunc = NULL;
    languageSelectCallBackFunc = NULL;
}

// on "init" you need to initialize your instance
bool FunctionMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    isFunctionMenuOpening = false;
    isBellDoOpeningAction = false;
    isLanguageOpening = false;
    isLanguageRunning = false;
    isSoundTurnOn = true;
    isSelectSceneOpening = false;
    scrollCount = 0;
    lastScrollCount = 0;
    isScroll = false;
    idleTime = timeLimitOfWaiting;
    functionMenuWaitingTime = timeLimitOfWaiting;
    nowScene = 0;
    
    //因應壓縮
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    
    imageCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    imageCache->addSpriteFramesWithFile("FunctionMenu.plist");
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(CLICKED_BUTTON_SOUND);
    
    languagePosition[0] = ccp(65, 169);
    languagePosition[1] = ccp(65, 101);
    languagePosition[2] = ccp(65, 35);
    languagePosition[3] = ccp(65, -36);
    
    bell = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(BellPictureName));
    bell->setAnchorPoint(ccp(0.5f, 1.0f));
    bell->setPosition(ccp(507, 113));
    
    this->addChild(bell, 5);
    
    bell->setVisible(false);
    
    preScene = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(PreScenePictureName));
    preScene->setPosition(ccp(57, 39));
    
    this->addChild(preScene, 5);
    
    nextScene = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(PreScenePictureName));
    nextScene->setPosition(ccp(966, 39));
    nextScene->setFlipX(true);
    
    this->addChild(nextScene, 5);
    
    nextScene->setVisible(false);
    
    subMenuLayer = CCLayer::create();
    subMenuLayer->setContentSize(CCSizeMake(1024, 130));
    subMenuLayer->setPosition(0, -130);
    
    this->addChild(subMenuLayer, 5);
    
    
    //子功能選單
    setSpriteObjectAtSubMenuLayer(storyBoard, ccp(200, 55), 5, StoryBoardPictureName);
    setSpriteObjectAtSubMenuLayer(language, ccp(673, 48), 5, LanguagePictureName);
    setSpriteObjectAtSubMenuLayer(hint, ccp(817, 51), 5, HintPictureName);
    
    const char* speakName = CCString::createWithFormat(SpeakerPictureName, TURN_ON)->getCString();
    
    setSpriteObjectAtSubMenuLayer(speaker, ccp(348, 51), 5, speakName);
    
    languageMenuLayer = CCLayer::create();
    languageMenuLayer->setContentSize(CCSizeMake(130, 200));
    languageMenuLayer->setPosition(607, 92);
    
    subMenuLayer->addChild(languageMenuLayer, 0);
    
    for (int i = 0; i < languageCount; i++)
    {
        setSpriteObjectAtTargetLayer(languageBackground[i], languagePosition[3], 2, LANGUAGE_BACKGROUND, languageMenuLayer);
        
        setSpriteObjectAtTargetLayer(highLight, ccp(0, 0), 1, LANGUAGE_HIGHLIGHT, languageBackground[i]);
        
        highLight->setAnchorPoint(ccp(0, 0));
    }

    languageItemTW = createLanguageButton(LANGUAGE_CHINESE_T, 0);
    
    languageItemCN = createLanguageButton(LANGUAGE_CHINESE_S, 1);
    
    languageItemEN = createLanguageButton(LANGUAGE_ENGLISH, 2);
    
    languageMenu = CCMenu::create(languageItemTW, languageItemCN, languageItemEN, NULL);
    
    languageMenu->setPosition(ccp(0, 0));
    
    languageMenuLayer->addChild(languageMenu, 40);
    
    setAllLanguageTouchCanTouchable(false);
    
    setSpriteObjectAtTargetLayer(languageTW, ccp(0, 0), 5, LANGUAGE_CHINESE_T,languageBackground[0]);
    languageTW->setAnchorPoint(ccp(0, 0));

    setSpriteObjectAtTargetLayer(languageCN, ccp(0, 0), 5, LANGUAGE_CHINESE_S,languageBackground[1]);
    languageCN->setAnchorPoint(ccp(0, 0));
    
    setSpriteObjectAtTargetLayer(languageEN, ccp(0, 0), 5, LANGUAGE_ENGLISH,languageBackground[2]);
    languageEN->setAnchorPoint(ccp(0, 0));
    
    languageMenuLayer->setVisible(false);
    
    //設定所有的動作
    setAllAction();
    
    createHintObjects();
    
    //主要按鈕
    CCSprite* bellButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(BellPictureName));
    bellButton->setOpacity(0);
    
//    CCMenuItemSprite* bellSpriteItem = CCMenuItemSprite::create(bellButton, bellButton, this, menu_selector(FunctionMenu::onBellClicked));
    CCMenuItemSprite* bellSpriteItem = CCMenuItemSprite::create(bellButton, bellButton, this, NULL);
    bellSpriteItem->setPosition(ccp(507, 113));
    bellSpriteItem->setAnchorPoint(ccp(0.5f, 1.0f));
    
    CCSprite* preButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(PreScenePictureName));
    preButton->setOpacity(0);
    
    CCMenuItemSprite* preSpriteItem = CCMenuItemSprite::create(preButton, preButton, this, menu_selector(FunctionMenu::onPreSceneClicked));
    preSpriteItem->setPosition(ccp(57, 39));
    
    CCSprite* nextButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(PreScenePictureName));
    nextButton->setOpacity(0);
    
//    CCMenuItemSprite* nextSpriteItem = CCMenuItemSprite::create(nextButton, nextButton, this, menu_selector(FunctionMenu::onNextSceneClicked));
    CCMenuItemSprite* nextSpriteItem = CCMenuItemSprite::create(nextButton, nextButton, this, NULL);
    nextSpriteItem->setPosition(ccp(966, 39));
    
    mainMenu = CCMenu::create(bellSpriteItem, preSpriteItem, nextSpriteItem, NULL);
    mainMenu->setPosition(ccp(0, 0));
    mainMenu->setZOrder(40);
    this->addChild(mainMenu);
    
    //關閉按鈕
    CCSprite* functionMenuCloseButton = CCSprite::create();
    CCRect functionMenuSize = CCRectMake(0, 0, 1024, 768);
    functionMenuCloseButton->setTextureRect(functionMenuSize);
    functionMenuCloseButton->setOpacity(0);
    
    CCMenuItemSprite* startSpriteItem = CCMenuItemSprite::create(functionMenuCloseButton, functionMenuCloseButton, this, menu_selector(FunctionMenu::onCloseButtonClicked));
    startSpriteItem->setPosition(ccp(0, 0));
    startSpriteItem->setAnchorPoint(ccp(0, 0));
    
    functionCloseMenu = CCMenu::create(startSpriteItem, NULL);
    functionCloseMenu->setPosition(ccp(0, 0));
    functionCloseMenu->setZOrder(0);
    functionCloseMenu->setTouchPriority(-100);
    this->addChild(functionCloseMenu);
    
    functionCloseMenu->setVisible(false);
    
    //子選單按鈕
    //語言按鈕
    CCSprite* languageButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(LanguagePictureName));
    languageButton->setOpacity(0);
    
    CCMenuItemSprite* languageSpriteItem = CCMenuItemSprite::create(languageButton, languageButton, this, menu_selector(FunctionMenu::onLanguageClicked));
    languageSpriteItem->setPosition(ccp(673, 48));
    
    //聲音按鈕
    CCSprite* speakerButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(speakName));
    speakerButton->setOpacity(0);
    
    CCMenuItemSprite* speakerSpriteItem = CCMenuItemSprite::create(speakerButton, speakerButton, this, menu_selector(FunctionMenu::onSoundClicked));
    speakerSpriteItem->setPosition(ccp(348, 51));
    
    //分鏡按鈕
    CCSprite* sceneButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(StoryBoardPictureName));
    speakerButton->setOpacity(0);
    
    CCMenuItemSprite* sceneSpriteItem = CCMenuItemSprite::create(sceneButton, sceneButton, this, menu_selector(FunctionMenu::onSceneClicked));
    sceneSpriteItem->setPosition(ccp(200, 55));
    
    //提示按鈕
    CCSprite* hintButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(HintPictureName));
    hintButton->setOpacity(0);
    
    CCMenuItemSprite* hintSpriteItem = CCMenuItemSprite::create(hintButton, hintButton, this, menu_selector(FunctionMenu::onHintClicked));
    hintSpriteItem->setPosition(ccp(817, 51));
    
    subMenu = CCMenu::create(languageSpriteItem, speakerSpriteItem, sceneSpriteItem, hintSpriteItem, NULL);
    subMenu->setPosition(ccp(0, 0));
    
    subMenuLayer->addChild(subMenu);
    
    //阻擋按鈕
    CCSprite* blockButton = CCSprite::create();
    CCRect blockSize = CCRectMake(0, 0, 1024, 768);
    blockButton->setTextureRect(blockSize);
    blockButton->setOpacity(100);
    
    CCMenuItemSprite* blockSpriteItem = CCMenuItemSprite::create(blockButton, blockButton, this, NULL);
    blockSpriteItem->setPosition(ccp(0, 0));
    blockSpriteItem->setAnchorPoint(ccp(0, 0));
    
    blockMenu = CCMenu::create(blockSpriteItem, NULL);
    blockMenu->setPosition(ccp(0, 0));
    blockMenu->setZOrder(200);
    blockMenu->setTouchPriority(-255);
    this->addChild(blockMenu);
    
    blockMenu->setVisible(false);
    
    //執行所有動作
    bell->runAction(CCRepeatForever::create(bellNormalAction));
    
    changeLanguage();
    
    this->schedule(schedule_selector(FunctionMenu::functionSchedule), perDuringTime);
    
    return true;
}

void FunctionMenu::scrollViewDidScroll(CCScrollView* view){
    CCLOG("in Scroll");
    scrollCount++;
    isScroll = true;
}
void FunctionMenu::scrollViewDidZoom(CCScrollView* view){
    CCLOG("in Zoom");
}

#pragma mark -
#pragma mark 內部函數

void FunctionMenu::setAllLanguageTouchCanTouchable(bool touchable)
{
    languageMenu->setVisible(touchable);
}

CCMenuItemSprite* FunctionMenu::createLanguageButton(const char *frameName, int tag)
{
    CCSprite* target = TouchSprite::createBySpriteFrameName(frameName);
    
    target->setOpacity(0);
    
    CCMenuItemSprite* itemSprite = CCMenuItemSprite::create(target, target, this, menu_selector(FunctionMenu::onLanguageTouched));
    
    itemSprite->setTag(tag);

    itemSprite->setPosition(languagePosition[tag]);
    
    return itemSprite;

}

void FunctionMenu::setSprite(cocos2d::CCSprite *target, cocos2d::CCPoint position, int level, const char *name)
{
    target = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(name));
    
    target->setPosition(position);
    
    this->addChild(target, level);
}

void FunctionMenu::setAllAction()
{
    setBellAction();
    setSubMenuAction();
    setLanguageMenuAction();
    
    setSelectScene();
}

void FunctionMenu::setBellAction()
{
    CCActionInterval* rotateToRight = CCRotateTo::create(1.0f, -30);
    
    CCActionInterval* rotateToRightReverse = CCRotateTo::create(1.0f, 0);
    
    CCActionInterval* rotateToLeft = CCRotateTo::create(1.0f, 30);
    
    CCActionInterval* rotateToLeftReverse = CCRotateTo::create(1.0f, 0);
    
    bellNormalAction = CCSequence::create(rotateToRight, rotateToRightReverse, rotateToLeft, rotateToLeftReverse, rotateToRight, rotateToRightReverse, rotateToLeft, rotateToLeftReverse, NULL);
    
    bellNormalAction->retain();
    
    CCActionInterval* rotateToRightClick = CCRotateTo::create(0.2f, -30);
    
    CCActionInterval* rotateToRightReverseClick = CCRotateTo::create(0.2f, 0);
    
    CCActionInterval* rotateToLeftClick = CCRotateTo::create(0.2f, 30);
    
    CCActionInterval* rotateToLeftReverseClick = CCRotateTo::create(0.2f, 0);
    
    bellClickedAction = CCSequence::create(rotateToRightClick, rotateToRightReverseClick, rotateToLeftClick, rotateToLeftReverseClick, CCCallFunc::create(this, callfunc_selector(FunctionMenu::onBellPlayed)), NULL);
    
    bellClickedAction->retain();
}

void FunctionMenu::setSpriteObjectAtSubMenuLayer(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name)
{
    setSpriteObjectAtTargetLayer(target, position, layerLevel, name, subMenuLayer);
}

void FunctionMenu::setSpriteObjectAtTargetLayer(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* contents)
{
    target = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(name));
    
    target->setPosition(position);
    
    contents->addChild(target, layerLevel);
}

void FunctionMenu::setSubMenuAction()
{
    CCActionInterval* menuMoveUp = CCMoveTo::create(0.5f, ccp(0, 0));
    
    CCActionInterval* menuGoDown = CCMoveTo::create(0.5f, ccp(0, -130));
    
    subMenuMoveUp = CCSequence::create(menuMoveUp, CCCallFunc::create(this, callfunc_selector(FunctionMenu::onSubMenuMovedUp)), NULL);
    
    subMenuGoDown = CCSequence::create(menuGoDown, CCCallFunc::create(this, callfunc_selector(FunctionMenu::onSubMenuWentDown)), NULL);
    
    subMenuMoveUp->retain();
    subMenuGoDown->retain();
}

void FunctionMenu::checkAndDoForSubMenu()
{
    functionMenuWaitingTime = timeLimitOfWaiting;
    
    if (isFunctionMenuOpening)
    {
        isFunctionMenuOpening = false;
        
        if (isLanguageOpening)
        {
            for (int i = 0; i < languageCount; i++)
            {
                languageBackground[i]->setPosition(languagePosition[3]);
            }
            setAllLanguageTouchCanTouchable(false);
            languageMenuLayer->setVisible(false);
            
            isLanguageOpening = false;
        }
        
        subMenuLayer->runAction(subMenuGoDown);
        
        if (target && functionStatusCallBackFunc) {
            (target->*functionStatusCallBackFunc)(bell, (void*) FUNCTION_MENU_CLOSING);
        }
    }
    else
    {
        isFunctionMenuOpening = true;
        
        subMenuLayer->runAction(subMenuMoveUp);
        
        if (target && functionStatusCallBackFunc) {
            (target->*functionStatusCallBackFunc)(bell, (void*) FUNCTION_MENU_OPENING);
        }
    }
}

void FunctionMenu::setLanguageMenuAction()
{
    CCActionInterval* showBounce[languageCount];
    
    CCActionInterval* hideBounce[languageCount];
    
    for (int i = 0; i < languageCount; i++)
    {
        showBounce[i] = CCMoveTo::create(0.5f, languagePosition[i]);
        
        showBounce[i] = CCEaseBounceOut::create(showBounce[i]);
        
        languageShowAction[i] = CCSequence::create(showBounce[i], CCCallFunc::create(this, callfunc_selector(FunctionMenu::onLanguageShoeDone)), NULL);
        
        languageShowAction[i]->retain();
        
        hideBounce[i] = CCMoveTo::create(0.5f, languagePosition[3]);
        
        hideBounce[i] = CCEaseBounceOut::create(hideBounce[i]);
        
        languageHideAction[i] = CCSequence::create(hideBounce[i], CCCallFunc::create(this, callfunc_selector(FunctionMenu::onLanguageHideDone)), NULL);
        
        languageHideAction[i]->retain();
    }
    
}

void FunctionMenu::onLanguageShoeDone()
{
    isLanguageRunning = false;
    
    setAllLanguageTouchCanTouchable(true);
    languageMenuLayer->setVisible(true);
}

void FunctionMenu::onLanguageHideDone()
{
    isLanguageRunning = false;
    
    setAllLanguageTouchCanTouchable(false);
    languageMenuLayer->setVisible(false);
}

void FunctionMenu::setSoundSwitch(bool onOff)
{
    const char* speakName;
    
    if (onOff)
    {
        speakName = CCString::createWithFormat(SpeakerPictureName, TURN_ON)->getCString();
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(100);
    }
    else
    {
        speakName = CCString::createWithFormat(SpeakerPictureName, TURN_OFF)->getCString();
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
    }
    
    speaker->setDisplayFrame(imageCache->spriteFrameByName(speakName));
}

bool FunctionMenu::checkIsCanDoAction()
{
    if (isLanguageRunning || isBellDoOpeningAction || !isFunctionMenuOpening)
        return false;
    else
        return true;
}

void FunctionMenu::setSelectScene()
{
    //主要的分鏡的layer
    sceneLayer = CCLayer::create();
    sceneLayer->setContentSize(CCSizeMake(1024, 768));
    sceneLayer->setAnchorPoint(ccp(0, 0));
    sceneLayer->setPosition(0, 0);
    
    selectSceneView = CCScrollView::create();
    
    selectSceneView->setAnchorPoint(CCPointZero);
    selectSceneView->setPosition(ccp(0, 0));
    
    selectSceneView->setContentSize(CCSizeMake(2226, 238));
    selectSceneView->setViewSize(CCSizeMake(1024, 238));
    
    sceneLayer->addChild(selectSceneView, 50);
    
    selectSceneLayer = CCLayer::create();
    selectSceneLayer->setContentSize(CCSizeMake(2226, 238));
    
    selectSceneLayer->setAnchorPoint(CCPointZero);
    
    selectSceneView->setContainer(selectSceneLayer);
    selectSceneView->setContentOffset(ccp(0, 0));
    
    CCMenuItemSprite* sceneSpriteItem[sceneNumber];
    
    for (int i = 0; i < sceneNumber; i ++)
    {
        const char* sceneName = CCString::createWithFormat(SELECT_SCENE_PICTURE, i + 1)->getCString();
        
        CCSprite* scene;
        
        scene = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(sceneName));
        
        scene->setAnchorPoint(CCPointZero);
        
        sceneSpriteItem[i] = CCMenuItemSprite::create(scene, scene, this, menu_selector(FunctionMenu::onSelectSceneClicked));
        sceneSpriteItem[i]->setPosition(ccp(318 * i, 0));
        sceneSpriteItem[i]->setAnchorPoint(ccp(0.0f, 0));
        
        sceneSpriteItem[i]->setTag(SCENE_SHORTCUT_NUMBER + i);
    }
    
    CCMenu* selectSceneMenu = CCMenu::create(sceneSpriteItem[0], sceneSpriteItem[1], sceneSpriteItem[2], sceneSpriteItem[3], sceneSpriteItem[4], sceneSpriteItem[5], sceneSpriteItem[6], NULL);
    selectSceneMenu->setPosition(ccp(0, 0));
    selectSceneMenu->setZOrder(40);
    selectSceneMenu->setTouchPriority(-195);
    selectSceneLayer->addChild(selectSceneMenu);
    
    setSpriteObjectAtTargetLayer(selectSceneBackground, ccp(0, 0), 40, SELECT_SCENE_BACKGROUND, sceneLayer);
    
    selectSceneBackground->setAnchorPoint(CCPointZero);
    
    CCSprite* closeSeleceSceneButton = CCSprite::create();
    CCRect closeSeleceSceneZoneSize = CCRectMake(0, 0, 1024, 530);
    closeSeleceSceneButton->setTextureRect(closeSeleceSceneZoneSize);
    closeSeleceSceneButton->setOpacity(0);
    
    CCMenuItemSprite* closeSeleceSceneSpriteItem = CCMenuItemSprite::create(closeSeleceSceneButton, closeSeleceSceneButton, this, menu_selector(FunctionMenu::onCloseSceneClicked));
    closeSeleceSceneSpriteItem->setPosition(ccp(0, 238));
    closeSeleceSceneSpriteItem->setAnchorPoint(ccp(0.0f, 0));
    
    CCMenu* closeSceneMenu = CCMenu::create(closeSeleceSceneSpriteItem, NULL);
    closeSceneMenu->setPosition(ccp(0, 0));
    closeSceneMenu->setZOrder(40);
    sceneLayer->addChild(closeSceneMenu);
    
    CCSprite* maskButton = CCSprite::create();
    CCRect maskZoneSize = CCRectMake(0, 0, 1024, 250);
    maskButton->setTextureRect(maskZoneSize);
    maskButton->setOpacity(0);
    
    CCMenuItemSprite* maskSpriteItem = CCMenuItemSprite::create(maskButton, maskButton, this, NULL);
    maskSpriteItem->setPosition(ccp(0, 0));
    maskSpriteItem->setAnchorPoint(ccp(0.0f, 0));
    
    CCMenu* maskMenu = CCMenu::create(maskSpriteItem, NULL);
    maskMenu->setPosition(ccp(0, 0));
    maskMenu->setZOrder(40);
    
    maskMenu->setTouchPriority(-190);
    
    sceneLayer->addChild(maskMenu);
    
//    maskMenu->setTouchEnabled(false);
    
    selectSceneView->setDirection(kCCScrollViewDirectionHorizontal);
    
    selectSceneView->setTouchPriority(-199);
    
    selectSceneView->setTouchEnabled(false);
    
    selectSceneLayer->setVisible(false);
    
    selectSceneView->setVisible(false);
    
    sceneLayer->setVisible(false);
    
    selectSceneView->setDelegate(this);
    
    this->addChild(sceneLayer, 50);
}

void FunctionMenu::setAllFunctionMenu(bool enable)
{
    mainMenu->setVisible(enable);
    subMenu->setVisible(enable);
    
    functionCloseMenu->setVisible(enable);
}

void FunctionMenu::setSelectViewObject(bool enable)
{
    selectSceneView->setTouchEnabled(enable);
    
    selectSceneLayer->setVisible(enable);
    
    selectSceneView->setVisible(enable);
    
    sceneLayer->setVisible(enable);
    
    isSelectSceneOpening = enable;
}

void FunctionMenu::checkIsScrolling()
{
    if (lastScrollCount == scrollCount)
    {
        isScroll = false;
        scrollCount = 0;
        lastScrollCount = 0;
    }
    else
    {
        lastScrollCount = scrollCount;
        idleTime = timeLimitOfWaiting;
    }
}

void FunctionMenu::functionSchedule()
{
    checkIsScrolling();
    checkIsWaitingOverTimeLimit();
    checkIsFunctionMenuOpened();
}

void FunctionMenu::checkIsWaitingOverTimeLimit()
{
    if (isSelectSceneOpening)
    {
        idleTime -= perDuringTime;
        
        if (idleTime <= 0.0f)
        {
            //因為沒用到，所以隨便回傳一個物件
            onCloseSceneClicked();
            idleTime = timeLimitOfWaiting;
        }
    }
    else
    {
        idleTime = timeLimitOfWaiting;
    }
    
}

void FunctionMenu::checkIsFunctionMenuOpened()
{
    if (isFunctionMenuOpening)
    {
        if (functionMenuWaitingTime < 0)
        {
            checkAndDoForSubMenu();
            functionMenuWaitingTime = timeLimitOfWaiting;
        }
        else
        {
            functionMenuWaitingTime -= perDuringTime;
        }
        
    }
}

int FunctionMenu::playSoundEffect(const char *soundName, bool isRepeat)
{
    int soundId =  SimpleAudioEngine::sharedEngine()->playEffect(soundName, isRepeat);
    
    return soundId;
}

void FunctionMenu::createHintObjects()
{
    hintLayer = CCLayer::create();
    
    this->addChild(hintLayer, 40);
    
    //提示按鈕
    CCSprite* backgroundButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(HINT_BACKGROUND_PICTURE));
    
    CCMenuItemSprite* backgroundSpriteItem = CCMenuItemSprite::create(backgroundButton, backgroundButton, this, NULL);
    backgroundSpriteItem->setPosition(ccp(0, 0));
    backgroundSpriteItem->setAnchorPoint(ccp(0, 0));
    
    const char* languageName = "T";
    
    const char* hintWordName = CCString::createWithFormat(HINT_WORDS_PICTURE, languageName)->getCString();
    
    backgroundWordsButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(hintWordName));
    
    CCMenuItemSprite* backgroundWordsSpriteItem = CCMenuItemSprite::create(backgroundWordsButton, backgroundWordsButton, this, NULL);
    backgroundWordsSpriteItem->setPosition(ccp(0, 0));
    backgroundWordsSpriteItem->setAnchorPoint(ccp(0, 0));
    
    CCMenu* hintMenu = CCMenu::create(backgroundSpriteItem, backgroundWordsSpriteItem, NULL);
    hintMenu->setPosition(ccp(0, 0));
    
    hintMenu->setTouchPriority(-190);
    
    hintLayer->addChild(hintMenu);
    
    CCSprite* closeButton = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(FUNC_CLOSE_BUTTON));
    
    CCMenuItemSprite* closeSpriteItem = CCMenuItemSprite::create(closeButton, closeButton, this, menu_selector(FunctionMenu::closeHineLayer));
    closeSpriteItem->setPosition(ccp(998, 741));
    
    CCMenu* closeMenu = CCMenu::create(closeSpriteItem, NULL);
    closeMenu->setPosition(ccp(0, 0));
    
    closeMenu->setTouchPriority(-191);
    
    hintLayer->addChild(closeMenu);
    
    hintLayer->setVisible(false);
}

void FunctionMenu::changeLanguage()
{
    int languageType = (ShareData::getInstance())->getLanguageOfScene();
    
    const char* languageName = "";
    
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
    
    const char* hintWordName = CCString::createWithFormat(HINT_WORDS_PICTURE, languageName)->getCString();
    
    backgroundWordsButton->setDisplayFrame(imageCache->spriteFrameByName(hintWordName));
}

void FunctionMenu::languageEvent()
{
    if (target && languageSelectCallBackFunc) {
        (target->*languageSelectCallBackFunc)();
    }
}

void FunctionMenu::nextSceneEvent()
{
    if (target && nextSceneCallBackFunc) {
        (target->*nextSceneCallBackFunc)();
    }
}

void FunctionMenu::preSceneEvent()
{
    if (target && preSceneCallBackFunc) {
        (target->*preSceneCallBackFunc)();
    }
}

#pragma mark -
#pragma mark 事件觸發
void FunctionMenu::onLanguageTouched(TouchSprite *target)
{
    CCLog("onLanguageTouched = %d", target->getTag());
    
    blockMenu->setVisible(true);
    
    (ShareData::getInstance())->setLanguageOfScene((GameLanguageType)target->getTag());
    
    changeLanguage();
    
    scheduleOnce(schedule_selector(FunctionMenu::languageEvent), 0.1f);
}

void FunctionMenu::closeHineLayer()
{
    hintLayer->setVisible(false);
}

void FunctionMenu::onHintClicked()
{
    hintLayer->setVisible(true);
}

void FunctionMenu::onNextSceneClicked()
{
    blockMenu->setVisible(true);
    
    scheduleOnce(schedule_selector(FunctionMenu::nextSceneEvent), 0.1f);
}

void FunctionMenu::onPreSceneClicked()
{
    blockMenu->setVisible(true);
    
    scheduleOnce(schedule_selector(FunctionMenu::preSceneEvent), 0.1f);
}

void FunctionMenu::onBellClicked()
{
    if (isLanguageRunning)
        return;
    
    if (!isBellDoOpeningAction)
    {
        CCLog("onBellClicked()");
        
        playSoundEffect(CLICKED_BUTTON_SOUND, false);
        
        isBellDoOpeningAction = true;
        
        bell->stopAllActions();
        
        bell->runAction(bellClickedAction);
        
        checkAndDoForSubMenu();
    }
}

void FunctionMenu::onBellPlayed()
{
    CCLog("onBellPlayed()");
    
    isBellDoOpeningAction = false;
    
//    checkAndDoForSubMenu();
}

void FunctionMenu::onSubMenuMovedUp()
{
    functionCloseMenu->setVisible(true);
}

void FunctionMenu::onSubMenuWentDown()
{
    bell->runAction(CCRepeatForever::create(bellNormalAction));
    
    functionCloseMenu->setVisible(false);
}

void FunctionMenu::onCloseButtonClicked()
{
    if (!checkIsCanDoAction())
        return;
    
    checkAndDoForSubMenu();
}

void FunctionMenu::onLanguageClicked()
{
    if (!checkIsCanDoAction())
        return;
    
    CCLog("FunctionMenu::onLanguageClicked()");
    
    playSoundEffect(CLICKED_BUTTON_SOUND, false);
    
    functionMenuWaitingTime = timeLimitOfWaiting;
    
    if (isLanguageOpening)
    {
        for (int i = 0; i < languageCount; i++)
        {
            languageBackground[i]->runAction(languageHideAction[i]);
        }
        
        isLanguageOpening = false;
        
        isLanguageRunning = true;
    }
    else
    {
        setAllLanguageTouchCanTouchable(true);
        languageMenuLayer->setVisible(true);
        
        for (int i = 0; i < languageCount; i++)
        {
            languageBackground[i]->runAction(languageShowAction[i]);
        }
        
        isLanguageOpening = true;
        
        isLanguageRunning = true;
    }
}

void FunctionMenu::onSoundClicked()
{
    if (isBellDoOpeningAction || !isFunctionMenuOpening)
        return;
    
    playSoundEffect(CLICKED_BUTTON_SOUND, false);
    
    functionMenuWaitingTime = timeLimitOfWaiting;
    
    CCLog("onSoundClicked()");
    
    isSoundTurnOn = !isSoundTurnOn;
    
    setSoundSwitch(isSoundTurnOn);
}

void FunctionMenu::onSceneClicked()
{
    playSoundEffect(CLICKED_BUTTON_SOUND, false);
    
    setSelectViewObject(true);
    
    setAllFunctionMenu(false);
}

void FunctionMenu::onCloseSceneClicked()
{
    setSelectViewObject(false);
    
    setAllFunctionMenu(true);
}

void FunctionMenu::onSelectSceneClicked(cocos2d::CCObject *pSender)
{
    CCLog("onSelectSceneClicked");
    
    if (isScroll)
        return;
    
    playSoundEffect(CLICKED_BUTTON_SOUND, false);

    blockMenu->setVisible(true);
    
    int sceneId = ((CCMenuItemSprite*)pSender)->getTag() % SCENE_SHORTCUT_NUMBER;
    
    if (target && choseSceneClickedCallFunc) {
        (target->*choseSceneClickedCallFunc)(((CCMenuItemSprite*)pSender), (void*)(sceneId));
    }
}


#pragma mark -
#pragma mark 外部函數

void FunctionMenu::setInitialObject(int nowScene, cocos2d::CCObject *target, SEL_CallFunc preSceneCallBackFunc, SEL_CallFunc nextSceneCallBackFunc, SEL_CallFuncND choseSceneClickedCallFunc, SEL_CallFuncND functionStatusCallBackFunc, SEL_CallFunc languageSelectCallBackFunc)
{
    this->nowScene = nowScene;
    this->target = target;
    this->preSceneCallBackFunc = preSceneCallBackFunc;
    this->nextSceneCallBackFunc = nextSceneCallBackFunc;
    this->choseSceneClickedCallFunc = choseSceneClickedCallFunc;
    this->functionStatusCallBackFunc = functionStatusCallBackFunc;
    this->languageSelectCallBackFunc = languageSelectCallBackFunc;
}

void FunctionMenu::releaseObject()
{
    this->unschedule(schedule_selector(FunctionMenu::functionSchedule));
    
    setSelectViewObject(false);
    
    bellNormalAction->release();
    bellClickedAction->release();
    
    subMenuMoveUp->release();
    subMenuGoDown->release();
    
    for (int i = 0; i < languageCount; i++)
    {
        languageShowAction[i]->release();
        languageHideAction[i]->release();
    }
    
    SimpleAudioEngine::sharedEngine()->unloadEffect(CLICKED_BUTTON_SOUND);
    
    CCTextureCache::sharedTextureCache()->removeTextureForKey("FunctionMenu.pvr.ccz");
}
