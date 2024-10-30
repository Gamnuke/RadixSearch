all : dict2 dict3

dict2 : stage_2.c dynstring.c node.c processors.c radix.c array.c
	gcc -o dict2 stage_2.c dynstring.c node.c processors.c radix.c array.c

dict3 : stage_3.c dynstring.c node.c processors.c radix.c array.c
	gcc -o dict3 stage_3.c dynstring.c node.c processors.c radix.c array.c