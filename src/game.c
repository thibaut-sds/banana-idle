#include "game.h"

void InitGame(GameState* state) {
    state->bananas = BigNumberFromFloat(0.0f);
    state->clickPower = BigNumberFromFloat(1.0f);
    state->bananasPerSecond = BigNumberFromFloat(0.0f);
    InitUpgrade(&state->shop.clickUpgrade, 10.0f, 2.5f); // Base cost 10 bananas, 150% increase per level
    InitUpgrade(&state->shop.idleUpgrade, 50.0f, 1.15f);  // Base cost 50 bananas, 15% increase per level

    // Fever
    state->feverGauge = 0.0f;
    state->isFever = false;
}

void UpdateGame(GameState* state, float deltaTime) {
    
    // Fever
    if (state->isFever) {
        // In Fever mode, the gauge drains quickly (5 seconds of Fever = -20 per second)
        state->feverGauge -= 40.0f * deltaTime;
        if (state->feverGauge <= 0.0f) {
            state->feverGauge = 0.0f;
            state->isFever = false; // Fever's end 
        }
    } else {
        // In normal mode, if you don't click, the gauge slowly drops back down
        state->feverGauge -= 15.0f * deltaTime;
        if (state->feverGauge < 0.0f) state->feverGauge = 0.0f;
    }


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
    // Filling the gauge
    state->feverGauge += 5.0f; 
    
    if (state->feverGauge >= 100.0f) {
        state->feverGauge = 100.0f;
        
        if (!state->isFever) {
            state->isFever = true; 
        }
    }

    BigNumber gain = state->clickPower;
    if (state->isFever) {
        gain = BigNumberMultiply(gain, BigNumberFromFloat(2.0f)); 
    }

    // Add click power to total bananas
    state->bananas = BigNumberAdd(state->bananas, gain);
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