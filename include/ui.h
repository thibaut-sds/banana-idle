#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <stdbool.h>
#include <math.h>

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

/**
 * @brief Draw the fever gauge with a dynamic fill based on the current fever amount.
 * @param position The top left corner of the gauge.
 * @param width The total width of the gauge.
 * @param height The total height of the gauge.
 * @param feverAmount The current amount of fever (0.0 to 100.0).
 * @param isFever true if the player is currently in fever mode.
 */
void DrawFeverGauge(Vector2 position, float width, float height, float feverAmount, bool isFever);

#endif // UI_H