
#include "HelloWorldScene.h"
#include "PluginKochava/PluginKochava.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    CCLOG("Sample Startup");

    // add logo
    auto winsize = Director::getInstance()->getWinSize();
    auto logo = Sprite::create("Logo.png");
    auto logoSize = logo->getContentSize();
    logo->setPosition(Vec2(logoSize.width / 2,
                           winsize.height - logoSize.height / 2));
    addChild(logo);

    // add quit button
    auto label = Label::createWithSystemFont("QUIT", "sans", 32);
    auto quit = MenuItemLabel::create(label, [](Ref*){
        exit(0);
    });
    auto labelSize = label->getContentSize();
    quit->setPosition(Vec2(winsize.width / 2 - labelSize.width / 2 - 16,
                           -winsize.height / 2 + labelSize.height / 2 + 16));
    addChild(Menu::create(quit, NULL));

    sdkbox::PluginKochava::setAttributionCallback([](const std::map<std::string, std::string>* attr) {
        CCLOG("Kochava setAttributionCallback");
        for (auto it : *attr) {
            CCLOG("Kochava item %s:%s", it.first.c_str(), it.second.c_str());
        }
    });

    sdkbox::PluginKochava::setBeaconCallback([](const char* s) {
        CCLOG("sss");
    });
    sdkbox::PluginKochava::init();
    
    // add test menu
    createTestMenu();

    return true;
}

void HelloWorld::createTestMenu()
{
    auto menu = Menu::create();

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("track event", "sans", 24), [](Ref*){
        CCLOG("track event");
        sdkbox::PluginKochava::trackEvent("test event", "1111");
    }));
    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("retrieve attribution", "sans", 24), [](Ref*){
        CCLOG("retrieveAttribution");
        auto attrs = sdkbox::PluginKochava::retrieveAttribution();
        for (auto it : *attrs) {
            CCLOG("Kochava item %s:%s", it.first.c_str(), it.second.c_str());
        }
    }));

    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
}

