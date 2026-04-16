#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

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
        // Здесь должна быть логика отправки запроса на сервер (WS)
        // Для примера имитируем ответ
        isHosting = true;
        roomCode = "456272"; 
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
            auto cursor = CCSprite::createWithSpriteFrameName("cursor.png"_spr);
            if (!cursor) cursor = CCSprite::create("GJ_cursor.png"); // Fallback
            cursor->setScale(0.5f);
            cursor->setOpacity(150);
            layer->m_objectLayer->addChild(cursor, 100);
            cursorSprites[id] = cursor;
        }
        cursorSprites[id]->setPosition(pos);
    }
};

// Хук для добавления кнопок в CreatorLayer (меню Create)
class $modify(MyCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        auto menu = this->getChildByID("creator-buttons-menu");

        // Кнопка Хоста
        auto hostBtn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 0.8f, CircleBaseColor::Green),
            this,
            menu_selector(MyCreatorLayer::onHost)
        );
        hostBtn->setID("host-button");

        // Кнопка Присоединения
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
            // Открываем поле ввода кода
            geode::createQuickPopup(
                "Join Collab",
                "Enter 6-digit code:",
                "Cancel", "Join",
                [](auto, bool btn2) {
                    if (btn2) {
                        // Здесь логика получения текста из инпута (упрощено)
                        CollabManager::get()->join("456272");
                    }
                }
            );
        }
    }
};

// Хук для редактора - передача курсора и отрисовка чужих
class $modify(MyEditor, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelEditorLayer::init(level, p1)) return false;
        
        this->schedule(schedule_selector(MyEditor::syncLoop), 0.1f);
        return true;
    }

    void syncLoop(float dt) {
        if (!CollabManager::get()->isHosting && !CollabManager::get()->isJoined) return;

        // Получаем позицию своего курсора
        CCPoint myPos = m_objectLayer->convertTouchToNodeSpace(m_editorUI->m_lastTouchPoint);
        
        // Здесь должен быть код отправки myPos на сервер через WebSocket
        // И получение списка чужих курсоров
    }
};
