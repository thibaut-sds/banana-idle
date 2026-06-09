#include "vfxManager.h"
#include <stdio.h>
#include <math.h>

void InitVFX(VisualEffects* fx) {
    fx->targetBananaScale = 8.0f;
    fx->currentBananaScale = 8.0f;
    fx->shopButtonRotation = 0.0f;
    
    for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
        fx->texts[i].active = false;
    }
}

void UpdateVFX(VisualEffects* fx, float dt) {
    // Banana animation returns to normal (Squish)
    float speed = 15.0f; 
    fx->currentBananaScale += (fx->targetBananaScale - fx->currentBananaScale) * speed * dt;

    // Update floating texts
    for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
        if (fx->texts[i].active) {
            fx->texts[i].life -= dt;
            fx->texts[i].position.y -= 50.0f * dt; // Floating to the top
            if (fx->texts[i].life <= 0) {
                fx->texts[i].active = false;
            }
        }
    }
}

void UpdateShopButtonHover(VisualEffects* fx, bool isHovered, float dt) {
    if (isHovered) {
        fx->shopButtonRotation = sinf(GetTime() * 20.0f) * 12.0f;
    } else {
        fx->shopButtonRotation += (0.0f - fx->shopButtonRotation) * 15.0f * dt;
    }
}

void SpawnFloatingText(VisualEffects* fx, Vector2 position, const char* text) {
    for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
        if (!fx->texts[i].active) {
            fx->texts[i].active = true;
            fx->texts[i].life = 1.0f; 
            fx->texts[i].maxLife = 1.0f;
            fx->texts[i].position = position; 
            
            // Safe text copy
            snprintf(fx->texts[i].text, sizeof(fx->texts[i].text), "%s", text);
            break;
        }
    }
}