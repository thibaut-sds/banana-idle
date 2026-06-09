#ifndef VFX_MANAGER_H
#define VFX_MANAGER_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_FLOATING_TEXTS 50

typedef struct {
    Vector2 position;
    float life;
    float maxLife;
    char text[64];
    bool active;
} FloatingText;

typedef struct {
    float currentBananaScale;
    float targetBananaScale;
    float shopButtonRotation;
    FloatingText texts[MAX_FLOATING_TEXTS];
} VisualEffects;

/**
* @brief Initializes the visual effects manager.
*/
void InitVFX(VisualEffects* fx);

/**
* @brief Updates all continuous effects (animations, particles, etc.) based on time.
*/
void UpdateVFX(VisualEffects* fx, float dt);

/**
* @brief Triggers the wiggle effect of the shop button.
* @param isHovered true if the mouse is over the button, false otherwise.
*/
void UpdateShopButtonHover(VisualEffects* fx, bool isHovered, float dt);

/**
* @brief Displays floating text at a given position.
*/
void SpawnFloatingText(VisualEffects* fx, Vector2 position, const char* text);

#endif // VFX_MANAGER_H