test_sec:
	gcc sequencial.c objeto.c mundo.c
	./a.out < ./testes/teste.txt
	rm a.out

test_prl:
	gcc sequencial.c objeto.c mundoParalelo.c -fopenmp
	./a.out < ./testes/teste.txt
	rm a.out

sec: 
	gcc -o tp2 sequencial.c objeto.c mundo.c

prl: 
	gcc -o tp2 sequencial.c objeto.c mundoParalelo.c -fopenmp

debug_sec:
	gcc -g sequencial.c objeto.c mundo.c
	gdb a.out
	rm a.out

debug_prl:
	gcc -g sequencial.c objeto.c mundoParalelo.c -fopenmp
	gdb a.out
	rm a.out