#include "zf_common_headfile.h"
#include "My_headfile.h"
#include "math.h"
#include "image.h"
uint8_t tmImage[H][W];//图像存放数组
uint8_t tmImage_ostu[H][W];//二值化存放数组
uint8_t zhongzhi[H][W];
uint8_t fb_image[H][W];
uint8_t tv_image[H][W];
uint8 Threshold;



void image_control()
{
    /*******滤波************/
    zhongzhi_lvbo(mt9v03x_image);
    /*******压缩图像*********/
    //    sxt_yasuo(1,2);//压缩图像
    Image_acquisition();//压缩图像
    GetOSTU () ;
    Binarization(tmImage,Threshold);
    Bin_Image_Filter();
//    img_conversion(tmImage,Threshold);
//    find_borders();
//    twoBJ();
    tft180_show_gray_image(0, 0, tmImage[0], 94, 62, 94, 62, Threshold);
}





/*---------------------------------------------------------------
 【函    数】maopao_paixv
 【功    能】冒泡
 【参    数】无
 【返 回 值】无
 【注意事项】将数据冒泡排序
 【来源】来威宇
 ----------------------------------------------------------------*/
void maopao_paixv(int arr[], int sz)
{

    int i;
    for (i = 1; i <= sz - 1; i++)//趟数
    {
        int j = 0;
        for (j = 1; j <= sz - i; j++)//交换的个数
        {
            if (arr[j-1] > arr[j])//交换两个元素
            {
                int tmp = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}
/*---------------------------------------------------------------
 【函    数】zhongzhi_lvbo
 【功    能】中值滤波
 【参    数】无
 【返 回 值】无
 【注意事项】将冒泡后的数据取中值
 【来源】来威宇
 ----------------------------------------------------------------*/
void zhongzhi_lvbo(uint8 edge[120][188])//中值滤波
{
    int paixv_shuzu[9];
    int weishu=0;
    for(int y;y<115;y++)
    {
        for(int a=0;a<89;a++)//3*3卷积核
        {
            for(int g=0;g<=2;g++)//把卷积核中的数都放到一个数组里
            {
                for(int h=0;h<=2;h++)
                {
                    paixv_shuzu[weishu]=mt9v03x_image[g][h];
                    weishu++;
                }
            }//放入完成
            weishu=0;//清0
            //排序
            maopao_paixv(paixv_shuzu,9);
            //取中值
            zhongzhi[y+1][a+1]=paixv_shuzu[4];
        }

    }
}
/*---------------------------------------------------------------
 【函    数】sxt_yasuo
 【功    能】图像 抽取压缩
 【参    数】无
 【返 回 值】无
 【注意事项】行压缩了，列未压缩
 【来源】来威宇
 ----------------------------------------------------------------*/
// 图像 抽取压缩188X120 to 94X60
void sxt_yasuo(int yasoubeishu_line,int yasoubeishu_list)
{
    short i = 0, j = 0, row = 0, line = 0;

    for (i = 0; i < MT9V03X_H; i += yasoubeishu_line)          //摄像头高 120 / yasoubeishu_line  = 60，
    {
        for (j = 0; j < MT9V03X_W; j += yasoubeishu_list)     //摄像头宽188 / yasoubeishu_list  = 94，
        {
            tmImage[row][line] = mt9v03x_image[i][j];
            line++;//每次行数+1
        }
        line = 0;//行数清0，列数加一，为从新进行下一行准备
        row++;
    }
}
/*---------------------------------------------------------------
 【函    数】Image_conversion
 【功    能】图像转换 抽取压缩
 【参    数】无
 【返 回 值】无
 【注意事项】本函数将原图像188*120转化为94*60;
 【来源】荣哥
 ----------------------------------------------------------------*/
void Image_acquisition(void)
{
   for (int i = 0; i < H; i++)    //60行处理图像
   {
       for (int j = 0; j < W; j++)//188列图像每两行处理为一行
       {
           tmImage[i][j] = mt9v03x_image[i*2][j*2]; //每两行两列算一行
       }
   }
}
// An highlighted block
/*************************************************************************
 *  函数名称：GetOSTU (void)
 *  功能说明：大津法求阈值大小
 *  参数说明：tmImage ： 图像数据
 *  函数返回：Threshold
 *  修改时间：2011年10月28日
 *  备    注：  GetOSTU(Image_Use);//大津法阈值
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代 1
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像
        的比例w0，        并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背
        景像素)  * 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7) 将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
*************************************************************************/
uint8 GetOSTU(void)
{
        int16 i, j;
        uint32 Amount = 0;
        uint32 PixelBack = 0;
        uint32 PixelIntegralBack = 0;
        uint32 PixelIntegral = 0;
        int32 PixelIntegralFore = 0;
        int32 PixelFore = 0;
        double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
        uint16 MinValue, MaxValue;
        #define GrayScale 256                   
        //定义256个灰度级
        uint16 HistoGram[GrayScale] = {0}; //定义灰度直方图，并初始化

        int16 Mincount = 0, Maxcount = 0;

        // for (j = 0; j < 256; j++)
        // {
        //     HistoGram[j] = 0; //初始化灰度直方图
        // }
        for (j = 0; j < 120; j++)
        {
            for (i = 0; i < 93 ; i++)
            {
                HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
            }
        }

        //获取最小灰度的值

        for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] <= 5; MinValue++)
        {
            Mincount += HistoGram[MinValue];
            HistoGram[MinValue] = 0;
        }
        //获取最大灰度的值
        for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] <= 15; MaxValue--)
        {
            Maxcount += HistoGram[MaxValue];
            HistoGram[MaxValue] = 0;
        }
        //滤除反光点
