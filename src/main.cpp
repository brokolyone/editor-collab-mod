#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <map>
#include <string>

using namespace geode::prelude;

class CollabManager {
public:
    static CollabManager* get() {
        static auto instance = new CollabManager();
        return instance;
    }

    bool isHosting = false;
    bool isJoined = false;
    std::string roomCode = "";
    std::map<int, CCPoint> remoteCursors;
    std::map<int, CCSprite*> cursorSprites;

    void onHost() {
        if (isJoined) return;
        if (isHosting) {
            isHosting = false;
            roomCode = "";
            FLAlertLayer::create("Info", "Host turned off!", "OK")->show();
        } else {
            isHosting = true;
            roomCode = std::to_string(100000 + rand() % 900000);
            FLAlertLayer::create("Host Success", "Your code is: " + roomCode, "OK")->show();
        }
    }

    void onJoin(std::string code) {
        if (isHosting) return;
        if (isJoined) {
            isJoined = false;
            roomCode = "";
            FLAlertLayer::create("Info", "Disconnected!", "OK")->show();
        } else {
            isJoined = true;
            roomCode = code;
            FLAlertLayer::create("Join Success", "Connected to: " + code, "OK")->show();
        }
    }
};

class $modify(MyCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        auto menu = this->getChildByID("creator-buttons-menu");
        if (menu) {
            auto hostSprite = ButtonSprite::create("Host", "goldFont.fnt", "GJ_button_01.png", 0.7f);
            auto hostBtn = CCMenuItemSpriteExtra::create(hostSprite, this, menu_selector(MyCreatorLayer::onHostBtn));
            
            auto joinSprite = ButtonSprite::create("Join", "goldFont.fnt", "GJ_button_02.png", 0.7f);
            auto joinBtn = CCMenuItemSpriteExtra::create(joinSprite, this, menu_selector(MyCreatorLayer::onJoinBtn));

            menu->addChild(hostBtn);
            menu->addChild(joinBtn);
            menu->updateLayout();
        }
        return true;
    }

    void onHostBtn(CCObject*) { CollabManager::get()->onHost(); }
    void onJoinBtn(CCObject*) { CollabManager::get()->onJoin("456272"); }
};

class $modify(MyEditor, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelEditorLayer::init(level, p1)) return false;
        return true;
    }
};
