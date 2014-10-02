#include "Scene4.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


Scene4::Scene4(): goToGameMenu(NULL), functionMenu(NULL), actorsLines(NULL), imageCache(NULL), master(NULL), donkey(NULL), background(NULL), wave(NULL), land(NULL), fishAtTheFront(NULL), fishInTheBack(NULL), storyLayer(NULL), gameLayer(NULL), donkeyAction(NULL), masterPullAtFirst(NULL), masterPullAtSecond(NULL), masterPullAtThird(NULL), masterPullAtFourth(NULL), masterPullAtFiveth(NULL), fishActionAtTheFront(NULL), fishActionInTheBack(NULL), waveAction(NULL), masterButton(NULL), masterFadeIn(NULL), masterFadeOut(NULL), donkeyFadeIn(NULL), donkeyFadeOut(NULL), gameBackground(NULL), gameBackgroundFadeIn(NULL), gameBackgroundFadeOut(NULL), puzzleAction(NULL), playAgainLayer(NULL), playAgainMenu(NULL), playAgainWord(NULL), playAgainLayerMoveOut(NULL), playAgainLayerMoveIn(NULL), finishBackground(NULL), finishWord(NULL), finishAction(NULL)
{
    for (int i = 0; i < puzzlesCount; i ++)
    {
        puzzles[i] = NULL;
    }
}
Scene4::~Scene4()
{
    CCLog("~Scene4()");
}