//        for (j = 0; j < 61; j++)
        //        {
//            for (i = 1; i < 93; i++)
            //            {
//                if (tmImage[j][i] > MaxValue - 8)
                //                {
//                    tmImage[j][i] = MaxValue - 8;
                //                }
//            }
//        }
        HistoGram[MaxValue] += Maxcount;
        HistoGram[MinValue] += Mincount;
        if (MaxValue == MinValue)
        {
            return MaxValue; // 图像中只有一个颜色
        }

        if (MinValue + 1 == MaxValue)
        {
            return MinValue; // 图像中只有二个颜色
        }


        for (j = MinValue; j <= MaxValue; j++)
        {
            Amount += HistoGram[j]; // 像素总数
        }

        PixelIntegral = 0;
        for (j = MinValue; j <= MaxValue; j++)
        {
            PixelIntegral += HistoGram[j] * j; //灰度值总数
        }

        SigmaB = -1;

        for (j = MinValue; j < MaxValue; j++)
        {
            PixelBack = PixelBack + HistoGram[j];
            //前景像素点数
            PixelFore = Amount - PixelBack;
            //背景像素点数
            OmegaBack = (double)PixelBack / Amount;
            //前景像素百分比
            OmegaFore = (double)PixelFore / Amount;
            //背景像素百分比
            PixelIntegralBack += HistoGram[j] * j;
            //前景灰度值
            PixelIntegralFore = PixelIntegral - PixelIntegralBack;
            //背景灰度值
            MicroBack = (double)PixelIntegralBack / PixelBack;
            //前景灰度百分比
            MicroFore = (double)PixelIntegralFore / PixelFore;
            //背景灰度百分比
            Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack- MicroFore); //计算类间方差
            if (Sigma > SigmaB)
            //遍历最大的类间方差 g //找出最大类间方差以及对应的阈值
            {
                SigmaB = Sigma;

                Threshold = (uint8)j;
            }
        }
        return Threshold; //返回最佳阈值;

}
//**********************************二值化数组***************************//
/*---------------------------------------------------------------
 【函    数】Binarization(uint8_t Bin_Image[121][94],uint8 yuzhi)
 【功    能】将图像二值化
 【参    数】uint8_t Bin_Image[121][94]       处理后的数组   
            uint8 yuzhi       大津法求出的阈值
 【返 回 值】无
 【注意事项】无
 【来源】来威宇
 ----------------------------------------------------------------*/
