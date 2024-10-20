#include<stdio.h>
#define a(x,y) x##y
#define b(x) #x
#define c(x) b(x)
#define expand_and_stringify(x) c(x)
int main() {
	printf("%s\n", c(a(sachin, tendulkar)));
	printf("%s\n", expand_and_stringify(a(22,55)));
	printf("%s\n", b(!8k* ));
	return 0;
}
