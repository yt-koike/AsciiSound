#include<stdio.h>
int main(){
FILE *fp=fopen("test2.aso","w");
fprintf(fp,"M\n");
fprintf(fp,"8\n");
fprintf(fp,"44100\n");

int n=44100*10;
fprintf(fp,"%d\n",n);

int phases=10;
for(int j=1;j<=phases;j++){
for(int i=0;i<n/phases;i++){
fprintf(fp,"%d\n",(i*j)%255-127);
}
}
fclose(fp);
}
