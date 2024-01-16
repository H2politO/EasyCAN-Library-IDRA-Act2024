/*
 * EasyCAN.h
 *
 *  Created on: Jan 16, 2024
 *      Author: Antonio Filannino
 */

#ifndef INC_EASYCAN_H_
#define INC_EASYCAN_H_

#include <stdint.h>
#include "main.h"

typedef struct {
	uint8_t strategy;
	uint8_t motorON;
	uint8_t purge;
	uint8_t powermode;
	uint8_t shortFC;
}WHEELBOARD_BUTTONS;

void sendCAN( uint16_t ADDRESS , uint8_t Tx[8] , uint8_t dataLen );

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
#endif /* INC_EASYCAN_H_ */
