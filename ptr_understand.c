#include <stdio.h>

void func_fail(char* p){
    printf("\n\nbelow is info of func_fail\n");
    printf("ptr passed is : %d\n", *p);
    printf("ptr passed is : %d\n", (int)p[0]);
    p =    "lose you";
    printf("called ptr's addr is: %p\n", (void*)&p);
    printf("addr stored by ptr is %p\n", (void*)p);
    printf("                      %p\n", (void*)&p[0]);
    printf("\n\n\n");
}

void func_well(char **p){
    *p = "losa you";   //解引用了过后是一个新的entity，当然可以赋值
}                      //所有的指针声明时候， (xxx)*** p, 前面我们都不要看,只是标签
                       //选取一个*看，那是其他实体的地址
                       //去掉一个*看，(xxx)**,那是p存的东西

int main() {
    char* p = "find you";
    printf("origin addr is : %p\n", (void*)&p);
    printf("origin content is : %s\n", p);
    func_fail(p);
    func_well(&p);
    printf("after content is: %s\n", p );
}
