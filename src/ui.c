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