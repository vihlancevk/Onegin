#include "fileOperations.h"
#include "sortText.h"

const char *PUNCTUATION_MARKS = " ,.!?:-;\"(){}<>";

//================================================================================
//! @brief Функция обмена значений двух элементов.
//!
//! @param [out] elem1 указатель на первый элемент.
//! @param [out] elem2 указатель на второй элемент.
//! @param [in] struct_size размер элемента в байтах.
//--------------------------------------------------------------------------------

void swap(void *elem1, void *elem2, size_t struct_size)
{
    assert(elem1 != nullptr);
    assert(elem2 != nullptr);
    assert(struct_size != 0);

    unsigned long long *arrBytesULL1 = (unsigned long long *) elem1;
    unsigned long long *arrBytesULL2 = (unsigned long long *) elem2;
    size_t numberIterations = struct_size / sizeof(unsigned long long);
    for (size_t num = 0; num < numberIterations; num++)
    {
        unsigned long long bufferULL = arrBytesULL1[num];
        arrBytesULL1[num] = arrBytesULL2[num];
        arrBytesULL2[num] = bufferULL;
    }
    if ((struct_size - numberIterations*sizeof(unsigned long long)) / sizeof(int) == 1)
    {
        int *arrBytesI1 = (int *) ((int *) elem1 + numberIterations*sizeof(unsigned long long));
        int *arrBytesI2 = (int *) ((int *) elem2 + numberIterations*sizeof(unsigned long long));
        int bufferI = arrBytesI1[0];
        arrBytesI1[0] = arrBytesI2[0];
        arrBytesI2[0] = bufferI;
    }
    if ((struct_size - numberIterations*sizeof(unsigned long long) - sizeof(int)) / sizeof(short int) == 1)
    {
        short int *arrBytesSI1 = (short int *) ((short int *) elem1 + numberIterations*sizeof(unsigned long long) + sizeof(int));
        short int *arrBytesSI2 = (short int *) ((short int *) elem2 + numberIterations*sizeof(unsigned long long) + sizeof(int));
        short int bufferSI = arrBytesSI1[0];
        arrBytesSI1[0] = arrBytesSI2[0];
        arrBytesSI2[0] = bufferSI;
    }
    if ((struct_size - numberIterations*sizeof(unsigned long long) - sizeof(int) - sizeof(short int)) / sizeof(char) == 1)
    {
        char *arrBytesC1 = (char *) ((char *) elem1 + numberIterations*sizeof(unsigned long long) + sizeof(int) + sizeof(short int));
        char *arrBytesC2 = (char *) ((char *) elem2 + numberIterations*sizeof(unsigned long long) + sizeof(int) + sizeof(short int));
        char bufferC = arrBytesC1[0];
        arrBytesC1[0] = arrBytesC2[0];
        arrBytesC2[0] = bufferC;
    }
}

//================================================================================
//! @brief Функция проверки символа на его принодлежность к пунктационным знакам.
//!
//! @param [in] symbol символ, который необходимо проверить.
//!
//! @return логическое значние: да или нет.
//--------------------------------------------------------------------------------

bool isPunctuationMark(int symbol)
{
    return strchr(PUNCTUATION_MARKS, symbol) != nullptr;
}

//================================================================================
//! @brief Функция, которая пропускает пунктационне знаки в строке.
//!
//! @param [in] str указатель на начало строки.
//! @param [in] endStr указатель на конец строки.
//! @param [in] isReverse логическая переменная, которая показывает, с начала
//!                       или с конца строки необходимо пропустить
//!                       пунктационные знаки.
//!
//! @return Указатель на первый символ в строке (с начала или
//!         с конца - это зависит от значения переменной isReverse),
//!         который является не пунктационным.
//--------------------------------------------------------------------------------

void *skipPunctuationMark(char *str, char *endStr, bool isReverse)
{
    assert(str != nullptr);
    assert(endStr != nullptr);

    if (!isReverse)
    {
        char *ptrStr = str;

        while(isPunctuationMark(*ptrStr) && *ptrStr != '\0')
        {
            ptrStr++;
        }

        return ptrStr;
    }
    else
    {
        char *ptrEndStr = endStr;

        while(isPunctuationMark(*ptrEndStr) && ptrEndStr >= str)
        {
            ptrEndStr--;
        }

        return ptrEndStr;
    }
}

//================================================================================
//! @brief Функция сравнения строк в лексиграфическом порядке слева направо.
//!
//! @param [in] line1 указатель на первый элемент массива структур Line.
//! @param [in] line2 указатель на первый элемент массива структур Line.
//!
//! @note Функция сравнивает только символы строк, игнорируя пунктационные знаки.
//!
//! @return Число больше нуля, если первая строка больше второй, нуль,
//!         если строки равны, и число меньше нуля, если первая строка
//!         меньше второй.
//--------------------------------------------------------------------------------

