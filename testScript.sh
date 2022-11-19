#!/bin/bash
for num in '0,5x' '2x' '4x'
do
    for arq in 'GEN_PROC_COELHOS' 'GEN_PROC_RAPOSA' 'GEN_COMIDA_RAPOSA' 'N_GEN' 'L_C' 'N'
    do
        for i in {1..10}
        do
            ./tp2 < ./testes/teste_${arq}_$num.txt
        done
    done
done