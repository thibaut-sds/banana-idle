#include <stdio.h>
#include "game.h"
#include "raylib.h"
#include "ui.h"
#include "assetsManager.h"
#include <math.h>
#include "vfxManager.h"

#define MAX_FLOATING_TEXTS 50


typedef enum GameScreen {
    SCREEN_TITLE = 0,
    SCREEN_GAMEPLAY,
    SCREEN_SHOP 
} GameScreen;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void DrawScreens(GameState* state, VisualEffects* fx, GameScreen screen, GameAssets* assets);
void UpdateScreens(GameState* state, VisualEffects* fx, GameScreen* screen, GameAssets* assets, float dt);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Banana Idle");
    SetTargetFPS(60);

    GameState gameState;
    InitGame(&gameState);

    VisualEffects fx;
    InitVFX(&fx);

    // Assets
    GameAssets assets;
    LoadGameAssets(&assets);

    GameScreen currentScreen = SCREEN_TITLE;
    
    
    // Main game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        UpdateLayout(&assets);
        
        // Update game logic 
        UpdateScreens(&gameState, &fx, &currentScreen, &assets, dt);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScreens(&gameState, &fx, currentScreen, &assets);

        // Virtual camera
        Camera2D camera = { 0 };
        camera.zoom = 1.0f;
        // If Fever mode is active during gameplay, the entire screen shifts randomly by a few pixels
        if (gameState.isFever && currentScreen == SCREEN_GAMEPLAY) {
            camera.offset.x = (float)GetRandomValue(-3, 3);
            camera.offset.y = (float)GetRandomValue(-3, 3);
        }

        BeginMode2D(camera);
        DrawScreens(&gameState, &fx, currentScreen, &assets);
        EndMode2D();

        EndDrawing();
    }

    UnloadGameAssets(&assets);
    CloseWindow();
    return 0;
}

void UpdateScreens(GameState* state, VisualEffects* fx, GameScreen* screen, GameAssets* assets, float dt) {
    UpdateVFX(fx, dt);

    // Shop button animation on hover
    Vector2 mousePos = GetMousePosition();

    bool isShopHovered = (*screen == SCREEN_GAMEPLAY) && CheckCollisionPointRec(mousePos, assets->shopRec);
    UpdateShopButtonHover(fx, isShopHovered, dt);

    // screens logic
    switch(*screen) {
        case SCREEN_TITLE:
            if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *screen = SCREEN_GAMEPLAY;
            }
            break;
            
        case SCREEN_GAMEPLAY:
            UpdateGame(state, dt);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Click on the banana
                if (CheckCollisionPointRec(mousePos, assets->bananaRec)) {
                    ClickBanana(state);
                    
                    // juice
                    fx->currentBananaScale = 7.5f; // squish effect
                    
                    // Text : +x bananas
                    BigNumber visualGain = state->clickPower;
                    if (state->isFever) {
                        visualGain = BigNumberMultiply(visualGain, BigNumberFromFloat(2.0f));
                    }

                    char gainStr[32];
                    BigNumberToString(visualGain, gainStr, sizeof(gainStr));
                    char fullText[64];
                    snprintf(fullText, sizeof(fullText), "+%s", gainStr);
                    SpawnFloatingText(fx, mousePos, fullText);
                    SpawnClickParticles(fx, mousePos, GetRandomValue(5, 8));
                }
                // Click on the shop button
                if (isShopHovered) {
                    *screen = SCREEN_SHOP;
                }
            }
            break;
            
        case SCREEN_SHOP:
            UpdateGame(state, dt);

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                *screen = SCREEN_GAMEPLAY;
            }
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();

                if (CheckCollisionPointRec(mousePos, assets->backBtnRec)) {
                    *screen = SCREEN_GAMEPLAY;
                }
                
                if (CheckCollisionPointRec(mousePos, assets->btnClickRec)) {
                    BuyClickUpgrade(state);
                }
                
                if (CheckCollisionPointRec(mousePos, assets->btnIdleRec)) {
                    BuyIdleUpgrade(state);
                }
            }
            break;
    }
}

