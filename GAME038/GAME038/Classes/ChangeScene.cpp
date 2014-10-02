//
//  ChangeScene.cpp
//  RedHat
//
//  Created by Mac-8 on 13/7/29.
//
//

#include "ChangeScene.h"

//#include "Scene1.h"
//#include "Scene2.h"
//#include "Scene3.h"
#include "Scene4.h"
//#include "Scene5.h"
//#include "Scene6.h"
//#include "LastScene.h"

using namespace cocos2d;

static ChangeScene* _instance = NULL;

ChangeScene* ChangeScene::getInstance()
{
    if(_instance == NULL)
    {
        _instance = new ChangeScene();
        
        if (_instance)
        {
            _instance->init();
        }
        else
        {
            CC_SAFE_DELETE(_instance);
            
            return NULL;
        }
    }
    return _instance;
};

void ChangeScene::init()
{

}

void ChangeScene::changeToScene(int targetScene)
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    if (targetScene == 0)
    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = MainScene::scene();
//        
//        // replace
//        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
    }
//    else if (targetScene == 1)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene1::scene();
//        
//        // replace
//        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
//    }
//    else if (targetScene == 2)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene2::scene();
//        
//        // replace
//        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
//    }
//    else if (targetScene == 3)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene3::scene();
//        
//        // replace
//        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
//    }
    else if (targetScene == 4)
    {
        // create a scene. it's an autorelease object
        CCScene *pScene = Scene4::scene();
        
        // replace
        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
    }
//    else if (targetScene == 5)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene5::scene();
//        
//        // replace
//        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
//    }
//    else if (targetScene == 6)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene6::scene();
//        
//        // replace
//        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
//    }
//    else if (targetScene == 7)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = LastScene::scene();
//        
//        // replace
//        pDirector->replaceScene(CCTransitionFade::create(0.5f, pScene));
//    }

}

void ChangeScene::changeToTemplateScene(int targetScene)
{
    changeSceneWithNormal(targetScene);
}

void ChangeScene::changeSceneWithNormal(int targetScene)
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    if (targetScene == 0)
    {
        // create a scene. it's an autorelease object
        CCScene *pScene = StartScene::scene();
        
        // replace
        pDirector->replaceScene(pScene);
    }
//    else if (targetScene == 1)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene1::scene();
//        
//        // replace
//        pDirector->replaceScene(pScene);
//    }
//    else if (targetScene == 2)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene2::scene();
//        
//        // replace
//        pDirector->replaceScene(pScene);
//    }
//    else if (targetScene == 3)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene3::scene();
//        
//        // replace
//        pDirector->replaceScene(pScene);
//    }
    else if (targetScene == 4)
    {
        // create a scene. it's an autorelease object
        CCScene *pScene = Scene4::scene();
        
        // replace
        pDirector->replaceScene(pScene);
    }
//    else if (targetScene == 5)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene5::scene();
//        
//        // replace
//        pDirector->replaceScene(pScene);
//    }
//    else if (targetScene == 6)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = Scene6::scene();
//        
//        // replace
//        pDirector->replaceScene(pScene);
//    }
//    else if (targetScene == 7)
//    {
//        // create a scene. it's an autorelease object
//        CCScene *pScene = LastScene::scene();
//        
//        // replace
//        pDirector->replaceScene(pScene);
//    }
    
}


