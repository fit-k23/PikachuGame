//#include <stdio.h>
//
//// Library where getch() is stored
//#include <conio.h>
//
//int main() {
//	while (true) {
//		char input = getch();
//		printf("%c - %d\n", input, input);
//	}
//	return 0;
//}

#include <iostream>
#include <cstdio>
#include <conio.h>

int main () {
	int ch;

	while ((ch = getch()) != 27) /* 27 = Esc key */{
		printf("%d", ch);
		if (ch == 0 || ch == 224)
			printf (", %d", getch());
		printf("\n");
	}

	printf("ESC %d\n", ch);
	system("pause");
	return (0);
}