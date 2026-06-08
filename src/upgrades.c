#include "upgrades.h"

void InitUpgrade(Upgrade* upg, float baseCost, float multiplier) {
    upg->level = 0;
    upg->currentCost = BigNumberFromFloat(baseCost);
    upg->costMultiplier = BigNumberFromFloat(multiplier);
}

int TryBuyUpgrade(Upgrade* upg, BigNumber* bananasBalance) {
    // Check if the player has enough bananas to buy the upgrade
    if (BigNumberCompare(*bananasBalance, upg->currentCost) >= 0) {
        // Deduct the cost from the player's balance
        *bananasBalance = BigNumberSubtract(*bananasBalance, upg->currentCost);
        
        // Increase the upgrade level
        upg->level++;
        
        // Update the cost for the next level (cost = baseCost * multiplier^level)
        upg->currentCost = BigNumberMultiply(upg->currentCost, upg->costMultiplier);
        
        return 1; // Purchase successful
    }
    return 0; // Not enough bananas
}