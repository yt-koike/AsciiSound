#include<stdio.h>
//8bit stereo test
int main(){
FILE *fp=fopen("test4.aso","w");
fprintf(fp,"S\n");
fprintf(fp,"8\n");
fprintf(fp,"44100\n");

int phases=5;
int n=44100*phases*2;
fprintf(fp,"%d\n",n);

for(int j=1;j<=phases;j++){
for(int i=0;i<n/phases/2;i++){
fprintf(fp,"%d %d\n",(i*j)%256-128+1,0);
}
}
for(int j=1;j<=phases;j++){
for(int i=0;i<n/phases/2;i++){
fprintf(fp,"%d %d\n",0,(i*j)%256-128+1);
}
}
fclose(fp);
}
