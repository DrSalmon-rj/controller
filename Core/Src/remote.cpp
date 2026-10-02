//
// Created by Salmo on 2026/10/2.
//
/**
******************************************************************************
 * @file    remote.cpp/h
 * @brief   Remote control. 遥控器
 ******************************************************************************
 * Copyright (c) 2026 Team JiaoLong-SJTU
 * All rights reserved.
 ******************************************************************************
 */

#include "remote.h"

constexpr uint16_t REMOTE_CONNECT_TIMEOUT = 500u;
// Constructor 构造函数
Remote::Remote(UART_HandleTypeDef *huart): huart_(huart), connect_(REMOTE_CONNECT_TIMEOUT){
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
}

// Start UART(SBUS) receive. 打开UART接收
void Remote::init() {
    rx_len_ = 0;
    HAL_UART_Receive_DMA(huart_, rx_data_, RC_FRAME_LEN);
    // Your code here
}

// Reset RC data. 重置遥控器数据
void Remote::reset() {
    channel_.l_row = 1024;
    channel_.l_col = 1024;
    channel_.r_row = 1024;
    channel_.r_col = 1024;
    channel_.dial_wheel = 1024;
    switch_.l = RCSwitchState_e::DOWN;
    switch_.r = RCSwitchState_e::DOWN;
    // Your code here
}

// Check for uart correspondence. 检查串口是否匹配
void Remote::rxMsgCheck(UART_HandleTypeDef *huart) const {
    // Your code here

}

// Update connect status, restart UART(SBUS) receive.
// 更新连接状态，重新打开UART(SBUS)接收
void Remote::rxMsgCallback(uint8_t* rx_msg){
    connect_.refresh();
    for (int i = 0; i < RC_FRAME_LEN; i++) {
        this->rx_data_[i] = rx_msg[i];
    }

    // 执行解包
    handle();

    // Your code here

}

// Unpack data. 数据解包
void Remote::handle() {
    channel_.r_row =  ((int16_t)rx_data_[0] | ((int16_t)rx_data_[1] << 8)) & 0x07FF;
    channel_.r_col = (((uint16_t)rx_data_[2] >> 3) | ((uint16_t)rx_data_[2] << 5)) & 0x07FF;
    channel_.l_row = (((uint16_t)rx_data_[2] >> 6) | ((uint16_t)rx_data_[3] << 2) | ((uint16_t)rx_data_[4] << 10)) & 0x07FF;
    channel_.l_col = (((uint16_t)rx_data_[4] >> 1) | ((uint16_t)rx_data_[5] << 7)) & 0x07FF;
    switch_.l =  static_cast<RCSwitchState_e>(((rx_data_[5] >> 4) & 0x000C) >> 2);
    switch_.r =  static_cast<RCSwitchState_e>(((rx_data_[5] >> 4) & 0x0003));
    // Your code here

}

