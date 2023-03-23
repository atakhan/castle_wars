#include "Level.h"

Level::Level() : Level(0) {}

Level::Level(int level) : current(level) {}

int Level::getCurrentLevel() {
    return current;
}

int Level::getNextLevelCost() {
    return map[current+1][LEVEL_COST];
}

int Level::getCurrentMax() {
    return map[current][LEVEL_MAX_UNIT];
}

int Level::getCurrentRegen() {
    return map[current][LEVEL_REGEN];
}

int Level::getCurrentAttackFrequency() {
    return map[current][LEVEL_ATTACK_FREQUENCY];
}
