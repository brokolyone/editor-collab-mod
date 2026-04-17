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

    void host() {
        isHosting = true;
        roomCode = std::to_string(100000 + rand() % 900000); 
        FLAlertLayer::create("Host Created", "Your code: " + roomCode, "OK")->show();
    }

    void stopHost() {
        isHosting = false;
        FLAlertLayer::create("Info", "Host OFF", "OK")->show();
    }

    void join(std::string code) {
        isJoined = true;
        roomCode = code;
        FLAlertLayer::create("Joined!", "Code: " + code, "OK")->show();
    }

    void disconnect() {
        isJoined = false;
        FLAlertLayer::create("Info", "Disconnected", "OK")->show();
    }
};

class $modify(MyCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        auto menu = this->getChildByID("creator-buttons-menu");
        if (!menu) return true;

        // Используем обычные ButtonSprite
        auto hostSprite = ButtonSprite::create("Host", "goldFont.fnt", "GJ_button_01.png", 0.7f);
        auto hostBtn = CCMenuItemSpriteExtra::create(
            hostSprite,
            this,
            menu_selector(MyCreatorLayer::onHost)
        );
        hostBtn->setID("host-button");

        auto joinSprite = ButtonSprite::create("Join", "goldFont.fnt", "GJ_button_02.png", 0.7f);
        auto joinBtn = CCMenuItemSpriteExtra::create(
            joinSprite,
            this,
            menu_selector(MyCreatorLayer::onJoin)
        );
        joinBtn->setID("join-button");

        menu->addChild(hostBtn);
        menu->addChild(joinBtn);
        menu->updateLayout();

        return true;
    }

    void onHost(CCObject*) {
        if (CollabManager::get()->isHosting) CollabManager::get()->stopHost();
        else CollabManager::get()->host();
    }

    void onJoin(CCObject*) {
        if (CollabManager::get()->isJoined) CollabManager::get()->disconnect();
        else CollabManager::get()->join("456272");
    }
};

class $modify(MyEditor, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelEditorLayer::init(level, p1)) return false;
        return true;
    }
};