CCScene* Scene4::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Scene4* layer = Scene4::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Scene4::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    isGameOpenning = false;
    isFishJumpedAtFront = false;
    isFishJumpedInBack = false;
    isMasterNotInScene = false;
    isMasterPressed = false;
    masterBePressedTime = 0;
    cardIndex = 0;
    
    //因應壓縮
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    
    imageCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    imageCache->removeUnusedSpriteFrames();
    
    imageCache->addSpriteFramesWithFile("S4Story1.plist");
    imageCache->addSpriteFramesWithFile("S4Story2.plist");
    imageCache->addSpriteFramesWithFile("S4Story3.plist");
    imageCache->addSpriteFramesWithFile("S4Story4.plist");
    imageCache->addSpriteFramesWithFile("S4Story5.plist");
    imageCache->addSpriteFramesWithFile("S4Story6.plist");
    imageCache->addSpriteFramesWithFile("S4Story7.plist");
    imageCache->addSpriteFramesWithFile("S4Story8.plist");
    imageCache->addSpriteFramesWithFile("S4Story9.plist");
    imageCache->addSpriteFramesWithFile("S4Game.plist");
    imageCache->addSpriteFramesWithFile("GameShareObject.plist");
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_BACKGROUND_MUSIC);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_MASTER_PULL_SECOND_PHASE_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_MASTER_PULL_THIRD_PHASE_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_MASTER_PULL_FOURTH_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_FLY_AWAY_SOUND);
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_FISH_JUMP_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_PUZZLE_MOVE_OUT_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_PUZZLE_IS_RIGHT_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_SUCCESS_SOUND);
    SimpleAudioEngine::sharedEngine()->preloadEffect(S4_PLAY_AGAIN_SOUND);
    
    //共用
    setSpriteObjectAtContent(background, ccp(0, 0), 0, S4_BACKGROUND_PICTURE, this);
    background->setAnchorPoint(ccp(0, 0));
    
    const char* waveName = CCString::createWithFormat(S4_WAVE_PICTURE, 1)->getCString();
    setSpriteObjectAtContent(wave, ccp(512, 165), 0, waveName, this);
    
    setSpriteObjectAtContent(land, ccp(344, 147), 0, S4_LAND_PICTURE, this);
    
    answersPoint[0] = ccp(154, 632);
    answersPoint[1] = ccp(384, 667);
    answersPoint[2] = ccp(639, 632);
    answersPoint[3] = ccp(891, 667);
    answersPoint[4] = ccp(132, 480);
    answersPoint[5] = ccp(384, 480);
    answersPoint[6] = ccp(640, 481);
    answersPoint[7] = ccp(869, 482);
    answersPoint[8] = ccp(155, 289);
    answersPoint[9] = ccp(384, 289);
    answersPoint[10] = ccp(639, 288);
    answersPoint[11] = ccp(890, 289);
    answersPoint[12] = ccp(133, 105);
    answersPoint[13] = ccp(384, 140);
    answersPoint[14] = ccp(638, 105);
    answersPoint[15] = ccp(868, 139);
    
    centerPoint =  ccp(512, 384);
    
    puzzleStartPosition = ccp(-800, 384);
    
    createStoryObject();
    
    createGameObject();
    
    setAllAction();
    
    gameLayer->setVisible(false);
    
    master->runAction(masterPullAtFirst);
    donkey->runAction(donkeyAction);
    wave->runAction(waveAction);
    
    finishBackground->runAction(finishAction);
    
    //進遊戲按鈕
    CCSprite* goToGameButton = CCSprite::create();
    CCRect goToGameZoneSize = CCRectMake(0, 0, 1024, 638);
    goToGameButton->setTextureRect(goToGameZoneSize);
    goToGameButton->setOpacity(0);
    
    CCMenuItemSprite* goToGameSpriteItem = CCMenuItemSprite::create(goToGameButton, goToGameButton, this, menu_selector(Scene4::gotoGame));
    goToGameSpriteItem->setPosition(ccp(0, 130));
    goToGameSpriteItem->setAnchorPoint(ccp(0.0f, 0));
    
    goToGameMenu = CCMenu::create(goToGameSpriteItem, NULL);
    goToGameMenu->setPosition(ccp(0, 0));
    goToGameMenu->setZOrder(40);
    goToGameMenu->setTouchPriority(-109);
    this->addChild(goToGameMenu);
    
    functionMenu = FunctionMenu::create();
    functionMenu->setAnchorPoint(ccp(0, 0));
    functionMenu->setPosition(ccp(0, 0));
    
    //通用元件
    functionMenu->setInitialObject(4, this, callfunc_selector(Scene4::preSceneCallBackFunc), callfunc_selector(Scene4::nextSceneCallBackFunc), callfuncND_selector(Scene4::choseSceneClickedCallFunc),  callfuncND_selector(Scene4::functionStatusCallBackFunc), callfunc_selector(Scene4::onLanguageChanged));
    
    this->addChild(functionMenu, 51);
    
    actorsLines = ActorsLines::create();
    
    actorsLines->setInitialItmes(4, this, callfunc_selector(Scene4::closeWordsWithClickedBackgroundCallBack), callfunc_selector(Scene4::openWordsCallBack));
    
    this->addChild(actorsLines, 50);
    
    this->schedule(schedule_selector(Scene4::checkPerSecond), 1.0f, -1, 0);
    
    changeLanguage();
    
    actorsLines->playWordsMoveDown();
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(S4_BACKGROUND_MUSIC, true);
    
    actorsLines->setVisible(false);
    
    gotoGame();
    
    return true;
}

#pragma mark -
#pragma mark 初始函數

