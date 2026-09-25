#include "game.h"

void InitGame(GameState* state) {
    state->bananas = BigNumberFromFloat(0.0f);
    state->clickPower = BigNumberFromFloat(1.0f);
    state->bananasPerSecond = BigNumberFromFloat(0.0f);
    InitUpgrade(&state->shop.clickUpgrade, 10.0f, 2.5f); // Base cost 10 bananas, 150% increase per level
    InitUpgrade(&state->shop.idleUpgrade, 50.0f, 1.15f);  // Base cost 50 bananas, 15% increase per level
}

void UpdateGame(GameState* state, float deltaTime) {
    if (state->bananasPerSecond.mantissa == 0.0f) {
        return;
    }
    // convert deltaTime to BigNumber
    BigNumber dtBn = BigNumberFromFloat(deltaTime);

    // Calculate the production for this frame
    BigNumber production = BigNumberMultiply(state->bananasPerSecond, dtBn);

    // Add production to total bananas
    state->bananas = BigNumberAdd(state->bananas, production);
}

void ClickBanana(GameState* state) {
    // Add click power to total bananas
    state->bananas = BigNumberAdd(state->bananas, state->clickPower);
}

void BuyClickUpgrade(GameState* state) {
    if (TryBuyUpgrade(&state->shop.clickUpgrade, &state->bananas)) {
        state->clickPower = BigNumberMultiply(state->clickPower, BigNumberFromFloat(2.0f));
    }
}

void BuyIdleUpgrade(GameState* state) {
    if (TryBuyUpgrade(&state->shop.idleUpgrade, &state->bananas)) {
        state->bananasPerSecond = BigNumberAdd(state->bananasPerSecond, BigNumberFromFloat(1.0f));
    }
}