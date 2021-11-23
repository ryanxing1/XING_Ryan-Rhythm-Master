#include <stdbool.h> 
#include <stdio.h>   
#include <stdlib.h>  
#include "ece198.h"

void difficultySelection(int *gameDifficulty, int *lives, int *prevDif, int *gameState){

    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));
    int start = HAL_GetTick();
    while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));
    int finish = HAL_GetTick();
    
    int elapsed = finish - start; 

    //Checks if button is held long enough to lock in choice 
    if (elapsed >= 1000){
        
        if ((*gameDifficulty) == 0) {
            SerialPuts("Locked in difficulty as Easy\n");
            (*lives) = 4; 
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4); 
        }
        else if ((*gameDifficulty) == 1) {
            SerialPuts("Locked in difficulty as Medium\n");
            (*lives) = 3; 
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        }
        else if ((*gameDifficulty) == 2) {
            SerialPuts("Locked in difficulty as Hard\n");
            (*lives) = 2; 
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        }
        (*gameState)++; 

        for (int i=0; i<5; i++){
            
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
            HAL_Delay(75);
        }
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)) HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);

    }
    //Cycles through difficulties and lights up respective LED 
    else{

        (*gameDifficulty)++;
        if ((*gameDifficulty) == 3) (*gameDifficulty) = 0; 

        //Easy 
        if ((*gameDifficulty) == 0){
            
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
            
            if ((*prevDif) == 2) HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
            (*prevDif) = 0; 
        }
        //Medium
        else if ((*gameDifficulty) == 1){
            
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
            
            if ((*prevDif) == 0) HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
            (*prevDif) = 1; 
        }
        //Hard 
        else if ((*gameDifficulty) == 2){
            
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
            
            if ((*prevDif) == 1) HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
            (*prevDif) = 2; 
        }
    } 
}

void generatePattern(int gameDifficulty, int pattern[]){

    srand(HAL_GetTick());

    if (gameDifficulty == 0){

        for (int i=0; i<4; i++){
            pattern[i] = random() % 2 + 1;
            
        }
    }
    else if (gameDifficulty == 1){
        
        for (int i=0; i<6; i++){
            pattern[i] = random() % 2 + 1;
        }

    }
    else if (gameDifficulty == 2){
        
        for (int i=0; i<8; i++){
            pattern[i] = random() % 2 + 1;
        }

    }

    char buff[100]; 

    for (int i=0; i<8; i++){
        sprintf(buff, "Pattern: %lu \n", pattern[i]);
        SerialPuts(buff); 
    }

}

void flashPattern(int gameDifficulty, int pattern[]){

    int length = 4 + (gameDifficulty * 2); 

    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    HAL_Delay(1000);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_Delay(1000);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    HAL_Delay(1000);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);


    for (int i=0; i<length; i++){

        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
        HAL_Delay(25);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);

        if (pattern[i] == 1){
            HAL_Delay(800);
        }
        if (pattern[i] == 2){
            HAL_Delay(400);
        }
        if (pattern[i] == 3){
            HAL_Delay(200);
        }

    }

}

void checkPattern(int *curNote, int pattern[], bool *firstPress, bool *failed, uint32_t *start, uint32_t *finish, uint32_t *elapsed) { 

    char buff[100];

    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
    while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));
    HAL_GPIO_TogglePin (GPIOA, GPIO_PIN_10);

    if (*firstPress == false){
        *start = HAL_GetTick();
        *firstPress = true; 
    }
    else{                                               

        *finish = HAL_GetTick();
        
        *elapsed = *finish - *start;
        
        int difference =  abs((int)(*elapsed) - (800 / pattern[*curNote]));
        sprintf(buff, "Elapsed:%u, Expected:%u, Difference:%u\n", *elapsed, (800 / pattern[*curNote]), difference);
        SerialPuts(buff);

        if (abs((int)(*elapsed) - (800 / pattern[*curNote])) > 150){
            *failed = true; 
        } 

        (*curNote)++; 
        
        *start = *finish; 
    }
    
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
    while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);

    *finish = HAL_GetTick();

    *elapsed = *finish - *start; 

    int difference =  abs((int)(*elapsed) - (800 / pattern[*curNote]));
        sprintf(buff, "Elapsed:%u, Expected:%u, Difference:%u\n", *elapsed, (800 / pattern[*curNote]), difference);
        SerialPuts(buff);

    if (abs((int)(*elapsed) - (800 / pattern[*curNote])) > 150){
        *failed = true; 
    }

    (*curNote)++; 


    *start = *finish; 

}

void reset(int *gameState, int *gameDifficulty, int *prevDif, int *curNote, bool *failed, 
          bool *firstPress, uint32_t *start, uint32_t *finish, uint32_t *elapsed){

    *curNote = 0; 
    *failed = false; 
    *firstPress = false;
    *start = 0;
    *finish = 0; 
    *elapsed = 0; 
    
    *gameDifficulty = 0; 
    *prevDif = 0; 
    *gameState = 0; 

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
}