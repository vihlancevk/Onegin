//================================================================================
//! @mainpage OneginProgram
//!
//! @brief Обработка художественного текста.
//
//! @author Костя Вихлянцев (https://github.com/vihlancevk)
//! @file main.cpp
//! @date 28.08.2021
//--------------------------------------------------------------------------------

#include "fileOperations.h"
#include "sortText.h"

/// Описание сортировки текта.
struct Sort{
    const char *nameSort;                                      ///< Имя сортировки.
    int (*LineCompare)(const void *elem1, const void *elems2); ///< Функция сравнения, используемая в сотрировке.
};

const int NUMBER_OF_SORT = 2;
const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";

void *fillStructLine(const char *nameFile,int *linesCount, char *str);
void *copyLine(Line *lines, int linesCount);

int main()
{
    int linesCount = 0;
    char *str = nullptr;
    Line *lines = (Line *) fillStructLine(INPUT_FILE, &linesCount, str);
    Line *linesCopy = (Line *) copyLine(lines, linesCount);

    FILE *foutput = fopen(OUTPUT_FILE, "w");
    assert(foutput != nullptr);

    Sort sorts[] = {{"qsort",         (int (*)(const void *elem1, const void *elem2))(compareLine)       },
                    {"reverse qsort", (int (*)(const void *elem1, const void *elem2))(compareLineReverse)}};

    for (int i = 0; i < NUMBER_OF_SORT; i++)
    {
        qsort(lines, sizeof(Line), 0, linesCount - 1, sorts[i].LineCompare);
        writeFile(lines, linesCount, foutput, sorts[i].nameSort);
    }

    writeFile(linesCopy, linesCount, foutput, "no sotr");

    fclose(foutput);

    free(str);
    free(lines);
    free(linesCopy);

    return 0;
}

//================================================================================
//! @brief Функция заполняет массив струтур Line строками из файла.
//!
//! @param [in] nameFile имя файла.
//! @param [out] linesCount указатель на  переменную, в которой будет
//!              храниться количество строк в файле.
//! @param [in] str указатель на буфер, в которой будет скопирован файл nameFile.
//!
//! @return Указатель на заполненный массив структур Line.
//--------------------------------------------------------------------------------

void *fillStructLine(const char* nameFile ,int *linesCount, char *str)
{
    assert(nameFile != nullptr);
    assert(linesCount != nullptr);

    FILE *finput = fopen(nameFile, "r");
    assert(finput != nullptr);

    int numberBytesFile = getFileSize(finput);
    if (numberBytesFile == -1)
    {
        printf("Error getFileSize\n");
        return 0;
    }

    str = (char*)calloc(numberBytesFile + 1, sizeof(char));
    assert(str != nullptr);

    str  = (char *)readFile(finput, str, numberBytesFile);
    if (str == nullptr)
    {
        printf("Error readFile\n");
        return 0;
    }

    *linesCount = countNumberLines(str, numberBytesFile);

    Line *lines = (Line*)calloc(*linesCount, sizeof(Line));
    assert(lines != nullptr);

    splitToLines(lines, *linesCount, str);

    fclose(finput);

    return lines;
}

//================================================================================
//! @brief Функция создает копию массива струтур Line.
//!
//! @param [in] lines массив структур Line, который нужно скопировать.
//! @param [in] linesCount количество элементов в массиве структур Line.
//!
//! @return Возвращает указатель на копию массива структур Line.
//--------------------------------------------------------------------------------

void *copyLine(Line *lines, int linesCount)
{
    Line *linesCopy = (Line *)calloc(linesCount, sizeof(Line));
    memcpy(linesCopy, lines, sizeof(Line)*linesCount);

    return linesCopy;
}