void Scene4::createStoryObject()
{
    //故事
    storyLayer = CCLayer::create();
    
    this->addChild(storyLayer, 5);
    
    const char* fishName = CCString::createWithFormat(S4_FISH_PICTURE, 15)->getCString();
    
    setSpriteObjectAtContent(fishInTheBack, ccp(521, 364), 0, fishName, storyLayer);
    
    const char* masterName = CCString::createWithFormat(S4_MASTER_PULL_FIRST_PHASE_PICTURE, 1)->getCString();
    
    setSpriteObjectAtContent(master, ccp(305, 393), 5, masterName, storyLayer);
    
    CCRect masterButtonZoneSize = CCRectMake(0, 0, 300, 400);
    
    masterButton = TouchSprite::createByNoImage(masterButtonZoneSize);
    masterButton->setAnchorPoint(ccp(0, 0));
    masterButton->setOpacity(0);
    
    masterButton->setAllEventCallBack(this, menu_selector(Scene4::onMasterPress), menu_selector(Scene4::onMasterRelease), NULL);
    
    masterButton->setPosition(ccp(99, 153));
    
    this->addChild(masterButton, 40);
    
    const char* donkeyName = CCString::createWithFormat(S4_DONKEY_PICTURE, 1)->getCString();
    
    setSpriteObjectAtContent(donkey, ccp(758, 356), 5, donkeyName, storyLayer);
    
    setSpriteObjectAtContent(fishAtTheFront, ccp(752, 178), 10, fishName, storyLayer);
    
    
    CCSprite* fishAtFrontButton = CCSprite::create();
    CCRect fishAtFrontZoneSize = CCRectMake(0, 0, 410, 150);
    fishAtFrontButton->setTextureRect(fishAtFrontZoneSize);
    fishAtFrontButton->setOpacity(0);
    
    CCSprite* fishInBackButton = CCSprite::create();
    CCRect fishInBackZoneSize = CCRectMake(0, 0, 580, 140);
    fishInBackButton->setTextureRect(fishInBackZoneSize);
    fishInBackButton->setOpacity(0);
    
    CCMenuItemSprite* fishAtFrontSpriteItem = CCMenuItemSprite::create(fishAtFrontButton, fishAtFrontButton, this, menu_selector(Scene4::onFishAtFrontClicked));
    fishAtFrontSpriteItem->setPosition(ccp(615, 5));
    fishAtFrontSpriteItem->setAnchorPoint(ccp(0.0f, 0));
    
    CCMenuItemSprite* fishInBackSpriteItem = CCMenuItemSprite::create(fishInBackButton, fishInBackButton, this, menu_selector(Scene4::onFishInBackClicked));
    fishInBackSpriteItem->setPosition(ccp(443, 186));
    fishInBackSpriteItem->setAnchorPoint(ccp(0.0f, 0));
    
    CCMenu* fishMenu = CCMenu::create(fishAtFrontSpriteItem, fishInBackSpriteItem, NULL);
    fishMenu->setPosition(ccp(0, 0));
    fishMenu->setZOrder(40);
    this->addChild(fishMenu);
}

