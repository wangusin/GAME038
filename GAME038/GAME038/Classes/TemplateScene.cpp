#include "TemplateScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;


TemplateScene::TemplateScene()
{

}
TemplateScene::~TemplateScene()
{
    CCLog("~TemplateScene()");
}

CCScene* TemplateScene::scene(int targetSceneId)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TemplateScene* layer = TemplateScene::create();
    
    layer->targetSceneId = targetSceneId;

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TemplateScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    targetSceneId = 0;
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
    
    CCLayerColor* background = CCLayerColor::create(ccc4(0, 0, 0, 255));
    
    this->addChild(background);
    
    return true;
}

void TemplateScene::onEnter()
{
    CCNode::onEnter();
    
    this->scheduleOnce(schedule_selector(TemplateScene::changeToScene), 0.3f);

}

void TemplateScene::changeToScene()
{
    CCLog("targetSceneId = %d", targetSceneId);
    (ChangeScene::getInstance())->changeToScene(targetSceneId);
}