void Binarization(uint8_t Bin_Image[60][94],uint8 yuzhi)
{
    int i,j;
    for(i=0;i< 59;i++)
    {
        for(j=0;j<93;j++)
        {
            if(Bin_Image[i][j]>yuzhi)
            {
                tmImage_ostu[i][j]=0;   //白点
            }
            else
            {
                tmImage_ostu[i][j]=1;   //黑点
            }
        }
    }
}
// An highlighted block
/*---------------------------------------------------------------
 【函    数】Bin_Image_Filter
 【功    能】过滤噪点
 【参    数】无
 【返 回 值】无
 【注意事项】腐蚀图像，黑点判断一次
 【来源】小向是个der
 ----------------------------------------------------------------*/
void Bin_Image_Filter (void)
{
    uint16 nr; //行
    uint16 nc; //列

    for (nr = 1; nr < 120 - 1; nr++)
    {
        for (nc = 1; nc < 93 - 1; nc = nc + 1)
        {
            if ((tmImage[nr][nc] == 0)
                    && (tmImage[nr - 1][nc] + tmImage[nr + 1][nc] + tmImage[nr][nc + 1] + tmImage[nr][nc - 1] > 2))
            {
                tmImage[nr][nc] = 1;
            }
            else if ((tmImage[nr][nc] == 1)
                    && (tmImage[nr - 1][nc] + tmImage[nr + 1][nc] + tmImage[nr][nc + 1] + tmImage[nr][nc - 1] < 2))
            {
                tmImage[nr][nc] = 0;
            }
        }
    }
}
/*---------------------------------------------------------------
 【函    数】img_conversion
 【功    能】二值化图像
 【参    数】uint8_t Bin_Image[60][94]   处理后的数组   
            uint8 val       大津法求出的阈值
 【返 回 值】无
 【注意事项】将图像进行初步的二值化，黑点并不确判
 【来源】ys.journey
 ----------------------------------------------------------------*/
void img_conversion(uint8_t Bin_Image[60][94],uint8 val)
{
    uint8 line=0,col=0;
    for(line=0;line<61;line++)
    {
       for(col=0;col<95;col++)
       {
            if(Bin_Image[line][col]>val)  
                tmImage_ostu[line][col]=250;  //！！！！敲黑板 划重点 250 不是255
            else tmImage_ostu[line][col]=0;
       }
    }
}
/*---------------------------------------------------------------
 【函    数】find_borders
 【功    能】过滤噪点
 【参    数】无
 【返 回 值】无
 【注意事项】扫描黑点，进行第一次判断
 【来源】ys.journey
 ----------------------------------------------------------------*/