void Scene4::setAllAction()
{
    actionSettingWithBeginNumber(donkeyAction, S4_DONKEY_PICTURE, 12, -1, 0.083f, 1);
    donkeyAction->retain();
    
    actionSettingWithBeginNumber(masterPullAtFirst, S4_MASTER_PULL_FIRST_PHASE_PICTURE, 12, -1, 0.083f, 1);
    masterPullAtFirst->retain();
    
    actionSettingWithBeginNumber(masterPullAtSecond, S4_MASTER_PULL_SECOND_PHASE_PICTURE, 12, -1, 0.083, 13);
    masterPullAtSecond->retain();
    
    actionSettingWithBeginNumber(masterPullAtThird, S4_MASTER_PULL_THIRD_PHASE_PICTURE, 12, -1, 0.083, 25);
    masterPullAtThird->retain();

    CCActionInterval* fourthAction;
    actionSettingWithBeginNumber(fourthAction, S4_MASTER_PULL_FOURTH_PHASE_PICTURE, 12, 1, 0.083, 37);
    masterPullAtFourth = CCSequence::create(fourthAction, CCDelayTime::create(2.0f), CCCallFunc::create(this, callfunc_selector(Scene4::masterFlightAway)), NULL);
    masterPullAtFourth->retain();
    
    
    CCActionInterval* fivethAction;
    actionSettingWithBeginNumber(fivethAction, S4_MASTER_PULL_FIVETH_PHASE_PICTURE, 12, 1, 0.083, 49);
    masterPullAtFiveth = CCSequence::create(fivethAction, CCCallFunc::create(this, callfunc_selector(Scene4::masterWentToBack)), NULL);
    masterPullAtFiveth->retain();
    
    
    CCActionInterval* fishJumpAtFront;
    actionSettingWithBeginNumber(fishJumpAtFront, S4_FISH_PICTURE, 15, 1, 0.083, 1);
    fishActionAtTheFront = CCSequence::create(fishJumpAtFront, CCCallFunc::create(this, callfunc_selector(Scene4::fishJumpedAtFront)), NULL);
    fishActionAtTheFront->retain();
    
    
    CCActionInterval* fishJumpInBack;
    actionSettingWithBeginNumber(fishJumpInBack, S4_FISH_PICTURE, 15, 1, 0.083, 1);
    fishActionInTheBack = CCSequence::create(fishJumpInBack, CCCallFunc::create(this, callfunc_selector(Scene4::fishJumpedInBack)), NULL);
    fishActionInTheBack->retain();
    
    actionSettingWithBeginNumber(waveAction, S4_WAVE_PICTURE, 26, -1, 0.083f, 1);
    waveAction->retain();
    
    //遊戲動作
    CCFadeOut* fadeOut = CCFadeOut::create(0.01f);
    masterFadeOut = CCSequence::create(fadeOut, CCCallFunc::create(this, callfunc_selector(Scene4::onMasterFadedOut)), NULL);
    masterFadeOut->retain();
    
    CCFadeIn* fadeIn = CCFadeIn::create(1.0f);
    masterFadeIn = CCSequence::create(fadeIn, CCCallFunc::create(this, callfunc_selector(Scene4::onMasterFadedIn)), NULL);
    masterFadeIn->retain();
    
    donkeyFadeOut = CCFadeOut::create(0.01f);
    donkeyFadeOut->retain();
    
    donkeyFadeIn = CCFadeIn::create(1.0f);
    donkeyFadeIn->retain();
    
    
    CCFadeOut* backgroundFadeOut = CCFadeOut::create(1.0f);
    gameBackgroundFadeOut = CCSequence::create(backgroundFadeOut, CCCallFunc::create(this, callfunc_selector(Scene4::gameBackgroundFadedOut)), NULL);
    gameBackgroundFadeOut->retain();
    
    
    CCFadeIn* backgroundFadeIn = CCFadeIn::create(1.0f);
    gameBackgroundFadeIn = CCSequence::create(backgroundFadeIn, CCCallFunc::create(this, callfunc_selector(Scene4::gameBackgroundFadedIn)), NULL);
    gameBackgroundFadeIn->retain();
    
    CCMoveTo* puzzleMove = CCMoveTo::create(1.0f, centerPoint);
    
    CCEaseSineIn* puzzleEasyIn = CCEaseSineIn::create(puzzleMove);
    
    puzzleAction = CCSequence::create(puzzleEasyIn, CCCallFuncN::create(this, callfuncN_selector(Scene4::onPuzzleActionDone)), NULL);
    
    puzzleAction->retain();
    
    CCMoveTo* playAgainMoveOut = CCMoveTo::create(0.5f, ccp(300, 0));
    playAgainLayerMoveOut = CCSequence::create(playAgainMoveOut, CCCallFunc::create(this, callfunc_selector(Scene4::onPlayAgainLayerMovedOut)),NULL);
    playAgainLayerMoveOut->retain();
    
    CCMoveTo* playAgainMoveIn = CCMoveTo::create(0.5f, ccp(0, 0));
    playAgainLayerMoveIn = CCSequence::create(playAgainMoveIn, CCCallFunc::create(this, callfunc_selector(Scene4::onPlayAgainLayerMovedIn)),NULL);
    playAgainLayerMoveIn->retain();
    
    actionSettingWithBeginNumber(finishAction, S4_FINISH_BACKGROUND_PICTURE, 2, -1, 0.083f, 1);
    finishAction->retain();
}

