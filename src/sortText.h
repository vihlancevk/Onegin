#ifndef SORTTEXT_H_
#define SORTTEXT_H_

void swap(void *elems, size_t struct_size, int i, int j);
bool isPunctuationMark(int symbol);
void *skipPunctuationMark(char *str, char *endStr, bool isReverse);
int compareStr(char **str1_ptr, char **str2_ptr);
int compareStrReverse(char **str1_ptr, char **str2_ptr);
int compareNumbers(int *number1, int *number2);
void qsort(void *elems, size_t struct_size,  size_t offset, int left, int right, int (*strCompare)(void *, void *));

#endif
