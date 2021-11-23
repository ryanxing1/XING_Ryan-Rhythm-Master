
#include <stdbool.h> 
#include <stdio.h>   
#include <stdlib.h>  
#include "ece198.h"

int main(void)
{
    HAL_Init(); 

    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    __HAL_RCC_GPIOB_CLK_ENABLE(); 
    __HAL_RCC_GPIOC_CLK_ENABLE(); 

    InitializePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); 
    InitializePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0); 

    SerialSetup(9600);

   
    int gameDifficulty = 0; 
    int prevDif = 0; 
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

    int gameState = 0; 
    
    bool firstPress = false;  
    uint32_t start = 0; 
    uint32_t finish = 0; 
    uint32_t elapsed = 0; 

    int pattern[8] = {0};
    int curNote = 0; 
    bool failed = false; 
    int lives = 3; 

    char buff[100];

    while (true){

        if (gameState == 0){
            difficultySelection(&gameDifficulty, &lives, &prevDif, &gameState);  
        }
        else{

            generatePattern(gameDifficulty, pattern);
            
            while (true){
                
                if (lives == 0){
                    SerialPuts("Out of lives, game over. Resetting...");

                    for (int i=0; i<6; i++){
            
                        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        HAL_Delay(125);
                    }

                    reset(&gameState, &gameDifficulty, &prevDif, &curNote, &failed, &firstPress, &start, &finish, &elapsed);
                    break; 
                }

                flashPattern(gameDifficulty, pattern); 

                for (int i=0; i<2+gameDifficulty; i++){
                    checkPattern(&curNote, pattern, &firstPress, &failed,  &start,  &finish, &elapsed);
                }

                if (failed){
                
                    SerialPuts("Incorrect pattern, try again.\n"); 
                    curNote = 0; 
                    firstPress = 0; 
                    start = 0; 
                    finish = 0; 
                    failed = false; 
                    lives--; 
                }
                else{
                    SerialPuts("You won! Resetting game...");

                    for (int i=0; i<6; i++){
                        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
                        HAL_Delay(125);
                    }

                    reset(&gameState, &gameDifficulty, &prevDif, &curNote, &failed, &firstPress, &start, &finish, &elapsed);
                    break;
               
                }
            }
        }
    }

    return 0;
      
}

void SysTick_Handler(void)
{
    HAL_IncTick(); 

}
