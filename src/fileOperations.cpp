#include "fileOperations.h"

//================================================================================
//! @brief ������� �������� ������� ����� � ������.
//!
//! @param [in] finput ����, ������ �������� ���������.
//!
//! @warning � ������ ������ ��� ������ � ������ ����� ��������
//!          ��������������� ��������� � ��������� ������ ���������.
//!
//! @return ������ ����� � ������.
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
//! @brief ������� ���������� ����� � �����.
//!
//! @param [in] finput ����, ������ �������� �����������.
//! @param [out] str �����, � ������� ���������� ����������.
//! @param [in] numberBytesFile ������ ����� finput � ������.
//!
//! @warning � ������ ������ ��� ������ � ������ ����� ��������
//!          ��������������� ��������� � ��������� ������ ���������.
//!
//! @return ��������� �� �����.
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
//! @brief ������� �������� ���������� ����� � ������.
//!
//! @return [in] str ����� � �������.
//!
//! @return ���������� ����� � ������.
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
//! @brief ������� ���������� ������ �� ������.
//!
//! @return [out] lines ������ �������� Line.
//! @return [in] linesCount ���������� ����� � ������.
//! @return [in] str ����� � �������.
//!
//! @note � ������ �������� lines ���������� ���������� ������ ������.
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
//! @brief ������� �������� ������� ��������� � ����� ������ �� ����� ������.
//!
//! @param [in] foutput ����, � ������� ����� ����������� ������� ���������
//!             �� ����� ������.
//--------------------------------------------------------------------------------

void moveToNextLine(FILE *foutput)
{
    assert(foutput != nullptr);

    fputs("\n", foutput);
}

//================================================================================
//! @brief ������� ������ ���������� � ������ � ����.
//!
//! @param [in] lines ������ �������� Line, � ������� �������� �����.
//! @param [in] linesCount ���������� ����� � ������.
//! @param [in] foutput ����, � ������� ���������� ������.
//! @param [in] nameSort �������� ���������� ��� ������.
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
