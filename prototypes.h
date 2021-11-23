#include "ece198.h"

void difficultySelection(int *gameDifficulty, int *lives, int *prevDif, int *gameState);
void generatePattern(int gameDifficulty, int pattern[]);
void flashPattern(int gameDifficulty, int pattern[]);
void checkPattern(int *curNote, int pattern[], bool *firstPress, bool *failed, uint32_t *start, uint32_t *finish, uint32_t *elapsed);
void reset(int *gameState, int *gameDifficulty, int *prevDif, int *curNote, bool *failed, bool *firstPress, uint32_t *start, uint32_t *finish, uint32_t *elapsed);