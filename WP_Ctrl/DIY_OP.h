#ifndef _DIY_OP_
#define _DIY_OP_
typedef struct 
{
	int x_target;
	int y_target;
	int last_x_target;
	int last_y_target;
	int state;
}DIy_opmv;

extern DIy_opmv openmv;
extern unsigned char flag1;
extern int A_END;
extern int down_star_flag;
extern int down_expe_flag;
extern int down_end_flag;
extern int xy_star_flag;
extern int xy_expe_flag;
extern int xy_end_flag;
extern int xy_flag,xy_circle_cnt;
extern int xy_counter,A_flag_cnt,A_flag;
extern unsigned char A_cor,Cali_count;
extern int x_c,y_c;
void A_correct(void);
void DWOM_correct(double z_vel,float distance);
void A_down_correct(void);
void move_xy_target(float pos_x_target,float pos_y_target);
#endif
