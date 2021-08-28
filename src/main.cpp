//================================================================================
//! @brief Обработка художественного текста.
//!
//! @author Костя Вихлянцев (https://github.com/vihlancevk)
//! @file main.cpp
//! @date 28.08.2021
//!
//! @copyright Copyright (c) 2021
//--------------------------------------------------------------------------------

#include "fileOperations.h"
#include "sortText.h"

const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";

int main()
{
    FILE *finput = fopen(INPUT_FILE, "r");
    assert(finput != nullptr);
    int numberBytesFile = getFileSize(finput);

    char *str = (char*)calloc(numberBytesFile, sizeof(char));
    assert(str != nullptr);

    str  = (char *)readFile(finput, str, numberBytesFile);
    int linesCount = countNumberLines(str);

    Line *lines = (Line*)calloc(linesCount, sizeof(Line));
    assert(lines != nullptr);

    splitToLines(lines, linesCount, str);

    FILE *foutput = fopen(OUTPUT_FILE, "w");

    qsort(lines, sizeof(Line), offsetof(Line, str), 0, linesCount - 1, (int (*)(void *, void *))(compareStr));
    writeFile(lines, linesCount, foutput, "qsort");

    qsort(lines, sizeof(Line), offsetof(Line, str), 0, linesCount - 1, (int (*)(void *, void *))(compareStrReverse));
    writeFile(lines, linesCount, foutput, "reverse qsort");

    qsort(lines, sizeof(Line), offsetof(Line, str), 0, linesCount - 1, (int (*)(void *, void *))(compareNumbers));
    writeFile(lines, linesCount, foutput, "no sort");

    fclose(finput);
    fclose(foutput);

    free(str);
    free(lines);

    return 0;
}