void DrawScreens(GameState* state, VisualEffects* fx, GameScreen screen, GameAssets* assets) {
    char scoreBuffer[64];

    switch(screen) {
        case SCREEN_TITLE:
            {
                DrawTexture(assets->bgGameplayTex, 0, 0, WHITE);
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, 150 });

                const char* title = "BANANA IDLE";
                int titleFontSize = 60;
                int titleWidth = MeasureText(title, titleFontSize);
                float titleY = GetScreenHeight() * 0.15f;
                float titleX = (GetScreenWidth() - titleWidth) / 2.0f;

                DrawText(title, titleX + 4, titleY + 4, titleFontSize, BLACK);
                DrawText(title, titleX, titleY, titleFontSize, ORANGE);

                float breathScale = 8.0f + (sinf(GetTime() * 2.0f) * 0.5f); 
                float bananaW = assets->bananaTex.width * breathScale;
                float bananaH = assets->bananaTex.height * breathScale;

                Vector2 bananaPos = {
                    (GetScreenWidth() - bananaW) / 2.0f,
                    (GetScreenHeight() - bananaH) / 2.0f
                };
                DrawTextureEx(assets->bananaTex, bananaPos, 0.0f, breathScale, WHITE);

                const char* promptText = "- Click or press ENTER to play -";
                int promptFontSize = 24;
                int promptWidth = MeasureText(promptText, promptFontSize);

                unsigned char alpha = (unsigned char)((sinf(GetTime() * 4.0f) * 0.5f + 0.5f) * 255.0f);
                Color promptColor = (Color){ 220, 220, 220, alpha };
                DrawText(promptText, (GetScreenWidth() - promptWidth) / 2, GetScreenHeight() * 0.85f, promptFontSize, promptColor);
            }
            break;
            
        case SCREEN_GAMEPLAY:
            DrawTexture(assets->bgGameplayTex, 0, 0, WHITE);
            BigNumberToString(state->bananas, scoreBuffer, sizeof(scoreBuffer));
            DrawBananaCounter((Vector2){ GetScreenWidth() * 0.02f, GetScreenHeight() * 0.02f }, scoreBuffer, assets->bananaTex, false);

            // Fever gauge centered at the bottom of the screen
            float gaugeWidth = 400.0f;
            float gaugeHeight = 30.0f;
            Vector2 gaugePos = {
                (GetScreenWidth() - gaugeWidth) / 2.0f,
                GetScreenHeight() - gaugeHeight - 30.0f
            };
            DrawFeverGauge(gaugePos, gaugeWidth, gaugeHeight, state->feverGauge, state->isFever);
            
            // banana's draw with scale effect
            float currentWidth = assets->bananaTex.width * fx->currentBananaScale;
            float currentHeight = assets->bananaTex.height * fx->currentBananaScale;
            Vector2 dynamicPos = {
                assets->baseBananaPos.x + ((assets->bananaTex.width * fx->targetBananaScale) - currentWidth) / 2.0f,
                assets->baseBananaPos.y + ((assets->bananaTex.height * fx->targetBananaScale) - currentHeight) / 2.0f
            };
            
            DrawTextureEx(assets->bananaTex, dynamicPos, 0.0f, fx->currentBananaScale, WHITE);

            float shopScaledWidth = assets->shopTex.width * assets->shopScale;
            float shopScaledHeight = assets->shopTex.height * assets->shopScale;
            Rectangle sourceRec = { 0.0f, 0.0f, (float)assets->shopTex.width, (float)assets->shopTex.height };
            // offset the destination from the center of the image so that the rotation is centered
            Rectangle destRec = {
                assets->shopPos.x + (shopScaledWidth / 2.0f),
                assets->shopPos.y + (shopScaledHeight / 2.0f),
                shopScaledWidth,
                shopScaledHeight
            };
            Vector2 origin = { shopScaledWidth / 2.0f, shopScaledHeight / 2.0f }; // The axis of rotation in the middle
            
            DrawTexturePro(assets->shopTex, sourceRec, destRec, origin, fx->shopButtonRotation, WHITE);

            // Drawing floating texts
            for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
                if (fx->texts[i].active) {
                    unsigned char alpha = (unsigned char)((fx->texts[i].life / fx->texts[i].maxLife) * 255);
                    Color textColor = { 50, 200, 50, alpha }; 
                    DrawText(fx->texts[i].text, (int)fx->texts[i].position.x, (int)fx->texts[i].position.y, 20, textColor);
                }
            }

            for (int i = 0; i < MAX_PARTICLES; i++) {
                if (fx->particles[i].active) {
                    // fadout on the end of the lifetime
                    unsigned char alpha = 255;
                    if (fx->particles[i].life < 0.2f) { // Starts to disappear 0.2s before the end
                        alpha = (unsigned char)((fx->particles[i].life / 0.2f) * 255);
                    }
                    Color particleColor = (Color){ 255, 255, 255, alpha };
                    
                    float pw = assets->bananaTex.width * fx->particles[i].scale;
                    float ph = assets->bananaTex.height * fx->particles[i].scale;
                    Rectangle pSource = { 0.0f, 0.0f, (float)assets->bananaTex.width, (float)assets->bananaTex.height };
                    Rectangle pDest = { fx->particles[i].position.x, fx->particles[i].position.y, pw, ph };
                    Vector2 pOrigin = { pw / 2.0f, ph / 2.0f }; // Centered for rotation
                    
                    DrawTexturePro(assets->bananaTex, pSource, pDest, pOrigin, fx->particles[i].rotation, particleColor);
                }
            }
            break;

        case SCREEN_SHOP:
            DrawTexture(assets->bgGameplayTex, 0, 0, WHITE);
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, 180 });
            DrawTextureEx(assets->backBtnTex, assets->backBtnPos, 0.0f, assets->backBtnScale, WHITE);

            int titleWidth = MeasureText("--- SHOP ---", 40);
            DrawText("--- SHOP ---", (GetScreenWidth() - titleWidth) / 2, GetScreenHeight() * 0.05f, 40, DARKBLUE);

            BigNumberToString(state->bananas, scoreBuffer, sizeof(scoreBuffer));
            DrawBananaCounter((Vector2){ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.15f }, scoreBuffer, assets->bananaTex, true);

            char clickCostStr[32];
            BigNumberToString(state->shop.clickUpgrade.currentCost, clickCostStr, sizeof(clickCostStr));
            const char* clickText = TextFormat("Double Clic (Lvl %d) - Cost: %s", state->shop.clickUpgrade.level, clickCostStr);
            bool canAffordClick = (BigNumberCompare(state->bananas, state->shop.clickUpgrade.currentCost) >= 0);
            
            char idleCostStr[32];
            BigNumberToString(state->shop.idleUpgrade.currentCost, idleCostStr, sizeof(idleCostStr));
            const char* idleText = TextFormat("+1 Banana/sec (Lvl %d) - Cost: %s", state->shop.idleUpgrade.level, idleCostStr);
            bool canAffordIdle = (BigNumberCompare(state->bananas, state->shop.idleUpgrade.currentCost) >= 0);

            DrawShopButton(assets->btnClickRec, clickText, canAffordClick);
            DrawShopButton(assets->btnIdleRec, idleText, canAffordIdle);
            break;
    }
}