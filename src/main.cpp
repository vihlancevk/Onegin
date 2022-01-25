//================================================================================
//! @mainpage OneginProgram
//!
//! @brief Обработка художественного текста.
//
//! @author Костя Вихлянцев (https://github.com/vihlancevk)
//! @file main.cpp
//! @date 28.08.2021
//--------------------------------------------------------------------------------

#include "../libs/fileOperations.h"
#include "../libs/sortText.h"

/// Описание сортировки текта.
struct Sort{
    const char *nameSort;                                      ///< Имя сортировки.
    int (*LineCompare)(const void *elem1, const void *elems2); ///< Функция сравнения, используемая в сотрировке.
};

const int NUMBER_OF_SORT = 2;
const char *INPUT_FILE  = "../res/input.txt";
const char *OUTPUT_FILE = "../res/output.txt";

void *copyLine(Line *lines, int linesCount);

int main()
{
    int linesCount = 0;
    Line *lines = (Line *) fillStructLine(INPUT_FILE, &linesCount);
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

    free(lines);
    free(linesCopy);

    return 0;
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
