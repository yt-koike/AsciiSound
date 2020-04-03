#ifndef ASCIISOUND_H
#define ASCIISOUND_H
#include<stdio.h>
void x_print2(FILE *fp,unsigned int n){
putc((n&(0xff<<8*0))>>8*0,fp);
putc((n&(0xff<<8*1))>>8*1,fp);
return;
}
void x_print4(FILE *fp,unsigned int n){
putc((n&(0xff<<8*0))>>8*0,fp);
putc((n&(0xff<<8*1))>>8*1,fp);
putc((n&(0xff<<8*2))>>8*2,fp);
putc((n&(0xff<<8*3))>>8*3,fp);
return;
}

int ASO2WAV(char *ASO_filename,char *WAV_filename){
FILE *aso_fp=fopen(ASO_filename,"r");
if(!aso_fp){
perror("ASO fileopen error");
return -1;
}
char MorS;//monoral or stereo
int bit;//generally 8 or 16 for WAV
int Hz;//generally 44100 for WAV
int data_N;//number of datas
fscanf(aso_fp,"%c",&MorS);
fscanf(aso_fp,"%d",&bit);
fscanf(aso_fp,"%d",&Hz);
fscanf(aso_fp,"%d",&data_N);

if(bit%8!=0)bit=(bit/8)*8;
if(bit==0)bit=8;
int byte=bit/8;

FILE *wav_fp=fopen(WAV_filename,"wb");
if(!wav_fp){
perror("WAV fileopen error");
return -1;
}

unsigned int filesize=44+data_N*byte*((MorS=='S')?2:1);

fprintf(wav_fp,"RIFF");
x_print4(wav_fp,filesize-8);
fprintf(wav_fp,"WAVEfmt ");
x_print4(wav_fp,16);
x_print2(wav_fp,1);
x_print2(wav_fp,(MorS=='S')?2:1);
x_print4(wav_fp,Hz);
x_print4(wav_fp,Hz*byte*((MorS=='S')?2:1));
x_print2(wav_fp,bit*((MorS=='S')?2:1)/8);
x_print2(wav_fp,bit);
fprintf(wav_fp,"data");
x_print4(wav_fp,filesize-126);

unsigned int buf;
int asodata;
for(unsigned int i=0;i<data_N;i++){
fscanf(aso_fp,"%d",&asodata);
if(bit==8)asodata+=127;
printf("%d\n",asodata);
fwrite(&asodata,byte,1,wav_fp);
}

fclose(aso_fp);
fclose(wav_fp);
}


#endif
