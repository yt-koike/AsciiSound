#include<stdio.h>
int main(){
FILE *fp=fopen("test1.aso","w");
fprintf(fp,"M\n");
fprintf(fp,"8\n");
fprintf(fp,"44100\n");
int n=44100;
fprintf(fp,"%d\n",n);
for(int i=0;i<n;i++){
fprintf(fp,"%d\n",i%255-127);
}
fclose(fp);
}
