#ifndef ASSETS_MANAGER_H
#define ASSETS_MANAGER_H

#include "raylib.h"

/**
* @brief A structure that centralizes all the game's graphic resources and layout.
*/
typedef struct {
    // Textures
    Texture2D bgGameplayTex;
    Texture2D bananaTex;
    Texture2D shopTex;

    // Game screen layout
    Vector2 baseBananaPos;
    Rectangle bananaRec;
    Vector2 shopPos;
    Rectangle shopRec;
    float shopScale;

    // Shop's layouts
    Rectangle btnClickRec;
    Rectangle btnIdleRec;

    // Back button
    Texture2D backBtnTex;
    Vector2 backBtnPos;
    Rectangle backBtnRec;
    float backBtnScale;
} GameAssets;

/**
* @brief Loads all textures into VRAM and calculates the positions.
*/
void LoadGameAssets(GameAssets* assets, int screenWidth, int screenHeight);

/**
* @brief Clears the memory properly upon closing.
*/
void UnloadGameAssets(GameAssets* assets);

#endif // ASSETS_MANAGER_H