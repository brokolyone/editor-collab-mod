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
        Notification::create("Host OFF", NotificationIcon::Info)->show();
    }

    void join(std::string code) {
        isJoined = true;
        roomCode = code;
        FLAlertLayer::create("Joined!", "Code: " + code, "OK")->show();
    }

    void disconnect() {
        isJoined = false;
        Notification::create("Disconnected", NotificationIcon::Info)->show();
    }
};

class $modify(MyCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        auto menu = this->getChildByID("creator-buttons-menu");
        if (!menu) return true;

        auto hostBtn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 0.8f, CircleBaseColor::Green),
            this,
            menu_selector(MyCreatorLayer::onHost)
        );
        hostBtn->setID("host-button");

        auto joinBtn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName("GJ_shareBtn_001.png", 0.8f, CircleBaseColor::Blue),
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
        this->schedule(schedule_selector(MyEditor::syncLoop), 0.1f);
        return true;
    }

    void syncLoop(float dt) {
        // Logic for sync here
    }
};
