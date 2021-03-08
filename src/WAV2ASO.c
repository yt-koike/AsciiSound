#include "../lib/AsciiSound.h"
int main(int args, char **argv)
{
    if (args >= 3)
        WAV2ASO(argv[1], argv[2]);
    else if (args == 2)
        WAV2ASO(argv[1], "output.aso");
    else
        WAV2ASO("test1.wav", "output.aso");
    return 0;
}
