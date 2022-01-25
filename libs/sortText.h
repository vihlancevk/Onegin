#ifndef SORT_TEXT_H_
#define SORT_TEXT_H_

void swap(void *elem1, void *elem2, size_t struct_size);
bool isPunctuationMark(int symbol);
void *skipPunctuationMark(char *str, char *endStr, bool isReverse);
int compareLine(const Line *line1,  const Line *line2);
int compareLineReverse(const Line *line1, const Line *line2);
void qsort(void *elems, size_t struct_size, int left, int right,
           int (*LineCompare)(const void *elem1, const void *elem2));

#endif 
