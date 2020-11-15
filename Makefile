sorter: sorter-ru sorter-en

sorter-ru:
	gcc -c main.c -Wall -Wextra -D RU
	gcc -o sorter-ru main.o text/text.o    

sorter-en:
	gcc -c main.c -Wall -Wextra -D EN
	gcc -o sorter-en main.o text/text.o    

clean:
	rm *.o
