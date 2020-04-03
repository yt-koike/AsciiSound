#include"../lib/AsciiSound.h"
int main(int args,char **argv){
if(args>=2)
ASO2WAV(argv[1],"output.wav");
else
ASO2WAV("test1.aso","output.wav");
return 0;
}
