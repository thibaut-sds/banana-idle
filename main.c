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
    LoadGameAssets(&assets, SCREEN_WIDTH, SCREEN_HEIGHT);

    GameScreen currentScreen = SCREEN_TITLE;
    
    
    // Main game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Update game logic 
        UpdateScreens(&gameState, &fx, &currentScreen, &assets, dt);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScreens(&gameState, &fx, currentScreen, &assets);
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
                    char gainStr[32];
                    BigNumberToString(state->clickPower, gainStr, sizeof(gainStr));
                    char fullText[64];
                    snprintf(fullText, sizeof(fullText), "+%s", gainStr);
                    SpawnFloatingText(fx, mousePos, fullText);
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
            DrawText("BANANA IDLE", 240, 200, 50, ORANGE);
            DrawText("Click or press ENTER to play !", 180, 300, 20, DARKGRAY);
            break;
            
        case SCREEN_GAMEPLAY:
            DrawTexture(assets->bgGameplayTex, 0, 0, WHITE);
            BigNumberToString(state->bananas, scoreBuffer, sizeof(scoreBuffer));
            DrawBananaCounter((Vector2){ 20, 20 }, scoreBuffer, assets->bananaTex);
            
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
            break;

        case SCREEN_SHOP:
            DrawText("--- SHOP ---", 280, 50, 40, DARKBLUE);
            DrawText("Press ENTER to return to the game", 180, 550, 20, DARKGRAY);

            BigNumberToString(state->bananas, scoreBuffer, sizeof(scoreBuffer));
            DrawBananaCounter((Vector2){ 160, 20 }, scoreBuffer, assets->bananaTex);

            DrawTextureEx(assets->backBtnTex, assets->backBtnPos, 0.0f, assets->backBtnScale, WHITE);

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