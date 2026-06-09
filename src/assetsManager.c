#include "assetsManager.h"

void LoadGameAssets(GameAssets* assets) {
    // Loading textures
    assets->bgGameplayTex = LoadTexture("assets/graphics/background_gamescreen.png");
    
    assets->bananaTex = LoadTexture("assets/graphics/banana.png");
    SetTextureFilter(assets->bananaTex, TEXTURE_FILTER_POINT);
    
    assets->shopTex = LoadTexture("assets/graphics/shop.png");
    SetTextureFilter(assets->shopTex, TEXTURE_FILTER_POINT);

    assets->backBtnTex = LoadTexture("assets/graphics/back_button.png");
    SetTextureFilter(assets->backBtnTex, TEXTURE_FILTER_POINT);

    UpdateLayout(assets);
}

void UpdateLayout(GameAssets* assets) {
    // We retrieve the CURRENT size of the window
    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();

    // Banane center (50% X, 50% Y)
    float bananaScale = 8.0f; 
    float scaledW = assets->bananaTex.width * bananaScale;
    float scaledH = assets->bananaTex.height * bananaScale;
    assets->baseBananaPos = (Vector2){ (sw * 0.5f) - (scaledW * 0.5f), (sh * 0.5f) - (scaledH * 0.5f) };
    assets->bananaRec = (Rectangle){ assets->baseBananaPos.x, assets->baseBananaPos.y, scaledW, scaledH };

    // Shop icon (Top Right : 98% X, 2% Y)
    assets->shopScale = 4.0f; 
    float shopW = assets->shopTex.width * assets->shopScale;
    float shopH = assets->shopTex.height * assets->shopScale;
    assets->shopPos = (Vector2){ sw - shopW - (sw * 0.02f), sh * 0.02f };
    assets->shopRec = (Rectangle){ assets->shopPos.x, assets->shopPos.y, shopW, shopH };

    // Back button (Top Left : 2% X, 2% Y)
    assets->backBtnScale = 4.0f;
    float backW = assets->backBtnTex.width * assets->backBtnScale;
    float backH = assets->backBtnTex.height * assets->backBtnScale;
    assets->backBtnPos = (Vector2){ sw * 0.02f, sh * 0.02f };
    assets->backBtnRec = (Rectangle){ assets->backBtnPos.x, assets->backBtnPos.y, backW, backH };

    // Buy button (Width : 60% of screen, Height : 8%)
    float btnW = sw * 0.60f; 
    float btnH = sh * 0.08f; 
    float startX = (sw - btnW) / 2.0f;
    
    // The first button starts at 30% of the height, the second at 42%
    assets->btnClickRec = (Rectangle){ startX, sh * 0.30f, btnW, btnH };
    assets->btnIdleRec = (Rectangle){ startX, sh * 0.42f, btnW, btnH };
}

void UnloadGameAssets(GameAssets* assets) {
    UnloadTexture(assets->bgGameplayTex);
    UnloadTexture(assets->bananaTex);
    UnloadTexture(assets->shopTex);
    UnloadTexture(assets->backBtnTex);
}