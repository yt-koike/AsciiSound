#ifndef ASCIISOUND_H
#define ASCIISOUND_H
#include <stdio.h>
#define DEBUG 0
#define COLUMN_SIZE 50

int isFig(const char ch)
{
	return '0' <= ch && ch <= '9';
}

int isNewLine(const char ch)
{
	return ch == '\n' || ch == '\r';
}

unsigned int atoi(const char *str)
{
	unsigned int result = 0;
	while (isFig(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return result;
}

int search(const char *str, const char element)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == element)
			return i;
	}
	return -1;
}

unsigned int count(const char *str, const char element)
{
	int count = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == element)
			count++;
	}
	return count;
}

unsigned int readline(FILE *fp, char *buf, unsigned int char_limit)
{
	for (unsigned int i = 0; i < char_limit - 1; i++)
	{
		char data = getc(fp);
		if (data < 0)
			return 0;
		if (isNewLine(data))
		{
			buf[i] = '\0';
			return i;
		}
		buf[i] = data;
	}
	perror("Invalid size column (Too long or Too short)");
	return 0;
}

unsigned int x_read(FILE *fp, const unsigned int byte)
{
	unsigned int result = 0;
	for (int i = 0; i < byte; i++)
		result = result | (getc(fp) << 8 * i);
	return result;
}

void x_write(FILE *fp, const unsigned int n, const unsigned int byte)
{
	for (int i = 0; i < byte; i++)
		putc((n & (0xff << 8 * i)) >> 8 * i, fp);
}

int ASO2WAV(char *ASO_filename, char *WAV_filename)
{
	FILE *aso_fp = fopen(ASO_filename, "r");
	if (!aso_fp)
	{
		perror("ASO fileopen error");
		return -1;
	}

	FILE *wav_fp = fopen(WAV_filename, "wb");
	if (!wav_fp)
	{
		perror("WAV fileopen error");
		return -1;
	}

	unsigned int data_N = 0;
	char buf[COLUMN_SIZE];
	while (readline(aso_fp, buf, COLUMN_SIZE) > 0)
	{
		if (buf[0] != '#')
			data_N++;
	}
	data_N--;
	fseek(aso_fp, 0L, SEEK_SET);

	unsigned int firstline_len;
	char firstline[COLUMN_SIZE];
	do
	{
		firstline_len = readline(aso_fp, firstline, COLUMN_SIZE);
	} while (firstline[0] == '#');
	unsigned int Hz = atoi(firstline);
	unsigned int bit = 0;
	int n;
	if ((n = search(firstline, ' ')) >= 0)
	{
		bit = atoi(&firstline[n + 1]);
	}
	if (bit == 0)
	{
		perror("Bit Estimation not supported yet");
		return -1;
	}

	unsigned int line_len;
	char line[COLUMN_SIZE];
	do
	{
		line_len = readline(aso_fp, line, COLUMN_SIZE);
		if (line_len == 0)
			return -1;
	} while (line[0] == '#');
	unsigned int channel = 1 + count(line, ' ');
	if (channel > 2)
	{
		perror("Too much channels");
		return -1;
	}
	unsigned int byte = bit / 8;
	unsigned int filesize = 44 + data_N * byte * channel;
	fprintf(wav_fp, "RIFF");
	x_write(wav_fp, filesize - 8, 4);
	fprintf(wav_fp, "WAVEfmt ");
	x_write(wav_fp, 16, 4);
	x_write(wav_fp, 1, 2);
	x_write(wav_fp, channel, 2);
	x_write(wav_fp, Hz, 4);
	x_write(wav_fp, Hz * byte * channel, 4);
	x_write(wav_fp, bit * channel / 8, 2);
	x_write(wav_fp, bit, 2);
	fprintf(wav_fp, "data");
	x_write(wav_fp, filesize - 126, 4);
	printf("Datasize:%d\n", data_N);
	printf("Channel:%d\n", channel);
	printf("bit:%d\n", bit);
	printf("Hz:%d\n", Hz);
	if (byte != 1 && byte != 2)
	{
		perror("Invalid Byte number");
		return -1;
	}
	do
	{
		if (DEBUG)
			printf("%s: ", line);
		int idx = 0;
		for (int i = 0; i < channel - 1; i++)
		{
			unsigned int data = atoi(&line[idx]);
			x_write(wav_fp, data, byte);
			while (line[idx] != ' ')
				idx++;
			idx++;
		}
		x_write(wav_fp, atoi(&line[idx]), byte);
	} while ((line_len = readline(aso_fp, line, COLUMN_SIZE)) > 0);
	fclose(aso_fp);
	fclose(wav_fp);
}
int WAV2ASO(char *WAV_filename, char *ASO_filename)
{
	FILE *wav_fp = fopen(WAV_filename, "rb");
	if (!wav_fp)
	{
		perror("WAV fileopen error");
		return -1;
	}
	FILE *aso_fp = fopen(ASO_filename, "w");
	if (!aso_fp)
	{
		perror("ASO fileopen error");
		return -1;
	}
	while (1)
	{
		if (getc(wav_fp) != 'f')
			continue;
		if (getc(wav_fp) != 'm')
			continue;
		if (getc(wav_fp) != 't')
			continue;
		if (getc(wav_fp) != ' ')
			continue;
		break;
	}
	x_read(wav_fp, 6);
	unsigned int channel = x_read(wav_fp, 2);
	unsigned int Hz = x_read(wav_fp, 4);
	x_read(wav_fp, 4);
	unsigned int block_size = x_read(wav_fp, 2);
	while (1)
	{
		if (getc(wav_fp) != 'd')
			continue;
		if (getc(wav_fp) != 'a')
			continue;
		if (getc(wav_fp) != 't')
			continue;
		if (getc(wav_fp) != 'a')
			continue;
		break;
	}
	unsigned int datasize = x_read(wav_fp, 4);
	unsigned int data_N = datasize / block_size;
	unsigned int byte = block_size / channel;
	unsigned int data;
	printf("START\n");
	printf("Hz:%d bit:%d\n", Hz, byte * 8);
	fprintf(aso_fp, "%d %d\n", Hz, byte * 8);
	for (int i = 0; i < data_N; i++)
	{
		for (int j = 0; j < channel - 1; j++)
		{
			data = x_read(wav_fp, byte);
			if (DEBUG)
				printf("%d ", data);
			fprintf(aso_fp, "%d ", data);
		}
		data = x_read(wav_fp, byte);
		if (DEBUG)
			printf("%d\n", data);
		fprintf(aso_fp, "%d\n", data);
	}
}
#endif
