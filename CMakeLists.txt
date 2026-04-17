cmake_minimum_required(VERSION 3.25)

project(EditorCollab)

find_package(Geode REQUIRED)

add_geode_mod(
	brokolyone.editor_collab
	SOURCES src/main.cpp
)
