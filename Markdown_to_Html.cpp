#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

char *replace_image_tags(const char *line) {
	static char newLine[MAX_LINE_LENGTH];
	memset(newLine, 0, sizeof(newLine));

	const char *imgStart = strstr(line, "![");
	const char *imgEnd = strstr(line, "](");
	const char *imgPathStart = strstr(line, "(");
	const char *imgPathEnd = strstr(line, ")");

	if (imgStart != NULL && imgEnd != NULL && imgPathStart != NULL && imgPathEnd != NULL) {
		int descLength = imgEnd - imgStart - 2;
		int pathLength = imgPathEnd - imgPathStart - 1;

		strncat(newLine, line, imgStart - line);  // Copy the line before the image tag
		strcat(newLine, "<div align='center'><img src='");
		strncat(newLine, imgPathStart + 1, pathLength);  // Copy the image path
		strcat(newLine, "' width='50%'></div>");
		strcat(newLine, imgPathEnd + 1);  // Copy the remaining part of the line after the image path
	} else {
		strcpy(newLine, line);  // Copy the line as it is
	}

	return newLine;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("请将一个txt文件拖动到该程序上\n");
		return 1;
	}

	FILE *inputFile = fopen(argv[1], "r");
	if (inputFile == NULL) {
		printf("无法打开输入文件\n");
		return 1;
	}

	char outputFileName[100];
	strcpy(outputFileName, argv[1]);
	char *extension = strstr(outputFileName, ".txt");
	if (extension != NULL) {
		strcpy(extension, "_converted.txt");
	} else {
		strcat(outputFileName, "_converted.txt");
	}

	FILE *outputFile = fopen(outputFileName, "w");
	if (outputFile == NULL) {
		printf("无法创建输出文件\n");
		return 1;
	}

	char line[MAX_LINE_LENGTH];
	while (fgets(line, sizeof(line), inputFile)) {
		const char *newLine = replace_image_tags(line);
		fputs(newLine, outputFile);
	}

	fclose(inputFile);
	fclose(outputFile);

	printf("文件处理完成，已生成输出文件：%s\n", outputFileName);

	return 0;
}
