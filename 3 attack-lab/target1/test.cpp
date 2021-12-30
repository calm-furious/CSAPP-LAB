#include <cstdio>
#include <iostream>


using namespace std;

int main(){
    char *s = new char[20];
    char *p = "\372\227\271Y";
    unsigned int a = *((int*)p);
    sprintf(s,"%.8x",a);
    cout<<s;
}