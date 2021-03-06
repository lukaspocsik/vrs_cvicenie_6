/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"


void SystemClock_Config(void);

void process_serial_data(char ch);

int LED = 0;
int accept = 0;
int i = 0;
int pom = 0;
int reset = 1;
char data[100];

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART2_UART_Init();

  USART2_RegisterCallback(process_serial_data);

  int tx_data1 = '1';
  int tx_data2 = '0';

  while (1)
  {
	  if(LED == 1)
	  {
		  LL_USART_TransmitData8(USART2, tx_data1);
		  LL_mDelay(5000);
	  }
	  else if (LED == 0)
	  {
		  LL_USART_TransmitData8(USART2, tx_data2);
		  LL_mDelay(5000);
	  }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}


void process_serial_data(char ch)
{

	if (ch != 0)
	{
		if ((ch == '\r') && (reset == 1))
		{
			LL_mDelay(500);
		    i =0;
		    memset(data, 0, sizeof data);
		}
		else
		{
		    data[i++] = ch;
	    }

		ch = 0;
	}

		if(strcmp(data,"led")==0)
		{
			reset = 0;
		}

		if(reset == 0)
		{
			if((data[4] == 'O') && (data[5] == 'F') && (data[6] == 'F'))
			{
				LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
				LED = 0;
				LL_mDelay(50);
				reset = 1;
			}

			if((data[4] == 'O') && (data[5] == 'N'))
			{
		    	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
		    	LED = 1;
				LL_mDelay(50);
		    	reset = 1;
			}

			if((data[4] != 'O') && (data[4] != '\0'))
			{
				reset = 1;
			}

			if(((data[5] != 'N') && (data[5] != 'F')) && (data[5] != '\0'))
			{
				pom = 1;
				LL_mDelay(500);
			    i =0;
			    memset(data, 0, sizeof data);
			}

			if((data[6] != 'F') && (data[6] != '\0'))
			{
				LL_mDelay(500);
			    i =0;
			    memset(data, 0, sizeof data);
			}

		}

		if(strcmp(data,"ledOFF")==0)
		{
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
			LED = 0;
			accept = 0;
			reset = 1;
		}

		if(strcmp(data,"ledON")==0)
		{
	    	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
	    	LED = 1;
	    	accept = 0;
	    	reset = 1;
		}
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
