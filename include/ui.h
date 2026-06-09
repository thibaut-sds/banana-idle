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

#endif // UI_H