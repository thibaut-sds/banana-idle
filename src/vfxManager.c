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

    for (int i = 0; i < MAX_PARTICLES; i++) {
        fx->particles[i].active = false;
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

    // Update particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (fx->particles[i].active) {
            // Velocity movement
            fx->particles[i].position.x += fx->particles[i].velocity.x * dt;
            fx->particles[i].position.y += fx->particles[i].velocity.y * dt;
            
            // Gravity 
            fx->particles[i].velocity.y += 1200.0f * dt; 
            
            // Rotation on its own axis
            fx->particles[i].rotation += fx->particles[i].rotationSpeed * dt;
            
            fx->particles[i].life -= dt;
            if (fx->particles[i].life <= 0) {
                fx->particles[i].active = false;
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

void SpawnClickParticles(VisualEffects* fx, Vector2 position, int count) {
    int spawned = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!fx->particles[i].active) {
            fx->particles[i].active = true;
            fx->particles[i].position = position;
            
            // explodes upwards and outwards in a cone shape
            fx->particles[i].velocity.x = (float)GetRandomValue(-300, 300);
            fx->particles[i].velocity.y = (float)GetRandomValue(-600, -200);
            
            fx->particles[i].rotation = (float)GetRandomValue(0, 360);
            fx->particles[i].rotationSpeed = (float)GetRandomValue(-200, 200); // Spindle speed
            
            fx->particles[i].scale = (float)GetRandomValue(5, 15) / 10.0f; // size between 0.5x et 1.5x
            
            fx->particles[i].maxLife = (float)GetRandomValue(5, 15) / 10.0f; // lifetime (0.5 to 1.5s)
            fx->particles[i].life = fx->particles[i].maxLife;
            
            spawned++;
            if (spawned >= count) break;
        }
    }
}