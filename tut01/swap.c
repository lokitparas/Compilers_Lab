/*swap.c*/
extern int buf []; /*declaration buf*/
#define one 1
int *bufp0 = &buf[0]; /* initialized global */
int *bufp1;
void swap () {
/* uninitialized global */
 /* definition swap */
 /* local */
int temp;
f();
bufp1 = &buf[one];
temp = *bufp0;
*bufp0 = *bufp1;
*bufp1 = temp;
}