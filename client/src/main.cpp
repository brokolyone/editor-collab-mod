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

    void host() {
        isHosting = true;
        roomCode = std::to_string(100000 + rand() % 900000); 
        FLAlertLayer::create("Host Created!", "Your code is: " + roomCode, "OK")->show();
    }

    void stopHost() {
        isHosting = false;
        FLAlertLayer::create("Info", "Host turned off!", "OK")->show();
    }

    void join(std::string code) {
        isJoined = true;
        roomCode = code;
        FLAlertLayer::create("Success", "Joined session: " + code, "OK")->show();
    }

    void disconnect() {
        isJoined = false;
        FLAlertLayer::create("Info", "Disconnected from session!", "OK")->show();
    }

    void updateCursor(int id, CCPoint pos, LevelEditorLayer* layer) {
        if (!layer || !layer->m_objectLayer) return;
        
        remoteCursors[id] = pos;
        if (cursorSprites.find(id) == cursorSprites.end()) {
            auto cursor = CCSprite::createWithSpriteFrameName("GJ_cursor_001.png");
            if (!cursor) return;
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

        // Кнопка Хоста
        auto hostSprite = ButtonSprite::create("Host", "goldFont.fnt", "GJ_button_01.png", 0.6f);
        auto hostBtn = CCMenuItemSpriteExtra::create(
            hostSprite,
            this,
            menu_selector(MyCreatorLayer::onHost)
        );
        hostBtn->setID("host-button");

        // Кнопка Присоединения
        auto joinSprite = ButtonSprite::create("Join", "goldFont.fnt", "GJ_button_02.png", 0.6f);
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
        if (CollabManager::get()->isJoined) {
            FLAlertLayer::create("Error", "Leave other session first!", "OK")->show();
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
            CollabManager::get()->join("456272");
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
        
        // В 2.2 touch position получается иначе, используем безопасный метод
        CCPoint myPos = m_objectLayer->convertToNodeSpace(CCDirector::sharedDirector()->getWinSize() / 2); // Заглушка центра экрана
        // Отправка на сервер будет добавлена при интеграции WS
    }
};