int compareLine(const Line *line1, const Line *line2)
{
    assert(line1 != nullptr);
    assert(line2 != nullptr);

    char* str1 = (*line1).str;
    char* str2 = (*line2).str;

    assert(str1 != nullptr);
    assert(str2 != nullptr);

    char *ptrStr1 = (char*)skipPunctuationMark(str1, str1 + (*line1).sizeStr - 1, false);
    char *ptrStr2 = (char*)skipPunctuationMark(str2, str2 + (*line2).sizeStr - 1, false);

    for ( ; *ptrStr1 == *ptrStr2; )
    {
        if (isPunctuationMark(*ptrStr1))
        {
            ptrStr1 = (char*)skipPunctuationMark(ptrStr1, str1 + (*line1).sizeStr - 1, false);
            continue;
        }

        if (isPunctuationMark(*ptrStr2))
        {
            ptrStr2 = (char*)skipPunctuationMark(ptrStr2, str1 + (*line2).sizeStr - 1, false);
            continue;
        }

        if (*ptrStr1 == '\0' || *ptrStr2 == '\0')
        {
            return ((*line1).sizeStr - (*line2).sizeStr);
        }

        ptrStr1++;
        ptrStr2++;
    }

    return (*ptrStr1 - *ptrStr2);
}

//================================================================================
//! @brief Функция сравнения строк в лексиграфическом порядке справа налево.
//!
//! @param [in] line1 указатель на первый элемент массива структур Line.
//! @param [in] line2 указатель на первый элемент массива структур Line.
//!
//! @note Функция сравнивает только символы строк, игнорируя пунктационные знаки.
//!
//! @return Число больше нуля, если первая строка больше второй, нуль,
//!         если строки равны, и число меньше нуля, если первая строка
//!         меньше второй.
//--------------------------------------------------------------------------------

int compareLineReverse(const Line *line1, const Line *line2)
{
    assert(line1 != nullptr);
    assert(line2 != nullptr);

    char *str1 = (*line1).str;
    char *str2 = (*line2).str;

    assert(str1 != nullptr);
    assert(str2 != nullptr);

    char *ptrEndStr1 = (char*)skipPunctuationMark(str1, str1 + (*line1).sizeStr - 1, true);
    char *ptrEndStr2 = (char*)skipPunctuationMark(str2, str2 + (*line2).sizeStr - 1, true);

    for ( ; *ptrEndStr1 == *ptrEndStr2; )
    {
        if (isPunctuationMark(*ptrEndStr1))
        {
            ptrEndStr1 = (char*)skipPunctuationMark(str1, ptrEndStr1, true);
            continue;
        }

        if (isPunctuationMark(*ptrEndStr2))
        {
            ptrEndStr2 = (char*)skipPunctuationMark(str2, ptrEndStr2, true);
            continue;
        }

        if (ptrEndStr1 == str1 || ptrEndStr2 == str2)
        {
            return ((*line1).sizeStr - (*line1).sizeStr);
        }

        ptrEndStr1--;
        ptrEndStr2--;
    }

    return (*ptrEndStr1 - *ptrEndStr2);
}

//================================================================================
//! @brief Функция сортировки массива элементов.
//!
//! @param [out] elems указатель на массив элементов.
//! @param [in] struct_size размер элемента массива в байтах.
//! @param [in] left левая граница массива
//! @param [in] right правая граница массива
//! @param [in] LineCompare указатель на функцию сравнения, используемую
//!             при сортировке.
//--------------------------------------------------------------------------------

void qsort(void *elems, size_t struct_size, int left, int right,
           int (*LineCompare)(const void *elem1, const void *elem2))
{
    assert(elems != nullptr);
    assert(struct_size != 0);

    int pivot = left;

    if (left >= right)
        return;

    swap((void *)(elems + left * struct_size), (void *)(elems + ((left + right) / 2) * struct_size), struct_size);

    for (int i = left + 1; i <= right; i++)
    {
        if ((*LineCompare)((elems + i*struct_size), (elems + left*struct_size))  < 0)
        {
            pivot++;
            swap((void *)(elems + pivot * struct_size), (void *)(elems + i * struct_size), struct_size);
        }
    }

    swap((void *)(elems + left * struct_size), (void *)(elems + pivot * struct_size), struct_size);
    qsort(elems, struct_size, left, pivot - 1, LineCompare);
    qsort(elems, struct_size, pivot + 1, right, LineCompare);
}
