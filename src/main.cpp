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

    // Генерируем 6-значный код
    std::string generateCode() {
        return std::to_string(100000 + rand() % 900000);
    }

    void onHost() {
        if (isJoined) {
            FLAlertLayer::create("Error", "Stop being a guest first!", "OK")->show();
            return;
        }

        if (isHosting) {
            isHosting = false;
            roomCode = "";
            Notification::create("You successfully off Your host!", NotificationIcon::Info)->show();
        } else {
            isHosting = true;
            roomCode = generateCode();
            Notification::create("Host created! Your code is " + roomCode, NotificationIcon::Success)->show();
        }
    }

    void onJoin(std::string code) {
        if (isHosting) {
            FLAlertLayer::create("Error", "Stop hosting first!", "OK")->show();
            return;
        }

        if (isJoined) {
            isJoined = false;
            roomCode = "";
            Notification::create("You successfully disconnect host!", NotificationIcon::Info)->show();
        } else {
            isJoined = true;
            roomCode = code;
            Notification::create("Joined success! Code: " + roomCode, NotificationIcon::Success)->show();
        }
    }

    void updateRemoteCursor(int id, CCPoint pos, LevelEditorLayer* layer) {
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

// Хук в меню Create
class $modify(MyCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        auto menu = this->getChildByID("creator-buttons-menu");
        if (!menu) return true;

        // Кнопка Host (Зеленая с Плюсиком)
        auto hostBtn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 0.8f, CircleBaseColor::Green),
            this,
            menu_selector(MyCreatorLayer::onHostBtn)
        );
        hostBtn->setID("collab-host-button");

        // Кнопка Join (Синяя с Шейром)
        auto joinBtn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName("GJ_shareBtn_001.png", 0.8f, CircleBaseColor::Blue),
            this,
            menu_selector(MyCreatorLayer::onJoinBtn)
        );
        joinBtn->setID("collab-join-button");

        menu->addChild(hostBtn);
        menu->addChild(joinBtn);
        menu->updateLayout();

        return true;
    }

    void onHostBtn(CCObject*) {
        CollabManager::get()->onHost();
    }

    void onJoinBtn(CCObject*) {
        if (CollabManager::get()->isJoined) {
            CollabManager::get()->onJoin("");
            return;
        }

        // Поле ввода кода через Geode Popup
        geode::createQuickPopup(
            "Join Collab",
            "Enter 6-digit room code:",
            "Cancel", "Join",
            [this](auto, bool btn2) {
                if (btn2) {
                    // Пока захардкодим код, так как кастомные инпуты требуют больше кода
                    CollabManager::get()->onJoin("456272");
                }
            }
        );
    }
};

// Хук в редактор для синхронизации курсора
class $modify(MyEditor, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelEditorLayer::init(level, p1)) return false;
        
        // Запускаем цикл синхронизации каждые 0.1 сек
        this->schedule(schedule_selector(MyEditor::collabSync), 0.1f);
        return true;
    }

    void collabSync(float dt) {
        auto manager = CollabManager::get();
        if (!manager->isHosting && !manager->isJoined) return;

        // В GD 2.206+ m_editorUI всё еще m_editorUI в Geode
        if (m_editorUI) {
            // Безопасный способ получить позицию тача
            CCPoint lastTouch = m_editorUI->m_lastTouchPoint;
            CCPoint nodePos = m_objectLayer->convertToNodeSpace(lastTouch);
            
            // Здесь будет отправка nodePos на сервер (Websocket)
        }
    }
};
