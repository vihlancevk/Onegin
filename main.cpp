//================================================================================
//!
//--------------------------------------------------------------------------------

#include "txlib.h"

struct Line {
    int lineNumber;
    char *str;
};
int getNumbersBytesFile(FILE *finput);
void readFileStr(FILE *finput, char *str, int numberBytesSize);
int countNumberLines(FILE *finput);
void readFile(Line lines[], int linesCount , FILE *finput);
void moveToNextLine(FILE *foutput);
void writeFile(Line lines[], int linesCount, FILE *foutput);
void swap(void *lines, size_t struct_size, int i, int j);
bool isPunctuationMark(int symbol);
int myStrcmp(char **str1_ptr, char **str2_ptr);
int reverseStrcmp(char **str1_ptr, char **str2_ptr);
int compareNumbers(int *number1, int *number2);
void qsort(void *lines, size_t struct_size,  size_t offset, int left, int right, int (*strCompare)(void *, void *));

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

int getNumbersBytesFile(FILE *finput)
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

    return numberBytesFile;
}

void readFileStr(FILE *finput, char *str, int numberBytesFile)
{
    assert(finput != nullptr);
    assert(str != nullptr);

    if ((fread(str, sizeof(char), numberBytesFile, finput) != numberBytesFile) && !feof(finput))
    {
        printf("Error fread\n");
        abort();
    }
}

//================================================================================

//--------------------------------------------------------------------------------

int countNumberLines(FILE *finput)
{
    assert(finput != nullptr);

    int numberBytesFile = getNumbersBytesFile(finput);

    char *str = (char*)calloc(numberBytesFile, sizeof(char));
    readFileStr(finput, str, numberBytesFile);

    int linesCount = 0;

    for (int i = 0; i < numberBytesFile; i++)
    {
        if (str[i] == '\n')
            linesCount++;

        if (str[i] == EOF)
            break;
    }

    rewind(finput);

    free(str);

    return (linesCount + 1);
}

//================================================================================

//--------------------------------------------------------------------------------

void readFile(Line lines[], int linesCount, FILE *finput)
{
    assert(finput != nullptr);

    char *fileStr = nullptr;
    char *str = nullptr;

    int numberBytesFile = getNumbersBytesFile(finput);
    fileStr = (char*)calloc(numberBytesFile, sizeof(char));
    readFileStr(finput, fileStr, numberBytesFile);

    for (int i = 0, j = 0; i < linesCount; i++)
    {
        int symbol = fileStr[j++];
        int symbolCount = 0;

        str = (char*)calloc(1, sizeof(char));
        str[symbolCount++] = (char)symbol;
        symbol = fileStr[j++];

        while (symbol != '\n' && symbol != EOF)
        {
            str = (char*)realloc(str, (symbolCount + 1)*sizeof(char));
            assert(str != nullptr);

            str[symbolCount++] = (char)symbol;
            symbol = fileStr[j++];
        }

        str = (char*)realloc(str, (symbolCount + 1)*sizeof(char));
        assert(str != nullptr);

        str[symbolCount++] = '\0';
        lines[i].lineNumber = i + 1;
        lines[i].str = str;
    }
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void moveToNextLine(FILE *foutput)
{
    assert(foutput != nullptr);

    fputc('\n', foutput);
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void writeFile(Line lines[], int linesCount, FILE *foutput)
{
    assert(foutput != nullptr);

    foutput = fopen(OUTPUT_FILE, "w");

    fputs("qsort\n", foutput);
    qsort(lines, sizeof(Line), offsetof(Line, str), 0, linesCount - 1, (int (*)(void *, void *))(myStrcmp));

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("reverseQsort\n", foutput);
    qsort(lines, sizeof(Line), offsetof(Line, str), 0, linesCount - 1, (int (*)(void *, void *))(reverseStrcmp));

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }

    moveToNextLine(foutput);

    fputs("OriginalText\n", foutput);
    qsort(lines, sizeof(Line), offsetof(Line, lineNumber), 0, linesCount - 1, (int (*)(void *, void *))(compareNumbers));

    for (int i = 0; i < linesCount; i++)
    {
        fputs(lines[i].str, foutput);
        moveToNextLine(foutput);
    }
}

//================================================================================
//!
//--------------------------------------------------------------------------------

void swap(void *lines, size_t struct_size, int i, int j)
{

    void *buffer = calloc(1, struct_size);
    memcpy(buffer, (lines + i * struct_size), struct_size);

    memcpy((lines + i * struct_size), (lines + j * struct_size), struct_size);
    memcpy((lines + j * struct_size), buffer, struct_size);

    free(buffer);
}

bool isPunctuationMark(int symbol)
{
    return strchr(PUNCTUATION_MARKS, symbol) != nullptr;
}

//================================================================================
//!
//--------------------------------------------------------------------------------

int myStrcmp(char **str1_ptr, char **str2_ptr)
{
    assert(str1_ptr != nullptr);
    assert(str2_ptr != nullptr);

    char* str1 = *str1_ptr;
    char* str2 = *str2_ptr;

    assert(str1 != nullptr);
    assert(str2 != nullptr);

    char *ptrStr1 = str1;
    char *ptrStr2 = str2;

    for ( ; *ptrStr1 == *ptrStr2; )
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

        ptrStr1++;
        ptrStr2++;
    }

    return (*ptrStr1 - *ptrStr2);
}

//================================================================================
//!
//--------------------------------------------------------------------------------

int reverseStrcmp(char **str1_ptr, char **str2_ptr)
{
    assert(str1_ptr != nullptr);
    assert(str2_ptr != nullptr);

    char* str1 = *str1_ptr;
    char* str2 = *str2_ptr;

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

void qsort(void *lines, size_t struct_size,  size_t offset, int left, int right, int (*strCompare)(void *, void *))
{
    assert(strCompare != nullptr);

    int pivot = left;

    if (left >= right)
        return;

    swap(lines, struct_size, left, (left + right) / 2);

    for (int i = left + 1; i <= right; i++)
    {
        if ((*strCompare)(((lines + i*struct_size) + offset), ((lines + left*struct_size) + offset)) < 0)
        {
            pivot++;
            swap(lines, struct_size, pivot, i);
        }
    }

    swap(lines, struct_size, left, pivot);
    qsort(lines, struct_size, offset, left, pivot - 1, strCompare);
    qsort(lines, struct_size, offset, pivot + 1, right, strCompare);
}
