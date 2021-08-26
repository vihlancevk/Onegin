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
bool isPunctuationMark(int symbol);
void readFile(Line lines[], int linesCount , FILE *finput);
void moveToNextLine(FILE *foutput);
void writeFile(Line lines[], int linesCount, FILE *foutput);
void swap(Line lines[], int i, int j);
int myStrcmp(char *str1, char *str2);
void qsort(Line lines[], int left, int right);
int reverseStrcmp(char *str1, char *str2);
void reverseQsort(Line lines[], int left, int right);
void restoreOriginalText(Line lines[], int left, int right);

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

bool isPunctuationMark(int symbol)
{
    return strchr(PUNCTUATION_MARKS, symbol) != nullptr;
}

void readFile(Line lines[], int linesCount, FILE *finput)
{
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
    fputc('\n', foutput);
}

void writeFile(Line lines[], int linesCount, FILE *foutput)
{
    foutput = fopen(OUTPUT_FILE, "w");

    fputs("qsort\n", foutput);
    qsort(lines, 0, linesCount - 1);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("reverseQsort\n", foutput);
    reverseQsort(lines, 0, linesCount - 1);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("OriginalText\n", foutput);
    restoreOriginalText(lines, 0, linesCount - 1);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }
}

int myStrcmp(char *str1, char *str2)
{
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

void swap(Line lines[], int i, int j)
{
    Line buffer = lines[i];
    lines[i] = lines[j];
    lines[j] = buffer;
}

void qsort(Line lines[], int left, int right)
{
    int pivot = left;

    if (left >= right)
        return;

    swap(lines, left, (left + right) / 2);

    for (int i = left + 1; i <= right; i++)
    {
        if (myStrcmp(lines[i].str, lines[left].str) < 0)
        {
            pivot++;
            swap(lines, pivot, i);
        }
    }

    swap(lines, left, pivot);
    qsort(lines, left, pivot - 1);
    qsort(lines, pivot + 1, right);
}

int reverseStrcmp(char *str1, char *str2)
{
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

void reverseQsort(Line lines[], int left, int right)
{
    int pivot = left;

    if (left >= right)
        return;

    swap(lines, left, (left + right) / 2);

    for (int i = left + 1; i <= right; i++)
    {
        if (reverseStrcmp(lines[i].str, lines[left].str) < 0)
        {
            pivot++;
            swap(lines, pivot, i);
        }
    }

    swap(lines, left, pivot);
    reverseQsort(lines, left, pivot - 1);
    reverseQsort(lines, pivot + 1, right);
}

void restoreOriginalText(Line lines[], int left, int right)
{
    int pivot = left;

    if (left >= right)
        return;

    swap(lines, left, (left + right) / 2);

    for (int i = left + 1; i <= right; i++)
    {
        if (lines[i].lineNumber < lines[left].lineNumber)
        {
            pivot++;
            swap(lines, pivot, i);
        }
    }

    swap(lines, left, pivot);
    restoreOriginalText(lines, left, pivot - 1);
    restoreOriginalText(lines, pivot + 1, right);
}
