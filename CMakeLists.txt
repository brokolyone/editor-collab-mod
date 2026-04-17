cmake_minimum_required(VERSION 3.25)
project(EditorCollab VERSION 1.0.0)

find_package(Geode REQUIRED)

add_geode_mod(
	brokolyone.editor_collab
	SOURCES src/main.cpp
)
