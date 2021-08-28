#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include "txlib.h"

/// Описание строки текста
struct Line {
    int lineNumber; ///< Номер строки в тексте.
    char *str; ///< Строка текста.
};

int getFileSize(FILE *finput);
void *readFile(FILE *finput, char *str, int numberBytesSize);
int countNumberLines(char *str);
void *findNextLine(char *str);
void splitToLines(Line *lines, int linesCount, char *str);
void moveToNextLine(FILE *foutput);
void writeFile(Line *lines, int linesCount, FILE *foutput, char *nameSort);

#endif