void Scene4::createGameObject()
{
    gameLayer = CCLayer::create();
    
    this->addChild(gameLayer, 5);
    
    setSpriteObjectAtContent(gameBackground, ccp(0, 0), 0, S4_GAME_BACKGROUND, gameLayer);
    
    gameBackground->setAnchorPoint(ccp(0, 0));
    
    for (int i = 0; i < puzzlesCount; i++)
    {
        const char* puzzleName = CCString::createWithFormat(S4_PUZZLES_PICTURE, i + 1)->getCString();
        CCLog("puzzleName = %s", puzzleName);
        
        puzzles[i] = MoveableSprWithOffset::createBySpriteFramNameWithTouchPriority(puzzleName, -180);
        
        puzzles[i]->setPosition(puzzleStartPosition);
        
        gameLayer->addChild(puzzles[i]);
        
        puzzles[i]->setMoveable(false);
        
        puzzles[i]->setTarget(this, menu_selector(Scene4::onPuzzleTouchBegan), menu_selector(Scene4::onPuzzleTouchEnd));
        
        puzzles[i]->setTag(i);
    }
    
    //再玩一次
    
    const char* languageName = "T";
    
    playAgainLayer = CCLayer::create();
    
    playAgainLayer->setPosition(ccp(300, 0));
    
    gameLayer->addChild(playAgainLayer, 20);
    
    CCSprite* replayBackground;
    
    CCPoint replayBackgroundPosition = ccp(878, 136);
    
    setSpriteObjectAtContent(replayBackground, replayBackgroundPosition, 5, S4_REPLAY_GAME_BACKGROUND, playAgainLayer);
    
    const char* replayName = CCString::createWithFormat(S4_REPLAY_GAME_WORDS, languageName)->getCString();
    
    setSpriteObjectAtContent(playAgainWord, ccp(0, 0), 5, replayName, replayBackground);
    
    playAgainWord->setAnchorPoint(ccp(0, 0));
    
    //重打按鈕
    CCSprite* replayButton = CCSprite::create();
    CCRect replayZoneSize = CCRectMake(0, 0, 256, 154);
    replayButton->setTextureRect(replayZoneSize);
    replayButton->setOpacity(0);
    
    CCMenuItemSprite* replaySpriteItem = CCMenuItemSprite::create(replayButton, replayButton, this, menu_selector(Scene4::onReplayClicked));
    replaySpriteItem->setPosition(replayBackgroundPosition);
    
    playAgainMenu = CCMenu::create(replaySpriteItem, NULL);
    playAgainMenu->setPosition(ccp(0, 0));
    playAgainLayer->addChild(playAgainMenu);
    
    //結束圖示
    const char* finishBackgroundName = CCString::createWithFormat(S4_FINISH_BACKGROUND_PICTURE, 1)->getCString();
    
    setSpriteObjectAtContent(finishBackground, centerPoint, 40, finishBackgroundName, gameLayer);
    
    const char* finishWordName = CCString::createWithFormat(S4_FINISH_WORD_PICTURE, languageName)->getCString();
    
    setSpriteObjectAtContent(finishWord, ccp(0, 0), 5, finishWordName, finishBackground);
    finishWord->setAnchorPoint(ccp(0, 0));
    
    finishBackground->setVisible(false);
}

#pragma mark -
#pragma mark 內部函數

const char* Scene4::getLanguageName(bool isTwoType)
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

void Scene4::changeLanguage()
{
    actorsLines->changeLanguage();
    
    const char* languageName = getLanguageName(false);
    
    const char* finishWordName = CCString::createWithFormat(S4_FINISH_WORD_PICTURE, languageName)->getCString();
    
    finishWord->setDisplayFrame(imageCache->spriteFrameByName(finishWordName));
    
    const char* replayName = CCString::createWithFormat(S4_REPLAY_GAME_WORDS, languageName)->getCString();
    
    playAgainWord->setDisplayFrame(imageCache->spriteFrameByName(replayName));
    
}

void Scene4::setSpriteObjectAtContent(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* content)
{
    target = CCSprite::createWithSpriteFrame(imageCache->spriteFrameByName(name));
    
    target->setPosition(position);
    
    content->addChild(target, layerLevel);
}

void Scene4::actionSettingWithBeginNumber(CCActionInterval*& targetAction, const char* name, int count, int repeatCount, float delayPerUnit, int beginNumber)
{
    CCAnimation* targetAnimation = CCAnimation::create();
    for (int i = beginNumber ; i < count + beginNumber; i++) {
        const char* tempString = CCString::createWithFormat(name, i)->getCString();
        //CCLog("tempString = %s", tempString);
        
        if (!imageCache->spriteFrameByName(tempString))
        {
            CCLog("missing tempString = %s", tempString);
        }
        
        targetAnimation->addSpriteFrame(imageCache->spriteFrameByName(tempString));
    }
    
    targetAnimation->setDelayPerUnit(delayPerUnit);
    
    targetAction = (CCRepeat::create(CCAnimate::create(targetAnimation), repeatCount));
    
//    targetAction->retain();
    
}

