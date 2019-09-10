#include <stdio.h>
#include <kipr/util.h>
int main()
{
    int i;
    printf("try to stop 10 second countdown\n");
    for (i=10; i>=0; i--) {
        printf("%d  ",i);
        msleep(1000);
    }
    printf("\ndone\n");  
    return 0;
}
