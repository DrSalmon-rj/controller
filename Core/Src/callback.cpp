#include "usart.h"
#include "remote.h"

Remote outcome(&huart3);
extern uint8_t rx_msg[18];

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
    outcome.rxMsgCallback(rx_msg);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_msg, 18);
}