void Scene4::checkPerSecond()
{
    if (isMasterPressed)
    {
        masterBePressedTime++;
        
        CCLog("masterBePressedTime = %d", masterBePressedTime);
        
        if (masterBePressedTime == 1)
        {
            master->stopAllActions();
            
            master->runAction(masterPullAtSecond);
            
            SimpleAudioEngine::sharedEngine()->playEffect(S4_MASTER_PULL_SECOND_PHASE_SOUND, false);
        }
        else if (masterBePressedTime == 3)
        {
            master->stopAllActions();
            
            master->runAction(masterPullAtThird);
            SimpleAudioEngine::sharedEngine()->playEffect(S4_MASTER_PULL_THIRD_PHASE_SOUND, false);
        }
        else if (masterBePressedTime == 7)
        {
            isMasterPressed = false;
            
            isMasterNotInScene = true;
            
            master->stopAllActions();
            
            master->runAction(masterPullAtFourth);
            SimpleAudioEngine::sharedEngine()->playEffect(S4_MASTER_PULL_FOURTH_SOUND, false);
            SimpleAudioEngine::sharedEngine()->playEffect(S4_FLY_AWAY_SOUND, false);
        }
    }
}

void Scene4::dealPuzzle()
{
    int index = randomArray[cardIndex];
    
    puzzles[index]->setZOrder(cardIndex + 1);
    
    puzzles[index]->runAction(puzzleAction);
}

void Scene4::preparePuzzle()
{
    getRandomAndNoRepeat(randomArray, puzzlesCount, 1);
    
    for (int i = 0; i < puzzlesCount; i++)
    {
        CCLog("randomArray[%d]= %d", i, randomArray[i]);
    }
}

void Scene4::getRandomAndNoRepeat(int *randomArray, int xCount, int yCount)
{

    int quantity = xCount * yCount;
    int start = 0;
    int total = xCount * yCount;
    if (quantity > total) {
        CCLog("随机数错误");
    }
    int sequence[total];  //存放随机数的数组
    //int output[quantity]; //最终生成的不重复一系列随机数
    
    //将sequence 初始化
    for (int i = 0; i < total; i++) {
        sequence[i] = start + i;
    }
    
    //随机数种子
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    unsigned long int seed = psv.tv_sec*1000 + psv.tv_usec/1000;
    srand(seed);
    
    for (int i = 0; i < quantity; i++) {
        int num = arc4random() % total;//在指定范围下产生随机数
        randomArray[i] = sequence[num];//将产生的随机数存储
        sequence[num] = sequence[total-1];//将最后个下标的值填充到随机产生的下标中
        total--;//在指定范围 向前移
    }
}

bool Scene4::checkPuzzleIsOnPosition(MoveableSprWithOffset *target)
{
    CCPoint targetPosition = target->getPosition();
    float distance = ccpDistance(targetPosition, answersPoint[target->getTag()]);
    
    if (distance <= 50.0f)
        return true;
    else
        return false;
}

void Scene4::resetPuzzle()
{
    cardIndex = 0;
    
    for (int i = 0; i < puzzlesCount; i ++)
    {
        CCActionInterval* puzzleWentOut = CCMoveTo::create(0.1f, puzzleStartPosition);
        
        puzzles[i]->runAction(puzzleWentOut);
    }
}

#pragma mark -
#pragma mark 事件函數

void Scene4::onLanguageChanged()
{
    this->unscheduleAllSelectors();
    
    //    changeLanguage();
    
    CCLog("onStartStory()");
    
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    CCTextureCache::sharedTextureCache()->removeTextureForKey((ShareData::getInstance())->getPictureName());
    CCTextureCache::sharedTextureCache()->removeTextureForKey((ShareData::getInstance())->getPictureNameTs());
    
    (ChangeScene::getInstance())->changeToScene(4);
}

void Scene4::masterFlightAway()
{
    master->stopAllActions();
    master->runAction(masterPullAtFiveth);
}

