#ifndef _DEVELOPER_MODE_
#define _DEVELOPER_MODE_
#define SDK_TransForm_Enable 1
extern int8_t SDK1_Mode_Setup,SDK2_Mode_Setup;
extern uint8_t SDK_finish_flag;
extern float x_ta,y_ta;
extern int8_t SDK_TransForm_Flag;
extern int16_t Delay_Transform_Time;
void Auto_Flight_Ctrl(uint8_t mode);


#endif



