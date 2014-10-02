//
// 100 Game AppDelegate.cpp
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "StartScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1024, 768, kResolutionShowAll);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //IOS
    CCUserDefault *dataRecord = CCUserDefault::sharedUserDefault();
    
    const char* plistPath = "InfoPlist.strings";
    const char* fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile("InfoPlist.strings", plistPath);
    CCDictionary* plistDic = CCDictionary::createWithContentsOfFile(fullPath);
    CCString* languageInit = dynamic_cast<CCString*>(plistDic->objectForKey("LanguageInit"));
    
    dataRecord->setIntegerForKey("Record_Language", languageInit->intValue());
    
    dataRecord->flush();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCUserDefault *dataRecord = CCUserDefault::sharedUserDefault();
    
    ccLanguageType languagType=CCApplication::sharedApplication()->getCurrentLanguage();
    
    switch (languagType)
    {
        case cocos2d::kLanguageChinese:
            dataRecord->setIntegerForKey("Record_Language", 1);
            break;
        case cocos2d::kLanguageEnglish:
            dataRecord->setIntegerForKey("Record_Language", 2);
            break;
        case cocos2d::kLanguageTraditional:
            dataRecord->setIntegerForKey("Record_Language", 0);
            break;
        default:
            dataRecord->setIntegerForKey("Record_Language", 0);
            break;
    }
    dataRecord->flush();
#endif
    
    // create a scene. it's an autorelease object
    CCScene *pScene = StartScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
#endif
}
