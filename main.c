#include <stdio.h>
#include "game.h"
#include "raylib.h"


typedef enum GameScreen {
    SCREEN_TITLE = 0,
    SCREEN_GAMEPLAY,
    SCREEN_SHOP // todo
} GameScreen;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void UpdateScreens(GameState* state, GameScreen* screen, Rectangle bananaRec, Rectangle shopRec, float dt);
void DrawScreens(GameState* state, GameScreen screen, Texture2D bananaTex, Vector2 bananaPos, float bananaScale, Texture2D shopTex, Vector2 shopPos, float shopScale);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Banana Idle");
    SetTargetFPS(60);

    GameState gameState;
    InitGame(&gameState);

    // Load banana texture and set filter for pixel art
    Texture2D bananaTex = LoadTexture("assets/graphics/banana.png");
    SetTextureFilter(bananaTex, TEXTURE_FILTER_POINT);

    Texture2D shopTex = LoadTexture("assets/graphics/shop.png");
    SetTextureFilter(shopTex, TEXTURE_FILTER_POINT);

    GameScreen currentScreen = SCREEN_TITLE;
    
    // Position and size for the banana sprite
    float bananaScale = 8.0f; // 32px * 8 = 256px 
    float scaledWidth = bananaTex.width * bananaScale;
    float scaledHeight = bananaTex.height * bananaScale;
    
    Vector2 bananaPos = {
        (SCREEN_WIDTH / 2.0f) - (scaledWidth / 2.0f),
        (SCREEN_HEIGHT / 2.0f) - (scaledHeight / 2.0f)
    };
    
    // Collision must be based on the scaled size and position of the banana
    Rectangle bananaRec = { bananaPos.x, bananaPos.y, scaledWidth, scaledHeight };

    // SHOP
    float shopScale = 4.0f; 
    float shopScaledWidth = shopTex.width * shopScale;
    float shopScaledHeight = shopTex.height * shopScale;
    
    Vector2 shopPos = {
        SCREEN_WIDTH - shopScaledWidth - 20, 
        20                                   
    };
    Rectangle shopRec = { shopPos.x, shopPos.y, shopScaledWidth, shopScaledHeight };

    // Main game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        
        // Update game logic 
        UpdateScreens(&gameState, &currentScreen, bananaRec, shopRec, dt);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScreens(&gameState, currentScreen, bananaTex, bananaPos, bananaScale, shopTex, shopPos, shopScale);
        EndDrawing();
    }

    UnloadTexture(bananaTex);
    UnloadTexture(shopTex);
    CloseWindow();
    return 0;
}

void UpdateScreens(GameState* state, GameScreen* screen, Rectangle bananaRec, Rectangle shopRec, float dt) {
    switch(*screen) {
        case SCREEN_TITLE:
            if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                *screen = SCREEN_GAMEPLAY;
            }
            break;
            
        case SCREEN_GAMEPLAY:
            UpdateGame(state, dt);
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                
                // Click on the banana
                if (CheckCollisionPointRec(mousePos, bananaRec)) {
                    ClickBanana(state);
                }
                // Click on the shop
                if (CheckCollisionPointRec(mousePos, shopRec)) {
                    *screen = SCREEN_SHOP;
                }
            }
            break;
            
        case SCREEN_SHOP:
            UpdateGame(state, dt);

            // Left the shop with ENTER
            if (IsKeyPressed(KEY_ENTER)) {
                *screen = SCREEN_GAMEPLAY;
            }
            
            // Temporary purchases with keyboard (waiting for clickable buttons)
            if (IsKeyPressed(KEY_C)) {
                BuyClickUpgrade(state);
            }
            if (IsKeyPressed(KEY_I)) {
                BuyIdleUpgrade(state);
            }
            break;
    }
}

void DrawScreens(GameState* state, GameScreen screen, Texture2D bananaTex, Vector2 bananaPos, float bananaScale, Texture2D shopTex, Vector2 shopPos, float shopScale) {
    char scoreBuffer[64];

    switch(screen) {
        case SCREEN_TITLE:
            DrawText("BANANA IDLE", 240, 200, 50, ORANGE);
            DrawText("Click or press ENTER to play !", 180, 300, 20, DARKGRAY);
            break;
            
        case SCREEN_GAMEPLAY:
            // Score
            BigNumberToString(state->bananas, scoreBuffer, sizeof(scoreBuffer));
            DrawText(TextFormat("Bananas: %s", scoreBuffer), 30, 30, 30, BLACK);
            
            //Banana drawing 
            DrawTextureEx(bananaTex, bananaPos, 0.0f, bananaScale, WHITE);
            DrawTextureEx(shopTex, shopPos, 0.0f, shopScale, WHITE);
            break;

        case SCREEN_SHOP:
            DrawText("--- SHOP ---", 280, 50, 40, DARKBLUE);
            DrawText("Press ENTER to return to the game", 180, 550, 20, DARKGRAY);

            // Display score even in the shop
            BigNumberToString(state->bananas, scoreBuffer, sizeof(scoreBuffer));
            DrawText(TextFormat("Bananas: %s", scoreBuffer), 30, 30, 20, BLACK);

            // Display upgrades
            char clickCostStr[32];
            BigNumberToString(state->shop.clickUpgrade.currentCost, clickCostStr, sizeof(clickCostStr));
            DrawText(TextFormat("[C] Double Clic (Lvl %d) - Cost: %s", state->shop.clickUpgrade.level, clickCostStr), 50, 150, 20, BLACK);

            char idleCostStr[32];
            BigNumberToString(state->shop.idleUpgrade.currentCost, idleCostStr, sizeof(idleCostStr));
            DrawText(TextFormat("[I] +1 Banana/sec (Lvl %d) - Cost: %s", state->shop.idleUpgrade.level, idleCostStr), 50, 200, 20, BLACK);
            break;
    }
}