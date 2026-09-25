#include "ui.h"

void DrawShopButton(Rectangle bounds, const char* text, bool canAfford) {
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, bounds);

    Color pastelGreen = (Color){ 170, 230, 170, 255 };
    Color pastelGreenHover = (Color){ 140, 210, 140, 255 };
    Color pastelRed = (Color){ 255, 160, 160, 255 };
    Color pastelRedHover = (Color){ 255, 130, 130, 255 };

    Color bgColor;
    if (canAfford) {
        bgColor = isHovered ? pastelGreenHover : pastelGreen;
    } else {
        bgColor = isHovered ? pastelRedHover : pastelRed;
    }

    // Button renderer
    DrawRectangleRec(bounds, bgColor);
    DrawRectangleLinesEx(bounds, 2, DARKGRAY); // Border
    DrawText(text, bounds.x + 10, bounds.y + 10, 20, BLACK);
}

void DrawBananaCounter(Vector2 position, const char* countStr, Texture2D bananaTex, bool isCentered) {    // Size configuration
    float iconScale = 1.5f;
    float iconSize = bananaTex.width * iconScale;
    int fontSize = 22;
    
    // Calculating the dynamic width of the text
    int textWidth = MeasureText(countStr, fontSize);
    
    // Total panel size
    float paddingX = 12.0f;
    float paddingY = 6.0f;
    float spacing = 10.0f;
    
    float boxWidth = (paddingX * 2) + iconSize + spacing + textWidth;
    float boxHeight = iconSize + (paddingY * 2);

    Rectangle box;
    if (isCentered) {
        box = (Rectangle){ position.x - (boxWidth / 2.0f), position.y, boxWidth, boxHeight };
    } else {
        box = (Rectangle){ position.x, position.y, boxWidth, boxHeight };
    }
        
    // Renderer of background
    DrawRectangleRec(box, (Color){ 0, 0, 0, 140 });
    DrawRectangleLinesEx(box, 1, (Color){ 255, 255, 255, 40 }); // border
    
    Vector2 iconPos = { box.x + paddingX, box.y + paddingY };
    DrawTextureEx(bananaTex, iconPos, 0.0f, iconScale, WHITE);
    
    float textX = iconPos.x + iconSize + spacing;
    float textY = position.y + (boxHeight / 2.0f) - (fontSize / 2.0f); // Centré verticalement
    DrawText(countStr, (int)textX, (int)textY, fontSize, WHITE);
}