#include "mmemory.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_initialization()
{
	const int NUMBER_OF_PAGES = 8;
	const int PAGE_SIZE = 64;
	int initResult = _init (NUMBER_OF_PAGES, PAGE_SIZE);
	printf("%s = %d\n", "initResult", initResult);
}

void printq(char *s, int sz)
{
	int i;
	printf("\"");
	for (i = 0; i < sz; i++)
	{
		printf("%c", s[i]);
	}
	printf("\"\n");
}

void test_program()
{
	const int NUMBER_OF_PAGES = 8;
	const int PAGE_SIZE = 64;
	int initResult = _init (NUMBER_OF_PAGES, PAGE_SIZE);

	VA ts1, ts2, ts3, ts4, ts5, ts6, ts7;

	_malloc(&ts1, 21);
	_malloc(&ts2, 17);
	_malloc(&ts3, 26);
	_malloc(&ts4, 16);
	_malloc(&ts5, 37);
	_malloc(&ts6, 41);
	_malloc(&ts7, 38);

	_write(ts1, "Forgive me, cousin! ", 21);
	_write(ts2, "Ah, dear Juliet,\n", 17);
	_write(ts3, "Why art thou yet so fair? ", 26);
	_write(ts4, "Shall I believe\n", 16);
	_write(ts5, "That unsubstantial death is amorous,\n", 37);
	_write(ts6, "And that the lean abhorred monster keeps\n", 41);
	_write(ts7, "Thee here in dark to be his paramour?\n", 38);

	_printBuffer(ts1, 21);
	_printBuffer(ts2, 17);
	_printBuffer(ts3, 26);
	_printBuffer(ts4, 16);
	_printBuffer(ts5, 37);
	_printBuffer(ts6, 41);
	_printBuffer(ts7, 38);

	printf("\nTest address arithmetic:\n");

	_read(ts7, ts1, 10);	// "Thee here , cousin! "
	ts1 += 10;				// ", cousin! "
	_read(ts2, ts1, 9);		// "Ah, dear  "
	ts1 += 9;				// " "
	_write(ts1, "!", 1);	// "!"
	ts1 -= 19;				// "Thee here Ah, dear !"
	_printBuffer(ts1, 21);
	
	_free(ts1);
	_free(ts2);
	_free(ts3);
	_free(ts4);
	_free(ts5);
	_free(ts6);
	_free(ts7);
}

void main()
{
	test_program();
	_getch();
}