void Scene4::masterWentToBack()
{
    master->stopAllActions();
    master->runAction(masterPullAtFirst);
    isMasterNotInScene = false;
    masterBePressedTime = 0;
}

void Scene4::fishJumpedAtFront()
{
    isFishJumpedAtFront = false;
}

void Scene4::fishJumpedInBack()
{
    isFishJumpedInBack = false;
}

void Scene4::onFishAtFrontClicked()
{
    if (!isFishJumpedAtFront)
    {
        isFishJumpedAtFront = true;
        
        fishAtTheFront->runAction(fishActionAtTheFront);
        
        SimpleAudioEngine::sharedEngine()->playEffect(S4_FISH_JUMP_SOUND, false);
    }
}

void Scene4::onFishInBackClicked()
{
    if (!isFishJumpedInBack)
    {
        isFishJumpedInBack = true;
        
        fishInTheBack->runAction(fishActionInTheBack);
        
        SimpleAudioEngine::sharedEngine()->playEffect(S4_FISH_JUMP_SOUND, false);
    }
}

void Scene4::onMasterPress(cocos2d::CCObject* pTarget)
{
    if (!isMasterNotInScene)
        isMasterPressed = true;
}

void Scene4::onMasterRelease(cocos2d::CCObject* pTarget)
{
    if (isMasterPressed)
    {
        isMasterPressed = false;
        master->stopAllActions();
        master->runAction(masterPullAtFirst);
        masterBePressedTime = 0;
    }
}

void Scene4::onMasterFadedIn()
{
    goToGameMenu->setVisible(true);
    
}

void Scene4::onMasterFadedOut()
{
    storyLayer->setVisible(false);
    gameLayer->setVisible(true);
    
    gameBackground->runAction(gameBackgroundFadeIn);
}

void Scene4::gameBackgroundFadedIn()
{
    preparePuzzle();
    dealPuzzle();
}

void Scene4::gameBackgroundFadedOut()
{
    storyLayer->setVisible(true);
    gameLayer->setVisible(false);
    
    master->runAction(masterFadeIn);
    donkey->runAction(donkeyFadeIn);
}

void Scene4::onPuzzleActionDone(cocos2d::CCObject* pTarget)
{
    SimpleAudioEngine::sharedEngine()->playEffect(S4_PUZZLE_MOVE_OUT_SOUND, false);
    
    MoveableSprWithOffset* target = (MoveableSprWithOffset*)pTarget;
    
    target->setMoveable(true);
}

void Scene4::onPuzzleTouchBegan(cocos2d::CCObject *pTarget)
{
    
}

void Scene4::onPuzzleTouchEnd(cocos2d::CCObject *pTarget)
{
    MoveableSprWithOffset* target = (MoveableSprWithOffset*)pTarget;
    
    if (checkPuzzleIsOnPosition(target))
    {
        CCLog("puzzle index = %d", target->getTag());
        
        target->setPosition(answersPoint[target->getTag()]);
        
        target->setMoveable(false);
        
        cardIndex++;
        
        if (cardIndex < puzzlesCount)
        {
            SimpleAudioEngine::sharedEngine()->playEffect(S4_PUZZLE_IS_RIGHT_SOUND, false);
            dealPuzzle();
        }
        else
        {
            finishBackground->setVisible(true);
            playAgainLayer->runAction(playAgainLayerMoveIn);
            SimpleAudioEngine::sharedEngine()->playEffect(S4_SUCCESS_SOUND, false);
        }
        
    }
}

void Scene4::onReplayClicked()
{
    resetPuzzle();
    
    playAgainLayer->runAction(playAgainLayerMoveOut);
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(S4_BACKGROUND_MUSIC, true);
    
    SimpleAudioEngine::sharedEngine()->playEffect(S4_PLAY_AGAIN_SOUND, false);
    
}

void Scene4::onPlayAgainLayerMovedIn()
{
    playAgainMenu->setVisible(true);
}

