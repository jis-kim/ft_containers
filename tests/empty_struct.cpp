#include <iostream>
using namespace std;
struct X {
	int a;
};

void f(X* p)
{
	void* p1 = p;
	void* p2 = &p->a;
	if (p1 == p2) cout << "nice: good optimizer";
} 
int main(void) {
	X a;
f(&a);
}
