#ifndef __HELLOWORLD_TemplateScene_H__
#define __HELLOWORLD_TemplateScene_H__

#include "cocos2d.h"
#include "ChangeScene.h"


class TemplateScene : public cocos2d::CCLayer
{
public:
    TemplateScene();
    ~TemplateScene();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene(int targetSceneId);
    
    virtual void onEnter();
    

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(TemplateScene);
    
protected:
    
    int targetSceneId;
    
    void changeToScene();
    
};

#endif // __HELLOWORLD_TemplateScene_H__
