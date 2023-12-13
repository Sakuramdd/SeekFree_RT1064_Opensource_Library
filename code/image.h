#ifndef _image_h
#define _image_h

#define H   60
#define W   94

extern uint8 GetOSTU ();
extern void Binarization(uint8_t Bin_Image[121][94],uint8 yuzhi);
extern void image_control();
extern void maopao_paixv(int arr[], int sz);
extern void zhongzhi_lvbo(uint8 edge[120][188]);
extern void sxt_yasuo(int yasoubeishu_line,int yasoubeishu_list);
extern void Bin_Image_Filter (void);
extern void img_conversion(uint8_t Bin_Image[121][94],uint8 val);
extern void find_borders();
extern void twoBJ();
extern void Image_acquisition(void);
#endif
