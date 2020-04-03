#include<stdio.h>
//16bit test
int main(){
FILE *fp=fopen("test3.aso","w");
fprintf(fp,"M\n");
fprintf(fp,"16\n");
fprintf(fp,"44100\n");

int phases=5;
int n=44100*phases;
fprintf(fp,"%d\n",n);

for(int j=1;j<=phases;j++){
for(int i=0;i<n/phases;i++){
fprintf(fp,"%d\n",(i*j)%65536-32768+1);
}
}
fclose(fp);
}
