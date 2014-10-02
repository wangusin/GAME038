#ifndef __HELLOWORLD_Scene4_H__
#define __HELLOWORLD_Scene4_H__

#include "cocos2d.h"
#include "FunctionMenu.h"
#include "ActorsLines.h"
#include "TouchSprite.h"
#include "MoveableSprWithOffset.h"

#include "ChangeScene.h"
#include "ShareData.h"

#define S4_DONKEY_PICTURE "p0401p0500%02d.png"
#define S4_BACKGROUND_PICTURE "p0401p01.png"
#define S4_WAVE_PICTURE "p0401p0200%02d.png"
#define S4_LAND_PICTURE "p0401p03.png"

#define S4_MASTER_PULL_FIRST_PHASE_PICTURE "p0401p0400%02d.png"
#define S4_MASTER_PULL_SECOND_PHASE_PICTURE "p0401p0400%02d.png"
#define S4_MASTER_PULL_THIRD_PHASE_PICTURE "p0401p0400%02d.png"
#define S4_MASTER_PULL_FOURTH_PHASE_PICTURE "p0401p0400%02d.png"
#define S4_MASTER_PULL_FIVETH_PHASE_PICTURE "p0401p0400%02d.png"
#define S4_FISH_PICTURE "p0401p0600%02d.png"

#define S4_GAME_BACKGROUND "p0402g01.png"
#define S4_PUZZLES_PICTURE "02物件大小的切法/p0402g0202000%d.png"

#define S4_REPLAY_GAME_BACKGROUND "p0102g05.png"

#define S4_REPLAY_GAME_WORDS "p0102g05%s.png"

#define S4_FINISH_BACKGROUND_PICTURE "p0402g03000%d.png"
#define S4_FINISH_WORD_PICTURE "p0402g03%s.png"

//音樂
#define S4_BACKGROUND_MUSIC "HB0026BM0401.mp3"
#define S4_MASTER_PULL_SECOND_PHASE_SOUND "THB0026OS0406.mp3"
#define S4_MASTER_PULL_THIRD_PHASE_SOUND "THB0026OS0407.mp3"
#define S4_MASTER_PULL_FOURTH_SOUND "THB0026OS0408.mp3"

#define S4_FLY_AWAY_SOUND "HB0026SE0403.mp3"
#define S4_FISH_JUMP_SOUND "HB0026SE0402.mp3"

#define S4_PUZZLE_MOVE_OUT_SOUND "HB0026SE0404.mp3"
#define S4_PUZZLE_IS_RIGHT_SOUND "HB0026SE0405.mp3"

#define S4_SUCCESS_SOUND "HB0026SE0306.mp3"
#define S4_PLAY_AGAIN_SOUND "HB0026SE0102.mp3"
class Scene4 : public cocos2d::CCLayer
{
public:
    Scene4();
    ~Scene4();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Scene4);
    
protected:
    
    //共用
    FunctionMenu* functionMenu;
    ActorsLines* actorsLines;
    
    CCMenu* goToGameMenu;
    
    bool isGameOpenning;
    
    void gotoGame();
    
    void preSceneCallBackFunc();
    
    void nextSceneCallBackFunc();
    
    void choseSceneClickedCallFunc(cocos2d::CCObject *pSender, int chapterID);
    
    void functionStatusCallBackFunc(cocos2d::CCObject *pSender, int functionMenuStatus);
    
    void closeWordsWithClickedBackgroundCallBack();
    
    void openWordsCallBack();
    
    void whenLeaving();
    
    void setSpriteObjectAtContent(cocos2d::CCSprite*& target, cocos2d::CCPoint position, int layerLevel, const char* name, CCNode* content);
    
    void actionSettingWithBeginNumber(CCActionInterval*& targetAction, const char* name, int count, int repeatCount, float delayPerUnit, int beginNumber);
    
    CCSpriteFrameCache* imageCache;
    
    //end
    
    CCSprite* master;
    CCSprite* donkey;
    
    CCSprite* background;
    CCSprite* wave;
    CCSprite* land;
    CCSprite* fishAtTheFront;
    CCSprite* fishInTheBack;
    
    void createStoryObject();
    
    CCLayer* storyLayer;
    CCLayer* gameLayer;
    
    void setAllAction();
    
    CCActionInterval* donkeyAction;
    CCActionInterval* masterPullAtFirst;
    CCActionInterval* masterPullAtSecond;
    CCActionInterval* masterPullAtThird;
    
    CCSequence* masterPullAtFourth;
    CCSequence* masterPullAtFiveth;
    
    CCSequence* fishActionAtTheFront;
    CCSequence* fishActionInTheBack;
    
    CCActionInterval* waveAction;
    
    void masterFlightAway();
    
    void masterWentToBack();
    
    void fishJumpedAtFront();
    
    void fishJumpedInBack();
    
    bool isFishJumpedAtFront;
    bool isFishJumpedInBack;
    
    bool isMasterNotInScene;
    
    void onFishAtFrontClicked();
    void onFishInBackClicked();
    
    void onMasterPress(CCObject* pTarget);
    void onMasterRelease(CCObject* pTarget);
    
    TouchSprite* masterButton;
    bool isMasterPressed;
    
    int masterBePressedTime;
    
    //每秒運行
    void checkPerSecond();
    
    CCSequence* masterFadeOut;
    CCSequence* masterFadeIn;
    
    CCActionInterval* donkeyFadeOut;
    CCActionInterval* donkeyFadeIn;
    
    void onMasterFadedOut();
    void onMasterFadedIn();
    
    //遊戲
    static const int puzzlesCount = 16;
    MoveableSprWithOffset* puzzles[puzzlesCount];
    
    CCPoint answersPoint[puzzlesCount];
    
    CCPoint centerPoint;
    
    CCPoint puzzleStartPosition;
    
    CCSprite* gameBackground;
    
    void createGameObject();
    
    CCSequence* gameBackgroundFadeOut;
    CCSequence* gameBackgroundFadeIn;
    
    void gameBackgroundFadedOut();
    void gameBackgroundFadedIn();
    
    void dealPuzzle();
    
    void getRandomAndNoRepeat(int *randomArray, int xCount, int yCount);
    
    int randomArray[puzzlesCount];
    
    void preparePuzzle();
    
    int cardIndex;
    
    CCSequence* puzzleAction;
    
    void onPuzzleActionDone(CCObject* pTarget);
    
    void onPuzzleTouchBegan(CCObject* pTarget);
    void onPuzzleTouchEnd(CCObject* pTarget);
    
    bool checkPuzzleIsOnPosition(MoveableSprWithOffset* target);
    
    //再玩一次
    CCLayer* playAgainLayer;
    CCMenu* playAgainMenu;
    CCSprite* playAgainWord;
    
    CCSequence* playAgainLayerMoveOut;
    CCSequence* playAgainLayerMoveIn;
    
    void onReplayClicked();
    
    void onPlayAgainLayerMovedIn();
    void onPlayAgainLayerMovedOut();
    //end
    
    //結束圖示
    CCSprite* finishBackground;
    CCSprite* finishWord;
    
    CCActionInterval* finishAction;
    
    const char* getLanguageName(bool isTwoType);
    
    void changeLanguage();
    
    void onLanguageChanged();
    
    void resetPuzzle();
    
};

#endif // __HELLOWORLD_Scene4_H__
