#ifndef GAME_H
#define GAME_H

#include "bignumber.h"
#include "upgrades.h"

/**
* @brief Structure representing the shop, containing different upgrades available for purchase.
*/
typedef struct {
    Upgrade clickUpgrade; // Double click power
    Upgrade idleUpgrade;  // Add bananas per second
} Shop;

/**
* @brief Structure representing the state of the game, including total bananas, click power, and idle production.
*/
typedef struct {
    BigNumber bananas;           // Total bananas
    BigNumber clickPower;        // How many bananas per click
    BigNumber bananasPerSecond;  // Idle production
    Shop shop;
} GameState;

/**
* @brief Initialize the game state with default values.
* @param state Pointer to the game state to initialize.
*/
void InitGame(GameState* state);

/**
* @brief Update the game economy based on the elapsed time.
* @param state Pointer to the game state to update.
* @param deltaTime Elapsed time since the last frame (in seconds).
*/
void UpdateGame(GameState* state, float deltaTime);

/**
* @brief Function called when the player clicks the big banana.
* @param state Pointer to the game state.
*/
void ClickBanana(GameState* state);

/**
* @brief Attempt to buy the click upgrade, increasing click power if successful.
* @param state Pointer to the game state.
*/
void BuyClickUpgrade(GameState* state);

/**
* @brief Attempt to buy the idle upgrade, increasing idle production if successful.
* @param state Pointer to the game state.
*/
void BuyIdleUpgrade(GameState* state);


#endif // GAME_H