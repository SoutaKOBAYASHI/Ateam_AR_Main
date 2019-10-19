/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
			
#include "f405boardLib_conf.hpp"
#include "uart.hpp"
#include "control_area_network.hpp"
#include "sequence.hpp"
#include "autorunning_params.hpp"

#include <string>

float x = 0.0f, y = 0.0f, theta = 0.0f, run_vec_x = 0.0f, run_vec_y = 0.0f, run_vec_theta = 0.0f, run_vec_size = 0.0f;

void clockInit();

int main(void)
{
	clockInit();

	SysTick_Interrupt::init(100, 3);

	UART_Initialize<uartName::uart1, 115200> gyro_uart;
	UART_Initialize<uartName::uart2, 38400> pc_uart_port;
	CAN_Initialize<0x00> can;

	IO_sigPins<ioName::sig7, ioState::input, pinPullDirection::up> emergency;
	bool emergency_state = !emergency.readNowState();

	Sequence sequence(emergency_state, MECHA_CONTROLER_ADDRESS, AUTO_RUNNING_PARAMS, gyro_uart.uart_interface, pc_uart_port.uart_interface, can.can_interface);
	while(true)
	{
		emergency_state = !emergency.readNowState();
	}
}

void clockInit()
{
    /*
     * PLLM = 16, PLLN = 168, PLLP = 2, PLLQ = 8
     * HSI clock is 16MHz. This is constant value.
     * Expression of System clock is    : HSI x PLLN / (PLLM x PLLP) = 84MHz
     * Expression of HCLK clock is        : SystemClock / HCLK_prescaler = 84MHz
     * Expression of PCLK1 clock is        : HCLK / PCLK1_prescaler = 42MHz
     * Expression of PCLK2 clock is        : HCLK / PCLK2_prescaler = 84MHz
     */
    RCC_DeInit();
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                    // HCLK prescaler is 1.
    RCC_PCLK1Config(RCC_HCLK_Div2);                        // PCLK1 prescaler is 2.
    RCC_PCLK2Config(RCC_HCLK_Div1);                        // PCLK2 prescaler is 1.
    RCC_PLLCmd(DISABLE);
    RCC_PLLConfig(RCC_PLLSource_HSI, 16, 168, 2, 8);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08);
}