void find_borders()
{
    int16 i,j,temp_centere; //临时中点
    /**********先初始化边界数组********************/
        uint8_t fb_image[H][W]={0};
    /**********找59行的中心检测点********************/
    temp_centere=W/2;	     //   上一次中点位置 
    for(int i=0;i<=30;i++)  
    {
         if(tv_image[59][temp_centere+i]==250 || tv_image[59][temp_centere+i]==255)
         {
            temp_centere=temp_centere+i;
            break;
         }
         if(tv_image[59][temp_centere-i]==250 || tv_image[59][temp_centere-i]==255)
         { 
            temp_centere=temp_centere-i;
            break;
         }
    }
    /**********最后一行向左扫描********************/
    for(i=temp_centere;i>=0;i--)   
    {
        if(tv_image[59][i] !=0 )
        {
           tv_image[59][i] =255;
        }
        else break;
    }
    if(i==-1)  //for循环中1-2循环 到达0时 值已经为-1了
    {
       fb_image[59][0] = 0;
    }
    else
    {
       fb_image[59][0] = i+1; //存储数组  假设左边界是20
    }
    /**********最后一行向右扫描********************/
    for(i=temp_centere; i<W; i++)        
    {
        if(tv_image[59][i] !=0 )
        {
          tv_image[59][i] =255;
        }
        else break;
    }
    if(i == W)
    {
        fb_image[59][2] = 93;
    }
    else
    {
        fb_image[59][2] = i-1; //存储数组
    }
     
    /****************自下往上，扫描一次 筛选跳变点*****************/
    for(i=59;i>=0;i--)    //根据119行的255遍历找本行的250或者255 及边界
    {
        for(j=0;j<W;j++)
        {
          if( tv_image[i+1][j] == 255 )     //根据上一行的赛道来确定本行的赛道
          {
            if( tv_image[i][j]== 250 || tv_image[i][j] == 255 )
            {
              tv_image[i][j] = 255;
            }
            if( j-1 >=0 &&( tv_image[i][j-1] == 250 || tv_image[i][j-1] == 255 ))
            {
              tv_image[i][j-1] =255;
            }
            if( j+1<=W-1 &&(tv_image[i][j+1] == 250 || tv_image[i][j+1] == 255))
            {
              tv_image[i][j+1]= 255;
            }
          }
        }
         /**********向左扫描，寻找第一个255点********************/
        for(j=0;j<=W-1;j++)        
        {
            if(tv_image[i][j] == 255)
            {
               break;
            }
        }
        if(j==W)  //本行无赛道，跳出
        {
          break;
        }
        for(;j>=0;j--)      //根据上面找到的255,向回找255 或者 250 核心亮点 for循环还能这样玩
        {
            if(tv_image[i][j] == 250 || tv_image[i][j] == 255)
            {
              tv_image[i][j] =255;
            }
            else
            {
              fb_image[i][0] = j+1;
              break;
            }
        }
        if(j==-1)   //本行无赛道，跳出
        {
          fb_image[i][0] = 0;
        } 
        /**********向左扫描，寻找第一个255点********************/
        for(j=W-1;j>=0; j--)        //同上
        {
          if(tv_image[i][j] == 255)
          {
            break;
          }
        }
        if(j==-1)
        {
          break;
        }
        for(;j<=W-1;j++)     //同上
        {
          if(tv_image[i][j] == 250 || tv_image[i][j] == 255)
          {
            tv_image[i][j] = 255;
          }
          else
          {
            fb_image[i][2] = j-1;
            break;
          }
        }
        if(j==W)
        {
          fb_image[i][2] = W-1;
        }
    }
    
    
    /*******************************二次扫描是为了处理特殊情况****************************************************/ 
#if 1
    if(i >= 0 )  //代表图像未扫描完
    {
        temp_centere=W/2;	     //   上一次中点位置
        for(int j=0;j<=H/2;j++)
        {
          if(tv_image[i][temp_centere+j]==250 || tv_image[i][temp_centere+j]==255)
          {
            temp_centere=temp_centere+j;
            break;
          }
          if(tv_image[i][temp_centere-j]==250 || tv_image[i][temp_centere-j]==255)
          {
            temp_centere=temp_centere-j;
            break;
          }
        }
        if( temp_centere != W/2)        //不相等说明找到了该行的赛道
        {
           //自下往上，扫描一次
            for(i=temp_centere;i>=0;i--)    //根据159行的255遍历找本行的250或者255 及边界
            {
              for(j=0;j<W-1;j++)
              {
                if( tv_image[i+1][j] == 255 )     //根据上一行的赛道来确定本行的赛道
                {
                  if( tv_image[i][j]== 250 || tv_image[i][j] == 255)
                  {
                    tv_image[i][j] = 255;
                  }
                  if( j-1 >=0 &&( tv_image[i][j-1] == 250 || tv_image[i][j-1] == 255 ))
                  {
                    tv_image[i][j-1] =255;
                  }
                  if( j+1<=W-1 &&(tv_image[i][j+1] == 250 || tv_image[i][j+1] == 255))
                  {
                    tv_image[i][j+1]= 255;
                  }
               }
             }   
              
              
              
              for(j=0;j<=W-1;j++)         //向左扫描，寻找第一个255点
              {
                if(tv_image[i][j] == 255)
                {
                  break;
                }
              }
              if(j==W)  //本行无赛道，跳出
              {
                break;
              }
              for(;j>=0;j--)      //根据上面找到的255,向回找255 或者 250
              {
                if(tv_image[i][j] == 250 || tv_image[i][j] == 255)
                {
                  tv_image[i][j] =255;
                }
                else
                {
                  fb_image[i][0] = j+1;
                  break;
                }
              }
              if(j==-1)   //本行无赛道，跳出
              {
                fb_image[i][0] = 0;
              }
              
              
              
              for(j=W-1;j>=0; j--)        //同上
              {
                if(tv_image[i][j] == 255)
                {
                  break;
                }
              }
              if(j==-1)
              {
                break;
              }
              for(;j<=W-1;j++)     //同上
              {
                if(tv_image[i][j] == 250 || tv_image[i][j] == 255)
                {
                  tv_image[i][j] = 255;
                }
                else
                {
                  fb_image[i][2] = j-1;
                  break;
                }
              }
              if(j==W)
              {
                fb_image[i][2] = 159;
              }
              
              
            }
        }
    }
#endif
}
/*---------------------------------------------------------------
 【函    数】twoBJ
 【功    能】过滤噪点
 【参    数】无
 【返 回 值】无
 【注意事项】对黑点进行第二次判断
 【来源】ys.journey
 ----------------------------------------------------------------*/
