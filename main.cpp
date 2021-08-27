#include "txlib.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

struct Line {
    int lineNumber;
    char *str;
};

int countNumberLines(FILE *finput);
void readFile(Line lines[], int linesCount , FILE *finput);
void moveToNextLine(FILE *foutput);
void writeFile(Line lines[], int linesCount, FILE *foutput);
void swap(Line lines[], int i, int j);
bool isPunctuationMark(int symbol);
int myStrcmp(char *str1, char *str2);
int reverseStrcmp(char *str1, char *str2);
int compareNumbers(int *number1, int *number2);
void qsort(Line lines[], int left, int right, int (*strCompare)(void *, void *), bool isStrCmp);

const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";
const char *PUNCTUATION_MARKS = " ,.!?:-;\"(){}<>";

int main()
{
    FILE *finput = fopen(INPUT_FILE, "r");
    int linesCount = countNumberLines(finput);
    Line lines[linesCount];

    for (int i = 0; i < linesCount; i++)
    {
        lines[i].str = nullptr;
    }

    readFile(lines, linesCount, finput);

    FILE *foutput = fopen(OUTPUT_FILE, "w");

    writeFile(lines, linesCount, foutput);

    fclose(finput);
    fclose(foutput);

    return 0;
}

int countNumberLines(FILE *finput)
{
    assert(finput != nullptr);

    int linesCount = 0;
    int symbol = fgetc(finput);

    while (symbol != EOF)
    {
        if (symbol == '\n')
            linesCount++;

        symbol = fgetc(finput);
    }

    rewind(finput);

    return (linesCount + 1);
}

void readFile(Line lines[], int linesCount, FILE *finput)
{
    assert(finput != nullptr);

    char *str = nullptr;

    for (int i = 0; i < linesCount; i++)
    {
        int symbol = fgetc(finput);
        int symbolCount = 0;

        str = (char*)calloc(1, sizeof(char));
        str[symbolCount] = (char)symbol;
        symbolCount++;
        symbol = fgetc(finput);

        while (symbol != '\n' && symbol != EOF)
        {
            str = (char*)realloc(str, (symbolCount + 1)*sizeof(char));
            assert(str != nullptr);

            str[symbolCount] = (char)symbol;
            symbolCount++;
            symbol = fgetc(finput);
        }

        str = (char*)realloc(str, (symbolCount + 1)*sizeof(char));
        assert(str != nullptr);

        str[symbolCount] = '\0';
        lines[i].lineNumber = i + 1;
        lines[i].str = str;
    }
}

void moveToNextLine(FILE *foutput)
{
    assert(foutput != nullptr);

    fputc('\n', foutput);
}

void writeFile(Line lines[], int linesCount, FILE *foutput)
{
    assert(foutput != nullptr);

    foutput = fopen(OUTPUT_FILE, "w");

    fputs("qsort\n", foutput);
    qsort(lines, 0, linesCount - 1, (int (*)(void *, void *))(myStrcmp), true);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("reverseQsort\n", foutput);
    qsort(lines, 0, linesCount - 1, (int (*)(void *, void *))(reverseStrcmp), true);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("OriginalText\n", foutput);
    qsort(lines, 0, linesCount - 1, (int (*)(void *, void *))(compareNumbers), false);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }
}

void swap(Line lines[], int i, int j)
{
    Line buffer = lines[i];
    lines[i] = lines[j];
    lines[j] = buffer;
}

bool isPunctuationMark(int symbol)
{
    return strchr(PUNCTUATION_MARKS, symbol) != nullptr;
}

int myStrcmp(char *str1, char *str2)
{
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    char *ptrStr1 = str1;
    char *ptrStr2 = str2;

    for ( ; *ptrStr1 == *ptrStr2; ptrStr1++, ptrStr2++)
    {
        if (isPunctuationMark(*ptrStr1))
        {
            ptrStr1++;
            continue;
        }

        if (isPunctuationMark(*ptrStr2))
        {
            ptrStr2++;
            continue;
        }

        if (*ptrStr1 == '\0' || *ptrStr2 == '\0')
        {
            return ((int)strlen(str1) - (int)strlen(str2));
        }
    }

    return (*ptrStr1 - *ptrStr2);
}

int reverseStrcmp(char *str1, char *str2)
{
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    int numberUnreadCharactersStr1 = (int)strlen(str1) - 1;
    int numberUnreadCharactersStr2 = (int)strlen(str2) - 1;

    while (numberUnreadCharactersStr1 >= 0 && numberUnreadCharactersStr2 >= 0)
    {
        if (isPunctuationMark(str1[numberUnreadCharactersStr1]))
        {
            numberUnreadCharactersStr1--;
            continue;
        }

        if (isPunctuationMark(str2[numberUnreadCharactersStr2]))
        {
            numberUnreadCharactersStr2--;
            continue;
        }

        if (str1[numberUnreadCharactersStr1] != str2[numberUnreadCharactersStr2])
        {
            return (str1[numberUnreadCharactersStr1] - str2[numberUnreadCharactersStr2]);
        }

        numberUnreadCharactersStr1--;
        numberUnreadCharactersStr2--;
    }

    return (numberUnreadCharactersStr1 - numberUnreadCharactersStr2);
}

int compareNumbers(int *number1, int *number2)
{
    assert(number1 != nullptr);
    assert(number2 != nullptr);

    return (*number1 - *number2);
}

void qsort(Line lines[], int left, int right, int (*strCompare)(void *, void *), bool isStrCmp)
{
    assert(strCompare != nullptr);

    int pivot = left;

    if (left >= right)
        return;

    swap(lines, left, (left + right) / 2);

    for (int i = left + 1; i <= right; i++)
    {
        if (isStrCmp ? ((*strCompare)(lines[i].str, lines[left].str) < 0) :
                       ((*strCompare)(&lines[i].lineNumber, &lines[left].lineNumber) < 0))
        {
            pivot++;
            swap(lines, pivot, i);
        }
    }

    swap(lines, left, pivot);
    qsort(lines, left, pivot - 1, strCompare, isStrCmp);
    qsort(lines, pivot + 1, right, strCompare, isStrCmp);
}
