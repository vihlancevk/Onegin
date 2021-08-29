//================================================================================
//! @mainpage OneginProgram
//!
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

/// �������� ���������� �����.
struct Sort{
    const char *nameSort; ///< ��� ����������.
    int (*strCompare)(void *, void *); ///< ������� ���������, ������������ � ����������.
};

const int NUMBER_OF_SORT = 3;
const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";

int main()
{
    FILE *finput = fopen(INPUT_FILE, "r");
    assert(finput != nullptr);

    int numberBytesFile = getFileSize(finput);
    if (numberBytesFile == -1)
    {
        printf("Error getFileSize\n");
        return 0;
    }

    char *str = (char*)calloc(numberBytesFile, sizeof(char));
    assert(str != nullptr);

    str  = (char *)readFile(finput, str, numberBytesFile);
    if (str == nullptr)
    {
        printf("Error readFile\n");
        return 0;
    }

    int linesCount = countNumberLines(str);

    Line *lines = (Line*)calloc(linesCount, sizeof(Line));
    assert(lines != nullptr);

    splitToLines(lines, linesCount, str);

    FILE *foutput = fopen(OUTPUT_FILE, "w");
    assert(foutput != nullptr);

    Sort sorts[] = {{"qsort",         (int (*)(void *, void *))(compareStr)       },
                    {"reverse qsort", (int (*)(void *, void *))(compareStrReverse)},
                    {"no sort",       (int (*)(void *, void *))(compareNumbers)   }};

    for(int i = 0; i < NUMBER_OF_SORT; i++)
    {
        qsort(lines, sizeof(Line), offsetof(Line, str), 0, linesCount - 1, sorts[i].strCompare);
        writeFile(lines, linesCount, foutput, sorts[i].nameSort);
    }

    fclose(finput);
    fclose(foutput);

    free(str);
    free(lines);

    return 0;
}

