cmake_minimum_required(VERSION 3.25)

project(EditorCollab)

find_package(Geode REQUIRED)

add_geode_mod(
	EditorCollab
	SOURCES src/main.cpp
)

# Добавляем поддержку WebSocket (используем встроенные средства Geode или стороннюю либу)
# Для простоты в этом примере логика сети будет описана через Geode/Cocos2d-x интерфейсы
# или системные вызовы, если потребуются специфичные либы.
