#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <map>
#include <string>
#include <vector>

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

    void host() {
        isHosting = true;
        roomCode = std::to_string(100000 + rand() % 900000); 
        Notification::create("Host created! Your code is " + roomCode, NotificationIcon::Success)->show();
    }

    void stopHost() {
        isHosting = false;
        Notification::create("You successfully off Your host!", NotificationIcon::Info)->show();
    }

    void join(std::string code) {
        isJoined = true;
        roomCode = code;
        Notification::create("Joined success!", NotificationIcon::Success)->show();
    }

    void disconnect() {
        isJoined = false;
        Notification::create("You successfully disconnect host!", NotificationIcon::Info)->show();
    }

    void updateCursor(int id, CCPoint pos, LevelEditorLayer* layer) {
        remoteCursors[id] = pos;
        if (cursorSprites.find(id) == cursorSprites.end()) {
            // Используем стандартный спрайт GD, чтобы не искать свои текстуры
            auto cursor = CCSprite::createWithSpriteFrameName("GJ_cursor_001.png");
            if (!cursor) cursor = CCSprite::create("edit_ePointBtn_001.png"); 
            
            cursor->setScale(0.8f);
            cursor->setOpacity(150);
            layer->m_objectLayer->addChild(cursor, 100);
            cursorSprites[id] = cursor;
        }
        cursorSprites[id]->setPosition(pos);
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
        if (CollabManager::get()->isJoined) {
            FLAlertLayer::create("Error", "Disconnect from other host first!", "OK")->show();
            return;
        }
        if (CollabManager::get()->isHosting) {
            CollabManager::get()->stopHost();
        } else {
            CollabManager::get()->host();
        }
    }

    void onJoin(CCObject*) {
        if (CollabManager::get()->isHosting) {
            FLAlertLayer::create("Error", "Stop hosting first!", "OK")->show();
            return;
        }
        
        if (CollabManager::get()->isJoined) {
            CollabManager::get()->disconnect();
        } else {
            auto popup = geode::createQuickPopup(
                "Join Collab",
                "Do you want to join a session? \n(Logic for code input is in dev)",
                "Cancel", "Join",
                [](auto, bool btn2) {
                    if (btn2) {
                        CollabManager::get()->join("456272");
                    }
                }
            );
        }
    }
};

class $modify(MyEditor, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelEditorLayer::init(level, p1)) return false;
        
        this->schedule(schedule_selector(MyEditor::syncLoop), 0.1f);
        return true;
    }

    void syncLoop(float dt) {
        if (!CollabManager::get()->isHosting && !CollabManager::get()->isJoined) return;
        if (!m_editorUI) return;

        // В GD 2.2 координаты тача получаем через m_lastTouchPoint
        CCPoint myPos = m_objectLayer->convertTouchToNodeSpace(m_editorUI->m_lastTouchPoint);
        
        // Логика отправки позиции на сервер будет тут
    }
};
