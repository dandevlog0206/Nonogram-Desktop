#pragma once

#include <stdint.h>

#define GAME_BEGIN namespace game {
#define GAME_END }

#define UI_BEGIN namespace ui {
#define UI_END }

#define MAKE_API_VERSION(variant, major, minor, patch) \
    ((((uint32_t)(variant)) << 29U) | (((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)))

#define INITIAL_WIDTH 1920
#define INITIAL_HEIGHT 1080
#define INITIAL_FPS 60

#define APPLICATION_NAME    "Nonogram Desktop"
#define APPLICATION_TITLE   "Nonogram Desktop"
#define APPLICATION_VERSION MAKE_API_VERSION(1, 0, 0, 0)

#define ENGINE_API_VERSION  MAKE_API_VERSION(1, 3, 0, 0)
#define ENGINE_NAME         "VKDL"
#define ENGINE_VERSION      MAKE_API_VERSION(1, 0, 0, 0)

#define DEFALUT_FONT_PATH "C:/Windows/Fonts/consola.ttf"
#define TITLE_PUZZLES_PATH "puzzles/title.json"