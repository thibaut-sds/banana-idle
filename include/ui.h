#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <stdbool.h>

/**
* @brief Draw a shop button with dynamic color
* @param bounds Position and size of button.
* @param text Text to display.
* @param canAfford true if the player have enough to buy.
*/
void DrawShopButton(Rectangle bounds, const char* text, bool canAfford);

/**
* @brief Draw banana counter with an icon and a semi-transparent background.
* @param position The top left corner of the counter.
* @param countStr The score text is already formatted
* @param bananaTex The texture of the banana can be reused in small.
*/
void DrawBananaCounter(Vector2 position, const char* countStr, Texture2D bananaTex, bool isCentered);
#endif // UI_H