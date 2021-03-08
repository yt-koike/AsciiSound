#include<stdio.h>
#include<math.h>
int main(){
FILE *fp=fopen("sinwave16.aso","w");
fprintf(fp,"M\n16\n");
int Hz=44100;
fprintf(fp,"%d\n",Hz);
int s=1;
unsigned int n=Hz*s;
fprintf(fp,"%d\n",n);
for(int i=0;i<n;i++){
float degree=3.14*i/n;
int r=(sin(degree*2)*65536)-32768+1;
fprintf(fp,"%d\n",r);
}
fclose(fp);
}
