#ifndef UPGRADES_H
#define UPGRADES_H

#include "bignumber.h"

/**
* @brief Generic structure representing an upgrade in the game, with a level, current cost, and cost multiplier for scaling.
*/
typedef struct {
    int level;                  // Actual level of the upgrade
    BigNumber currentCost;      // Price of the next level
    BigNumber costMultiplier;   // Factor for scaling the price (e.g., x1.5 per level)
} Upgrade;

/**
* @brief Initializes an upgrade with a base cost and a multiplier for cost scaling.
* @param upg Pointer to the upgrade to initialize.
* @param baseCost The initial cost of the upgrade at level 0.
* @param multiplier The factor by which the cost increases with each level (e.g., 1.5 for 50% increase per level).
*/
void InitUpgrade(Upgrade* upg, float baseCost, float multiplier);

/**
* @brief Try to purchase an upgrade, increasing its level and cost if successful.
* @param upg The upgrade to purchase.
* @param bananasBalance Pointer to the player's banana balance.
* @return 1 if the purchase was successful, 0 if funds are insufficient.
*/
int TryBuyUpgrade(Upgrade* upg, BigNumber* bananasBalance);

#endif // UPGRADES_H