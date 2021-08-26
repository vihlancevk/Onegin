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
void Qsort(Line lines[], int left, int right);
int reverseStrcmp(char *str1, char *str2);
void reverseQsort(Line lines[], int left, int right);
void returnToOriginalText(Line lines[], int left, int right);

const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";
const char *PUNCTUATION_MARKS = " ,.!?:-;\"(){}<>";

int main()
{
    FILE *finput = nullptr;

    int linesCount = countNumberLines(finput);
    Line lines[linesCount];

    //printf("linesCount: %d\n", linesCount);

    for (int i = 0; i < linesCount; i++)
    {
        lines[i].str = nullptr;
    }

    readFile(lines, linesCount, finput);

    FILE *foutput = nullptr;

    writeFile(lines, linesCount, foutput);

    return 0;
}

int countNumberLines(FILE *finput)
{
    finput = fopen(INPUT_FILE, "r");

    assert(finput != nullptr);

    int linesCount = 0;
    int symbol = fgetc(finput);

    while (symbol != EOF)
    {
        if (symbol == '\n')
            linesCount++;

        symbol = fgetc(finput);
    }

    fclose(finput);

    return (linesCount + 1);
}

bool isPunctuationMark(int symbol)
{
    return strchr(PUNCTUATION_MARKS, symbol) != nullptr;
}

void readFile(Line lines[], int linesCount, FILE *finput)
{
    finput = fopen(INPUT_FILE, "r");

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

    fclose(finput);
}

void moveToNextLine(FILE *foutput)
{
    fputc('\n', foutput);
}

void writeFile(Line lines[], int linesCount, FILE *foutput)
{
    foutput = fopen(OUTPUT_FILE, "w");

    fputs("Qsort\n", foutput);
    Qsort(lines, 0, linesCount - 1);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("ReverseQsort\n", foutput);
    reverseQsort(lines, 0, linesCount - 1);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("OriginalText\n", foutput);
    returnToOriginalText(lines, 0, linesCount - 1);

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    fclose(foutput);
}

int myStrcmp(char *str1, char *str2)
{
    int lenStr1 = strlen(str1);
    int lenStr2 = strlen(str2);
    int i = 0, j = 0;

    while (i < lenStr1 && j < lenStr2)
    {
        if (isPunctuationMark(str1[i]))
        {
            i++;
            continue;
        }

        if (isPunctuationMark(str2[i]))
        {
            j++;
            continue;
        }

        if (str1[i] != str2[j])
        {
            return (str1[i] - str2[j]);
        }

        i++;
        j++;
    }

    return (lenStr1 - lenStr2);
}

void swap(Line lines[], int i, int j)
{
    Line buffer = lines[i];
    lines[i] = lines[j];
    lines[j] = buffer;
}

void Qsort(Line lines[], int left, int right)
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
    Qsort(lines, left, pivot - 1);
    Qsort(lines, pivot + 1, right);
}

int reverseStrcmp(char *str1, char *str2)
{
    int ptrSymbolStr1 = (int)strlen(str1) - 1;
    int ptrSymbolStr2 = (int)strlen(str2) - 1;

    while (ptrSymbolStr1 >= 0 && ptrSymbolStr2 >= 0)
    {
        if (isPunctuationMark(str1[ptrSymbolStr1]))
        {
            ptrSymbolStr1--;
            continue;
        }

        if (isPunctuationMark(str2[ptrSymbolStr2]))
        {
            ptrSymbolStr2--;
            continue;
        }

        if (str1[ptrSymbolStr1] != str2[ptrSymbolStr2])
        {
            return (str1[ptrSymbolStr1] - str2[ptrSymbolStr2]);
        }

        ptrSymbolStr1--;
        ptrSymbolStr2--;
    }

    return (ptrSymbolStr1 - ptrSymbolStr2);
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

void returnToOriginalText(Line lines[], int left, int right)
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
    returnToOriginalText(lines, left, pivot - 1);
    returnToOriginalText(lines, pivot + 1, right);
}
