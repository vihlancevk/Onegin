#include "workingFiles.h"
#include "sortText.h"

const char *PUNCTUATION_MARKS = " ,.!?:-;\"(){}<>";

//================================================================================
//! @brief Функция обмена значений двух элементов.
//!
//! @param [out] elems указатель на массив элементов.
//! @param [in] struct_size размер элемента в байтах.
//! @param [in] i сдвиг первого элемента относительно указателя.
//! @param [in] j сдвиг второго элемента относительно указателя.
//--------------------------------------------------------------------------------

void swap(void *elems, size_t struct_size, int i, int j)
{
    assert(elems != nullptr);
    assert(struct_size != 0);
    assert(i > -1);
    assert(j > -1);

    void *buffer = calloc(1, struct_size);
    assert(buffer != nullptr);
    memcpy(buffer, (elems + i * struct_size), struct_size);

    memcpy((elems + i * struct_size), (elems + j * struct_size), struct_size);
    memcpy((elems + j * struct_size), buffer, struct_size);

    free(buffer);
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
//! @param [in] str1_ptr указатель на область памяти,
//!                      содержащую адрес первой строки.
//! @param [in] str2_ptr указатель на область памяти,
//!                      содержащую адрес второй строки.
//!
//! @note Функция сравнивает только символы строк, игнорируя пунктационные знаки.
//!
//! @return Число больше нуля, если первая строка больше второй, нуль,
//!         если строки равны, и число меньше нуля, если первая строка
//!         меньше второй.
//--------------------------------------------------------------------------------

int compareStr(char **str1_ptr, char **str2_ptr)
{
    assert(str1_ptr != nullptr);
    assert(str2_ptr != nullptr);

    char* str1 = *str1_ptr;
    char* str2 = *str2_ptr;

    assert(str1 != nullptr);
    assert(str2 != nullptr);

    char *ptrStr1 = (char*)skipPunctuationMark(str1, str1 + strlen(str1) - sizeof(char), false);
    char *ptrStr2 = (char*)skipPunctuationMark(str2, str2 + strlen(str2) - sizeof(char), false);

    for ( ; *ptrStr1 == *ptrStr2; )
    {
        if (isPunctuationMark(*ptrStr1))
        {
            ptrStr1 = (char*)skipPunctuationMark(ptrStr1, str1 + strlen(str1) - sizeof(char), false);
            continue;
        }

        if (isPunctuationMark(*ptrStr2))
        {
            ptrStr2 = (char*)skipPunctuationMark(ptrStr2, str2 + strlen(str2) - sizeof(char), false);
            continue;
        }

        if (*ptrStr1 == '\0' || *ptrStr2 == '\0')
        {
            return ((int)strlen(str1) - (int)strlen(str2));
        }

        ptrStr1++;
        ptrStr2++;
    }

    return (*ptrStr1 - *ptrStr2);
}

//================================================================================
//! @brief Функция сравнения строк в лексиграфическом порядке справа налево.
//!
//! @param [in] str1_ptr указатель на область памяти,
//!                      содержащую адрес первой строки.
//! @param [in] str2_ptr указатель на область памяти,
//!                      содержащую адрес второй строки.
//!
//! @note Функция сравнивает только символы строк, игнорируя пунктационные знаки.
//!
//! @return Число больше нуля, если первая строка больше второй, нуль,
//!         если строки равны, и число меньше нуля, если первая строка
//!         меньше второй.
//--------------------------------------------------------------------------------

int compareStrReverse(char **str1_ptr, char **str2_ptr)
{
    assert(str1_ptr != nullptr);
    assert(str2_ptr != nullptr);

    char *str1 = *str1_ptr;
    char *str2 = *str2_ptr;

    assert(str1 != nullptr);
    assert(str2 != nullptr);

    char *ptrEndStr1 = (char*)skipPunctuationMark(str1, str1 + strlen(str1) - sizeof(char), true);
    char *ptrEndStr2 = (char*)skipPunctuationMark(str2, str2 + strlen(str2) - sizeof(char), true);

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
            return ((int)strlen(str1) - (int)strlen(str2));
        }

        ptrEndStr1--;
        ptrEndStr2--;
    }

    return (*ptrEndStr1 - *ptrEndStr2);
}

//================================================================================
//! @brief Функция сравнения двух целых чисел.
//!
//! @param [in] number1 указатель на первое число.
//! @param [in] number2 указатель на второе число.
//!
//! @return Число больше нуля, если первое число больше второго, нуль,
//!         если числа равны, и число меньше нуля, если первое число
//!         меньше второго.
//--------------------------------------------------------------------------------

int compareNumbers(int *number1, int *number2)
{
    assert(number1 != nullptr);
    assert(number2 != nullptr);

    return (*number1 - *number2);
}

//================================================================================
//! @brief Функция сортировки массива элементов.
//!
//! @param [out] elems указатель на массив элементов.
//! @param [in] struct_size размер элемента массива в байтах.
//! @param [in] offset сдвиг относительно элемента массива в байтах.
//! @param [in] left левая граница массива
//! @param [in] right правая граница массива
//! @param [in] strCompare указатель на функцию сравнения, используемую
//!             при сортировке.
//--------------------------------------------------------------------------------

void qsort(void *elems, size_t struct_size, size_t offset, int left, int right, int (*strCompare)(void *, void *))
{
    assert(elems != nullptr);
    assert(struct_size != 0);
    assert(strCompare != nullptr);

    int pivot = left;

    if (left >= right)
        return;

    swap(elems, struct_size, left, (left + right) / 2);

    for (int i = left + 1; i <= right; i++)
    {
        if ((*strCompare)(((elems + i*struct_size) + offset), ((elems + left*struct_size) + offset)) < 0)
        {
            pivot++;
            swap(elems, struct_size, pivot, i);
        }
    }

    swap(elems, struct_size, left, pivot);
    qsort(elems, struct_size, offset, left, pivot - 1, strCompare);
    qsort(elems, struct_size, offset, pivot + 1, right, strCompare);
}
