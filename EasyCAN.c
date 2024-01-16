#include "EasyCAN.h"
#include "can.h"


#define WHEELBOARD_ADDRESS 0x020

WHEELBOARD_BUTTONS buttons;

void sendCAN(uint16_t ADDRESS,uint8_t Tx[8],uint8_t dataLen){

	CAN_TxHeaderTypeDef TxHeader;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId=ADDRESS;
	TxHeader.DLC=dataLen;

	uint32_t Mailbox;

	uint8_t flag=1;
	for(int wait=0;wait<100;wait++){
		if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan2) != 0){
			flag=0;
			break;
		}
	}
	if(flag){
		//generic flag to alert about overflowed Mailbox
	}

	HAL_CAN_AddTxMessage(&hcan2, &TxHeader, Tx, &Mailbox);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){

	CAN_RxHeaderTypeDef rxHeader;
	uint8_t canRx[8] = {0,0,0,0,0,0,0,0};

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, canRx);

	if(rxHeader.StdId==WHEELBOARD_ADDRESS){
			buttons.strategy=canRx[0];
			buttons.motorON=canRx[1];
			buttons.purge=canRx[2];
			buttons.powermode=canRx[3];
			buttons.shortFC=canRx[4];
	}


}
