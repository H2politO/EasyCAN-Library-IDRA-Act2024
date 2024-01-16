# EasyCAN Library
The library contains a funtion <code>void sendCAN();</code> to send CAN messages easily and a version of the callback funtion <code>void HAL_CAN_RxFifo0MsgPendingCallback</code> to receive CAN messages.
<br>The callback function contains a filter for the elaboration of the wheelboard's data which are saved in a struct.