void Scene4::onPlayAgainLayerMovedOut()
{
    if (isGameOpenning)
    {
        finishBackground->setVisible(false);
        cardIndex = 0;
        preparePuzzle();
        dealPuzzle();
    }
    else
    {
        playAgainLayer->setVisible(false);
    }
    
}

//共用函數
void Scene4::whenLeaving()
{
    this->unscheduleAllSelectors();
    
    donkeyAction->release();
    masterPullAtFirst->release();
    masterPullAtSecond->release();
    masterPullAtThird->release();
    
    masterPullAtFourth->release();
    masterPullAtFiveth->release();
    
    fishActionAtTheFront->release();
    fishActionInTheBack->release();
    
    waveAction->release();
    
    masterFadeOut->release();
    masterFadeIn->release();
    
    donkeyFadeOut->release();
    donkeyFadeIn->release();
    
    gameBackgroundFadeOut->release();
    gameBackgroundFadeIn->release();
    
    puzzleAction->release();
    
    playAgainLayerMoveOut->release();
    playAgainLayerMoveIn->release();
    
    finishAction->release();
    
    functionMenu->releaseObject();
    actorsLines->releaseObject();
    
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_BACKGROUND_MUSIC);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_MASTER_PULL_SECOND_PHASE_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_MASTER_PULL_THIRD_PHASE_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_MASTER_PULL_FOURTH_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_FLY_AWAY_SOUND);
    
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_FISH_JUMP_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_PUZZLE_MOVE_OUT_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_PUZZLE_IS_RIGHT_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_SUCCESS_SOUND);
    SimpleAudioEngine::sharedEngine()->unloadEffect(S4_PLAY_AGAIN_SOUND);

    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story1.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story2.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story3.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story4.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story5.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story6.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story7.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story8.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Story9.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("GameShareObject.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("S4Game.pvr.ccz");
    
    imageCache->removeSpriteFrames();
    imageCache->removeUnusedSpriteFrames();
    
    CCLog("whenLeaving!!!");
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    
}

void Scene4::gotoGame()
{
    CCLog("gotoGame()");
    
    if (!isGameOpenning)
    {
        goToGameMenu->setVisible(false);
        
        master->stopAllActions();
        master->runAction(masterFadeOut);
        
        donkey->runAction(donkeyFadeOut);
        
        isGameOpenning = true;
    }
}

void Scene4::preSceneCallBackFunc()
{
    CCLog("onStartStory()");
    
    whenLeaving();
    
//    (ChangeScene::getInstance())->changeToScene(3);
    (ChangeScene::getInstance())->changeToTemplateScene(0);
}

void Scene4::nextSceneCallBackFunc()
{
    CCLog("onStartStory()");
    
    whenLeaving();
    
//    (ChangeScene::getInstance())->changeToScene(5);
    (ChangeScene::getInstance())->changeToTemplateScene(5);
}

void Scene4::choseSceneClickedCallFunc(cocos2d::CCObject *pSender, int chapterID)
{
    CCLog("choseSceneClickedCallFunc() id = %d", chapterID);
    
    whenLeaving();
    
//    (ChangeScene::getInstance())->changeToScene(chapterID);
    (ChangeScene::getInstance())->changeToTemplateScene(chapterID);
}

void Scene4::functionStatusCallBackFunc(cocos2d::CCObject *pSender, int functionMenuStatus)
{
    const char* functionMenuStatusName = functionMenuStatus == 0 ? "false" : "true";
    
    bool menuStatus;
    
    if (functionMenuStatus == FUNCTION_MENU_CLOSING)
    {
        CCLog("menuStatus = false");
        menuStatus = false;
    }
    else
    {
        CCLog("menuStatus = true");
        menuStatus = true;
    }
    
    CCLog("functionMenuStatus = %s", functionMenuStatusName);
    
    //    setAllMenuEnable(!menuStatus);
}

void Scene4::closeWordsWithClickedBackgroundCallBack()
{
    
}

void Scene4::openWordsCallBack()
{
    
    if (isGameOpenning)
    {
        
        isGameOpenning = false;
        
        resetPuzzle();
        
        gameBackground->runAction(gameBackgroundFadeOut);
        
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(S4_BACKGROUND_MUSIC, true);
    }
}

//end

