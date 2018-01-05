





r3: regex_3_sat.c
	gcc -o r3 regex_3_sat.c
	./r3 | tee r3.txt 
	cat r3.txt | grep Pattern > r3_pat.txt
	vi r3_pat.txt
	vi r3.txt 


clean:
	rm r3
