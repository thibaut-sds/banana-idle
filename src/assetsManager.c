#include "assetsManager.h"

void LoadGameAssets(GameAssets* assets, int screenWidth, int screenHeight) {
    // Loading textures
    assets->bgGameplayTex = LoadTexture("assets/graphics/background_gamescreen.png");
    
    assets->bananaTex = LoadTexture("assets/graphics/banana.png");
    SetTextureFilter(assets->bananaTex, TEXTURE_FILTER_POINT);
    
    assets->shopTex = LoadTexture("assets/graphics/shop.png");
    SetTextureFilter(assets->shopTex, TEXTURE_FILTER_POINT);

    assets->backBtnTex = LoadTexture("assets/graphics/back_button.png");
    SetTextureFilter(assets->backBtnTex, TEXTURE_FILTER_POINT);

    // Layout Calculation (banana)
    float bananaScale = 8.0f; 
    float scaledWidth = assets->bananaTex.width * bananaScale;
    float scaledHeight = assets->bananaTex.height * bananaScale;
    assets->baseBananaPos = (Vector2){ (screenWidth / 2.0f) - (scaledWidth / 2.0f), (screenHeight / 2.0f) - (scaledHeight / 2.0f) };
    assets->bananaRec = (Rectangle){ assets->baseBananaPos.x, assets->baseBananaPos.y, scaledWidth, scaledHeight };

    // Layout Calculation (shop)
    assets->shopScale = 4.0f; 
    float shopScaledWidth = assets->shopTex.width * assets->shopScale;
    float shopScaledHeight = assets->shopTex.height * assets->shopScale;
    assets->shopPos = (Vector2){ screenWidth - shopScaledWidth - 20, 20 };
    assets->shopRec = (Rectangle){ assets->shopPos.x, assets->shopPos.y, shopScaledWidth, shopScaledHeight };

    assets->backBtnScale = 4.0f; 
    float backScaledWidth = assets->backBtnTex.width * assets->backBtnScale;
    float backScaledHeight = assets->backBtnTex.height * assets->backBtnScale;
    assets->backBtnPos = (Vector2){ 20, 20 }; // Top left
    assets->backBtnRec = (Rectangle){ assets->backBtnPos.x, assets->backBtnPos.y, backScaledWidth, backScaledHeight };

    // Layout Calculation (shop buttons)
    assets->btnClickRec = (Rectangle){ 50, 140, 400, 40 };
    assets->btnIdleRec = (Rectangle){ 50, 190, 400, 40 };
}

void UnloadGameAssets(GameAssets* assets) {
    UnloadTexture(assets->bgGameplayTex);
    UnloadTexture(assets->bananaTex);
    UnloadTexture(assets->shopTex);
    UnloadTexture(assets->backBtnTex);
}