
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "cmsis_os.h"


UART_HandleTypeDef huart2;

#define GREEN GPIO_PIN_9
#define ORANGE GPIO_PIN_10
#define RED GPIO_PIN_11
#define BLUE GPIO_PIN_12

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);


const uint16_t *blue_led = (uint16_t *)BLUE;
const uint16_t *red_led = (uint16_t *)RED;
const uint16_t *green_led = (uint16_t *)GREEN;
const uint16_t *orange_led = (uint16_t *)ORANGE;

int __io_putchar(int ch);
void vLedControllerTask(void *pvParameters);
//void vRedLedControllerTask(void *pvParameters);
//void vGreenLedControllerTask(void *pvParameters);

typedef uint32_t taskProfiler;

taskProfiler blueTaskProfiler, redTaskProfiler, greenTaskProfiler;
//int msg[1] = {'H'};

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  xTaskCreate(vLedControllerTask,
		  	  "Blue LED Controller",
			  100,
			  (void *)blue_led,
			  1,
			  NULL);
  xTaskCreate(vLedControllerTask,
		  	  "Blue LED Controller",
			  100,
			  (void *)red_led,
			  1,
			  NULL);
  xTaskCreate(vLedControllerTask,
		  	  "Blue LED Controller",
			  100,
			  (void *)green_led,
			  1,
			  NULL);
  xTaskCreate(vLedControllerTask,
  		  	  "Orange LED Controller",
  			  100,
			  (void *)orange_led,
  			  1,
  			  NULL);

  vTaskStartScheduler();

  while (1)
  {
	  printf("hello from stm32...\n\r");
//	  HAL_UART_Transmit(&huart2, (uint8_t *)msg,1,0xFFFF);
  }
}

int __io_putchar(int ch){
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch,1,0xFFFF);
	return ch;
}

void vLedControllerTask(void *pvParameters){

	int i;
	while(1){
//		blueTaskProfiler++;
		HAL_GPIO_TogglePin(GPIOA, (uint16_t)pvParameters);
		for(i = 0; i< 60000; i++){};
	};
}

//void vRedLedControllerTask(void *pvParameters){
//
//	while(1){
//		redTaskProfiler++;
//	};
//}
//
//void vGreenLedControllerTask(void *pvParameters){
//
//	while(1){
//		greenTaskProfiler++;
//	};
//}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }


}


static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//enable clock access to ports
  __HAL_RCC_GPIOA_CLK_ENABLE();


  //reset the pin
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
  //configure pins
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  //initialize pins
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
