#include "dsplib/fft.h"
#include "dsplib/process.h"
#include "dsplib/tdoa.h"
#include "audio/wave.h"
#include "audio/VAD.h"
#include "stdio.h"
/*varlue*/
int TDOA_table[MIC_PAIR][SERCH_POINT];      //tdoa表格数据
struct mic_Array mic[MIC];                  //输入mic数据
const char  binfileAddr[2][12]={"./mic16.bin","tdoa1.bin"}; //bin文件的路径
char  filename[20]="out00.wav";
/* function  */
void init_func();
int data_read(int file);
void wav_read(float wav_data[WAV_SIZE]);
//int main(int argc, char* argv[])
int main()
{   int I;
    float wav_data[WAV_SIZE];
    init_func();
    //wav_data=audioread(filename);
    wav_read(wav_data);
    vad_init(wav_data);
   //vad_do_once(wav_data);
   // data_read(0);
   // tdoa_table_full(TDOA_table);
   // I = do_once_srp(mic,TDOA_table);
   // printf("I is %d\n",I);
}
  
/*******************************
 * 初始化函数
 * 在这里对需要初始化的函数进行
 * 初始化操作
*******************************/
void init_func(){
    myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(myFFT , FrameSize);
}

/*******************************
*获取表格的程序，这里用的是读预先
设计好的二进制文件，用于前期调试
*file=0 read  mic16.bin
*file=1 read  tdoa1.bin
*******************************/
int data_read(int file){
  float num=0;
  int num_int=0;
  int i,j;
  FILE *fp;//判断命令行是否正确
            //按读方式打开由argv[1]指出的文件
  if((fp=fopen(binfileAddr[file],"r"))==NULL){
    printf("The file <%s> can not be opened.\n",binfileAddr[file]);//打开操作不成功
    return -1;//结束程序的执行
  }
  if (file==0){
      
    for(i=0;i<MIC;i++){
         for(j=0;j<FrameSize;j++){
             fread( &num, sizeof(float), 1, fp ); 
              mic[i].mic_signal[j]=num;    
         }
     }
  }
  else if(file==1){
      for(i=0;i<SERCH_POINT;i++){
        for(j=0;j<MIC_PAIR;j++){
            fread( &num_int, sizeof(int), 1, fp );
            TDOA_table[j][i]=num_int;
         }
      }    
  }
  fclose(fp); //关闭fp所指文件
  return 0;
}

void  wav_read(float wav_data[WAV_SIZE]){
    FILE *fp;
    int i;
    float num;
    char wav_name[20]="./audio.bin";
     if((fp=fopen(wav_name,"r"))==NULL){
        printf("The file <%s> can not be opened.\n",wav_name);//打开操作不成功
     }
     for(i=0;i<WAV_SIZE;i++){
        fread( &num, sizeof(float), 1, fp );
        wav_data[i] = num;
     }
}

