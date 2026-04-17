cmake_minimum_required(VERSION 3.25)
project(EditorCollabMod VERSION 1.1.1)

find_package(Geode REQUIRED)

add_geode_mod(
	brokolyone.editor_collab_mod
	SOURCES src/main.cpp
)
