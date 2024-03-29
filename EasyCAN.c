#include "EasyCAN.h"
#include "can.h"
#include "pwm.h"
#include "interrupt.h"
#include "string.h"


#define WHEELBOARD_ADDRESS 0x020
#define TUNING_ADDRESS 0x040

WHEELBOARD_BUTTONS buttons;
TUNINGPARAM tuning;
uint32_t lastWheelBoardTime;
uint8_t canTx[8];
CAN_FilterTypeDef canFil;

void ConfigCAN(){
	canFil.FilterBank = 10;
	canFil.FilterMode = CAN_FILTERMODE_IDMASK;
	canFil.FilterFIFOAssignment = CAN_RX_FIFO0;
	canFil.FilterIdHigh = 0;
	canFil.FilterIdLow = 0;
	canFil.FilterMaskIdHigh = 0;
	canFil.FilterMaskIdLow = 0;
	canFil.FilterScale = CAN_FILTERSCALE_32BIT;
	canFil.FilterActivation = ENABLE;
	canFil.SlaveStartFilterBank = 0;
}

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
		flag=1;
		//generic flag to alert about overflowed Mailbox
	}

	HAL_CAN_AddTxMessage(&hcan2, &TxHeader, Tx, &Mailbox);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){

	CAN_RxHeaderTypeDef rxHeader;
	uint8_t canRx[8] = {0,0,0,0,0,0,0,0};

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, canRx);

	switch(rxHeader.StdId){
	case WHEELBOARD_ADDRESS:
		buttons.strategy=canRx[0];
		buttons.motorON=canRx[1];
		buttons.purge=canRx[2];
		buttons.powermode=canRx[3];
		buttons.shortFC=canRx[4];
		lastWheelBoardTime=HAL_GetTick();
		break;
	case TUNING_ADDRESS:
		tuning.p=canRx[0];
		tuning.i=canRx[1];
		tuning.d=canRx[2];
		break;
	}
}

void Transmit(){
	memcpy(canTx,&motorAmps,4);
    sendCAN(0x030,canTx,4);
    duty=pwmConfMotor.duty;
    memcpy(canTx,&duty,4);
    sendCAN(0x031,canTx,4);
    canSendFlag=0;
}

void resetWheelBoard(){
	  buttons.motorON=0;
	  buttons.strategy=0;
	  buttons.purge=0;
	  buttons.powermode=0;
	  buttons.shortFC=0;
}
