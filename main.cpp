//================================================================================
//!
//--------------------------------------------------------------------------------

#include "txlib.h"

//================================================================================
//!
//--------------------------------------------------------------------------------

struct Line {
    int lineNumber;
    char *str;
};

int getFileSize(FILE *finput);
void *readFile(FILE *finput, char *str, int numberBytesSize);
int countNumberLines(char *str);
void *findNextLine(char *str);
void splitToLines(Line *lines, int linesCount, char *str);
void moveToNextLine(FILE *foutput);
void writeFile(Line *lines, int linesCount, FILE *foutput, char *nameSort);
void swap(void *lines, size_t struct_size, int i, int j);
bool isPunctuationMark(int symbol);
void *skipPunctuationMark(char *str, char *endStr, bool isReverse);
int compareStr(char **str1_ptr, char **str2_ptr);
int compareStrReverse(char **str1_ptr, char **str2_ptr);
int compareNumbers(int *number1, int *number2);
void qsort(void *elems, size_t struct_size,  size_t offset, int left, int right, int (*strCompare)(void *, void *));

const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";
const char *PUNCTUATION_MARKS = " ,.!?:-;\"(){}<>";

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

//================================================================================
//!
//--------------------------------------------------------------------------------

int getFileSize(FILE *finput)
{
    assert(finput != nullptr);

    if (fseek(finput, 0, SEEK_END))
    {
        printf("Error fseek\n");
        abort();
    }

    long numberBytesFile = ftell(finput);
    if (numberBytesFile == -1L)
    {
        printf("Error ftell\n");
        abort();
    }

    rewind(finput);

    return (int)numberBytesFile;
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void *readFile(FILE *finput, char *str, int numberBytesFile)
{
    assert(finput != nullptr);
    assert(str != nullptr);
    assert(numberBytesFile > 0);

    if ((fread(str, sizeof(char), numberBytesFile, finput) != numberBytesFile) && !feof(finput))
    {
        printf("Error fread\n");
        abort();
    }

    return str;
}

//================================================================================
//!
//--------------------------------------------------------------------------------

int countNumberLines(char *str)
{
    assert(str != nullptr);

    int linesCount = 0;
    int length = strlen(str);

    for (int i = 0; i <= length; i++)
    {
        if (str[i] == '\n')
        {
            linesCount++;
            str[i] = '\0';
        }
    }

    return (linesCount + 1);
}

void *findNextLine(char *str)
{
    assert(str != nullptr);

    char *ptrStr = str;

    while (*ptrStr != '\0')
    {
        ptrStr++;
    }

    return ptrStr + 1;
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void splitToLines(Line *lines, int linesCount, char *str)
{
    assert(lines != nullptr);
    assert(linesCount > 0);
    assert(str != nullptr);

    char *ptrStr = str;

    for (int i = 0; i < linesCount; i++)
    {
        lines[i].lineNumber = i + 1;
        lines[i].str = ptrStr;
        ptrStr = (char *)findNextLine(ptrStr);
    }
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void moveToNextLine(FILE *foutput)
{
    assert(foutput != nullptr);

    fputs("\n", foutput);
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void writeFile(Line *lines, int linesCount, FILE *foutput, char *nameSort)
{
    assert(lines != nullptr);
    assert(linesCount > 0);
    assert(foutput != nullptr);
    assert(nameSort != nullptr);

    fputs(nameSort, foutput);
    moveToNextLine(foutput);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void swap(void *lines, size_t struct_size, int i, int j)
{
    assert(lines != nullptr);
    assert(struct_size != 0);
    assert(i > -1);
    assert(j > -1);

    void *buffer = calloc(1, struct_size);
    assert(buffer != nullptr);
    memcpy(buffer, (lines + i * struct_size), struct_size);

    memcpy((lines + i * struct_size), (lines + j * struct_size), struct_size);
    memcpy((lines + j * struct_size), buffer, struct_size);

    free(buffer);
}

//================================================================================
//!
//--------------------------------------------------------------------------------

bool isPunctuationMark(int symbol)
{
    return strchr(PUNCTUATION_MARKS, symbol) != nullptr;
}

//================================================================================
//!
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
//!
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
//!
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
//!
//--------------------------------------------------------------------------------

int compareNumbers(int *number1, int *number2)
{
    assert(number1 != nullptr);
    assert(number2 != nullptr);

    return (*number1 - *number2);
}

//================================================================================
//!
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