void twoBJ()
{
    int i,j,k,temp_centere;
  
  //寻找63行的中点
    temp_centere=W/2;	     //   上一次中点位置
    for(int i=0;i<=H/2;i++)
    {
      if(tv_image[H-1][temp_centere+i]==255)
      {
        temp_centere=temp_centere+i;
        break;
      }
      if( tv_image[H-1][temp_centere-i]==255)
      {
        temp_centere=temp_centere-i;
        break;
      }
    }
  
  
    for(i=H-1;i>=0;i--)        
    {
      for(j=temp_centere;j<W;j++)	//前一行中点位置作为本行的向右侧扫描的起点
      {
          if(tv_image[i][j]==0)		
          {
            if(j==temp_centere)      //第一个点就等于0，说明此行丢赛道，跳过
            {
              return ;
            }
            fb_image[i][2]=j-1;		//右侧扫描到第一个黑点，判定为赛道的边界
            break;
          }
          else			//改写赛道，消除干扰
          {
            tv_image[i][j]=255;
          }
      
      }
      if(j==W)
      {
        fb_image[i][2]=W-1;
      }
    
    
      for(k=temp_centere;k>=0;k--)	//同上
      {
          if(tv_image[i][k]==0)
          {
            if(k==temp_centere)     //第一个点就等于0，说明此行丢赛道，跳过
            {
              return ;
            }
            fb_image[i][0]=k+1;
            break;
          }
          else
          {
            tv_image[i][k]=255;
          }
          
      }
      
      if(k==-1)
      {
        fb_image[i][0]=0;
      }
      
      temp_centere=(j+k)/2;		//保存本行的中线，为下一行做参考
      
      fb_image[i][1]=(fb_image[i][0]+fb_image[i][2])/2;
      
      if((k==(W-1)/2&&j==(W-1)/2)||(k==0&& j==0))
      {
        break;
      }
      if( k == 2*H-1 && j == 2*H-1)
      {
        break;
      }
 
  }
  
}
/************************************************************  
【函数名称】Horizontal_line
【功    能】横向巡线函数
【参    数】无
【返 回 值】无
【实    例】Horizontal_line();
【注意事项】无
【来源】温水很好喝
***********************************************************/
// void Horizontal_line(void)
// {
//   uint8 i,j;
//   if(Pixle[H-1][W/2]==0)
//   {
//     if(Pixle[H-1][5]==white)
//       midline[H]=5;
//     else if(Pixle[H-1][W-5]==white)
//       midline[H]=W-5;
//     else
//       midline[H]=W/2;
//   }
//   else
// 	{
//     midline[H]=W/2;		
// 	}
	
//   for(i=H-1;i>0;i--)
//   {
//     for(j=midline[i+1];j>=0;j--)
//     {
//       if(Pixle[i][j]==0||j==0)
//       {
//         leftline[i]=j;
//         break;
//       }
//     }
//     for(j=midline[i+1];j<=W-1;j++)
//     {
//       if(Pixle[i][j]==0||j==W-1)
//       {
//         rightline[i]=j;
//         break;
//       }
//     }
//     midline[i]=(leftline[i]+rightline[i])/2;
//    if(Pixle[i-1][midline[i]]==0||i==0)
//     {
//    for(j=i;j>0;j--)
//    {
//     midline[j]=midline[i];
//     leftline[j]=midline[i];
//     rightline[j]=midline[i];
//    }
//       break;
//     }
//   }
// }
