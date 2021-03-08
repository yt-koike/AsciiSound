#include<stdio.h>
#include<math.h>
int main(){
FILE *fp=fopen("sinwave.aso","w");
fprintf(fp,"M\n8\n44100\n");
int n=44100;
fprintf(fp,"%d\n",n);
for(int i=0;i<n;i++){
float degree=3.14*i/n;
int r=(sin(degree*1000)*255)-128+1;
fprintf(fp,"%d\n",r);
}
fclose(fp);
}
