#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#define smallN 46440
#define bigN 46550
#define MEM 100000000

static int memory[MEM];
static unsigned int pos=0;
/* returns size chars */
void *my_malloc(int size) {
   void *result=memory+pos;
   while (size&3) size++;
   size>>=2;
   pos+=size;
   if (pos>MEM) {
      fprintf(stderr, "Memory overflow %d\n", pos);
      exit(1);
   }
   return result;
}

void my_free(int new_pos) {
   fprintf(stderr, "Maximum memory allocated: %d\n", pos);
   printf("Maximum memory allocated: %d\n", pos);
   pos=new_pos;
   return;
}

void my_heap_size() {
   fprintf(stderr, "Current heap size: %d\n", pos);
   return;
}

void *my_calloc(unsigned int nelem, size_t size) {
   void *result=memory+pos;
   size*=nelem;
   while (size&3) size++;
   size>>=2;
   pos+=size;
   if (pos>MEM) {
      fprintf(stderr, "Memory overflow %d\n", pos);
      exit(1);
   }
   memset(result, 0, size<<2);
   return result;
}

typedef struct Line_tag {
   int *values;
   int *non_0_values;
   int length;
   int non_0_length;
   int actual_length;
} *Line;

typedef struct Matrix_tag {
   int lines_no;
   int max_column;
   Line *lines;
} *Matrix;

void delete_Line(Line l) {
   //if (l!=NULL)
   //   free(l->values);
   //free(l);
   l=l;
   return;
}

void delete_Matrix(Matrix *M) {
   //int i;
   //for (i=0; i<(*M)->lines_no; i++)
   //   delete_Line((*M)->lines[i]);
   //free((*M)->lines);
   //free(*M);
   my_free(0);
   *M=NULL;
   return;
}

int mydiv(int a, int b, int c) {
   double aux;
   int result;
   aux=((1.0*a)*b)/(c*1.0);
   result=aux;
   //printf("cat=%d %.5f\n", result, aux);
   aux=aux-(result*1.0);
   aux=aux*(c*1.0);
   //printf("%d %d %d %.5f %d\n", a, b, c, aux, result=aux+0.1);
   result=aux+0.1;
   return result;
}

/* b^e (mod p) in a time of O(log e) */
int raise(int b, int e, int p) {
   int power=b, result=1;
   float aux;
   while (e) {
      if (e&1) {
         result=mydiv(result, power, p);
      }
      power=mydiv(power, power, p);
      //printf("%d \n", power);
      e>>=1;
   }
   return result;
}

/* finds out 4k+1 primes smaller than n */
int *sieve(int n) {
   char *array;
   int *result;
   int i, j, sqrtn=sqrt(n*1.0);
   int count=0;
   assert(array=calloc((unsigned)n, sizeof(char)));
   for (i=2; i<=sqrtn; i++)
      if (array[i]==0)
         for (j=i*i; j<n; j+=i)
            array[j]=1;
   for (i=2, count=0; i<n; i++)
      if ((array[i]==0)&&((i&3)==1))
         count++;
      else
         array[i]=1;
   assert(result=malloc((count+2)*sizeof(int)));
   result[0]=count;
   result[count+1]=0;
   for (i=2, j=1; i<n; i++)
      if (array[i]==0) {
         result[j]=i;
         j++;
      }
   free(array);
   return result;
}

int root(int p) {
   int i;
   for (i=1; i<p; i++)
      if (((i*i)%p)==(p-1))
         return i;
   return 0;
}

/* finds root^2=-1 (mod p) in a sufficiently small time */
int better_root(int p) {
   static int prime_list[]={2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
   int i, p4=p>>2, result;
   for (i=0; i<10; i++) {
      result=raise(prime_list[i], p4, p);
      if (mydiv(result, result, p)!=1) {
         assert(mydiv(result, result, p)==(p-1));
         //printf("%d %d %d %d\n",
         //       result, mydiv(result, result, p), p, prime_list[i]);
         return result;
      }
   }
   printf("Error in finding the root.\n");
   exit(-1);
   return 0;
}

int *find_inverses(int p) {
   int *result, *power;
   int i, j, pow;
   assert(result=calloc((unsigned)p+1, sizeof(int)));
   result[0]=p;
   result[1]=1;
   assert(power=malloc((p-1)*sizeof(int)));
   for (i=1; i<p; i++) {
      pow=1; j=0;
      do {
         power[j]=pow;
         j++;
         pow=(pow*i)%p;
      } while (pow!=1);
      if (j==(p-1)) {
         for (j=1; j<(p-1); j++)
            result[power[j]]=power[p-1-j];
         free(power);
         return result;
      }
   }
   free(power);
   return result;
}

Line new_node(int limit) {
   Line result;
   assert(result=my_malloc(sizeof(struct Line_tag)));
   assert(result->values=my_calloc(8, sizeof(int)));
   assert(result->non_0_values=my_calloc(9, sizeof(int)));
   result->length=limit;
   result->non_0_length=0;
   result->actual_length=8;
   return result;
}

Line add_value(int a, int val_a, int limit, Line l) {
   int i;
   if ((a<2)||(a>=limit))
      return l;
   if (l==NULL) {
	   l=new_node(limit);
	   l->values[0]=val_a;
      l->non_0_values[0]=a;
      l->non_0_values[1]=-1;
      l->non_0_length++;
	   return l;
   }
   if (l->non_0_values[1]==-1) {
      if (a==l->non_0_values[0])
         return l;
      l->values[1]=val_a;
      l->non_0_values[1]=a;
      l->non_0_values[2]=-1;
      l->non_0_length++;
   }
   else if (l->non_0_values[2]==-1) {
      if (a==l->non_0_values[1])
         return l;
      l->values[2]=val_a;
      l->non_0_values[2]=a;
      l->non_0_values[3]=-1;
      l->non_0_length++;
   }
   else assert(0);
   for (i=0; i<l->non_0_length; i++) {
      //printf("%d %d %d\n", i1, temp_non_0[i1], temp_val[i1]);
      //printf("%d %d %d\n", i1+1, temp_non_0[i1+1], temp_val[i1+1]);
      assert((l->non_0_values[i]<l->non_0_values[i+1])||(l->non_0_values[i+1]==-1));
      assert(l->values[i]);
   }
   return l;
}

void sort2(int *a, int *b) {
   static int c;
   if (*a<*b)
      return;
   c=*a; *a=*b; *b=c;
   return;
}

void sort3(int *a, int *b, int *c) {
   static int d;
   if (*a<*b) {
      if (*b<*c)
         return;
      if (*c<*a) {
         d=*a; *a=*c; *c=*b; *b=d;
         return;
      }
      d=*b; *b=*c; *c=d;
      return;
   }
   if (*c<*b) {
      d=*a; *a=*c; *c=d;
      return;
   }
   if (*c>*a) {
      d=*a; *a=*b; *b=d;
      return;
   }
   d=*a; *a=*b; *b=*c; *c=d;
   return;
}

Line make_line2(int a, int val_a, int b, int val_b, int limit) {
   sort2(&b, &a);
   return add_value(a, val_a, limit, add_value(b, val_b, limit, NULL));
}

Line make_line3(int a, int val_a, int b, int val_b, int c, int val_c,
                int limit) {
   sort3(&c, &b, &a);
   return add_value(a, val_a, limit,
                    add_value(b, val_b, limit,
                              add_value(c, val_c, limit, NULL)));
}

static int op_no=0;

Line substract2(Line l1, Line l2, int m) {
   static int temp_non_0[bigN], temp_val[bigN];
   int i1, i2, j, a, b, c;
   int temp;
   i1=0; i2=0; j=0;
   a=l1->values[0];
   assert(a);
   b=l2->values[0];
   assert(b);
   assert(l1->non_0_values[0]==l2->non_0_values[0]);
   c=l1->non_0_values[0];
   
   while ((l1->non_0_values[i1]!=-1)&&(l2->non_0_values[i2]!=-1)) {
      if (l1->non_0_values[i1]<l2->non_0_values[i2]) {
         temp_non_0[j]=l1->non_0_values[i1];
         temp_val[j]=(b*l1->values[i1])%m;
         i1++; j++; op_no++;
      }
      else if (l1->non_0_values[i1]>l2->non_0_values[i2]) {
         temp_non_0[j]=l2->non_0_values[i2];
         temp_val[j]=((-a)*l2->values[i2])%m;
         i2++; j++; op_no++;
      }
      else {
         temp=(l1->values[i1]*b-
              l2->values[i2]*a)%m;
         if (temp!=0) {
            temp_non_0[j]=l1->non_0_values[i1];
            temp_val[j]=temp;
            j++;
         }
         i1++; i2++; op_no++;
      }
   }
   while (l1->non_0_values[i1]!=-1) {
      temp_non_0[j]=l1->non_0_values[i1];
      temp_val[j]=(b*l1->values[i1])%m;
      i1++; j++; op_no++;
   }
   while (l2->non_0_values[i2]!=-1) {
      temp_non_0[j]=l2->non_0_values[i2];
      temp_val[j]=((-a)*l2->values[i2])%m;
      i2++; j++; op_no++;
   }
   if (j>l1->actual_length) {
      do
         l1->actual_length<<=2;
      while (j>l1->actual_length);
      l1->values=my_calloc((unsigned)l1->actual_length, sizeof(int));
      l1->non_0_values=my_calloc((unsigned)l1->actual_length+1, sizeof(int));
   }

   if (j==0)
      return NULL;

   l1->non_0_values[j]=-1;
   l1->non_0_length=j;
   
   for (j=0; j<l1->non_0_length; j++) {
      l1->non_0_values[j]=temp_non_0[j];
      l1->values[j]=temp_val[j];
      
   }
   return l1;
}

void print_Matrix(Matrix M);

typedef struct Heap_tag {
   Line *heap;
   int heap_size;
   int (*comp)(Line a, Line b);
} *Heap; 

int comp(Line a, Line b) {
   if ((a==NULL)||(b==NULL)) {
      if ((a==NULL)&&(b==NULL))
         return 0;
      if (a==NULL)
         return 1;
      return -1;
   }
   assert(a->non_0_length);
   assert(b->non_0_length);
   if (a->non_0_values[0]!=b->non_0_values[0])
      return a->non_0_values[0]-b->non_0_values[0];
   return a->non_0_length-b->non_0_length;
}

void Heapify(Heap h, int heap_pos) {
   Line ch1, ch2, p, min;
   if ((heap_pos<<1)<=h->heap_size)
      ch1=h->heap[heap_pos<<1];
   else
      ch1=NULL;
   if (((heap_pos<<1)|1)<=h->heap_size)
      ch2=h->heap[(heap_pos<<1)|1];
   else
      ch2=NULL;
   p=h->heap[heap_pos];
   min=h->comp(ch1, ch2)<0? ch1: ch2;
   if (h->comp(p, min)>0) {
      h->heap[heap_pos]=min;
      if (min==ch1) {
         h->heap[heap_pos<<1]=p;
         Heapify(h, heap_pos<<1);
      }
      else {
         h->heap[(heap_pos<<1)|1]=p;
         Heapify(h, (heap_pos<<1)|1);
      }
   }
   //printf("\n");
   return;
}

Heap Heap_init(Line *array, int heap_size, int (*comp)(Line a, Line b)) {
   int i;
   Heap result;
   assert(result=my_malloc(sizeof(struct Heap_tag)));
   result->heap=array-1;
   result->heap_size=heap_size;
   result->comp=comp;
   for (i=heap_size>>1; i>1; i--)
      Heapify(result, i);
   return result;
}

int rank_Matrix2(Matrix M, int m) {
   int i, result=0;
   Line l;
   Heap h=Heap_init(M->lines, M->lines_no, comp);
   for (i=2; i<M->max_column; i++) {
      l=NULL;
      while ((M->lines[0]!=NULL)&&(M->lines[0]->non_0_values[0]==i)) {
         if (l==NULL) {
            l=M->lines[0];
            M->lines[0]=NULL;
            Heapify(h, 1);
         }
         else {
            M->lines[0]=substract2(M->lines[0], l, m);
            Heapify(h, 1);
         }
      }
      if (l==NULL)
         result++;
      else
         delete_Line(l);
   }
   return result;
}

Matrix init_Matrix(int p) {
   Matrix result;
   char *aux;
   int root;
   int *inverse;
   int i, j;
   int a, b, c;
   inverse=find_inverses(p);
   assert(result=my_malloc(sizeof(struct Matrix_tag)));
   result->max_column=p-1;
   if ((p%3)==2)
      result->lines_no=((p/3)<<1)+p-2;
   else
      result->lines_no=((p/3)<<1)+p;
   assert(result->lines=my_calloc((unsigned)result->lines_no, sizeof(Line)));
   i=0;
   assert(aux=my_calloc((unsigned)p+1, sizeof(char)));
   for (j=2; j<=(p>>1); j++, i++)
      result->lines[i]=make_line2(j, 1, p-j, -1, p-1);
   for (j=2; j<(p-1); j++)
      if (aux[j]==0) {
         a=j; b=inverse[j];
         result->lines[i]=make_line2(a, 1, b, 1, p-1);
         i++;
         aux[a]=1; aux[b]=1;
      }
   //printf("%d\n", i);
   for (j=2; j<(p-1); j++)
      if (aux[j]<=1) {
         a=j; b=p+1-inverse[j]; c=p-inverse[j-1];
         result->lines[i]=make_line3(a, 1, b, 1, c, 1, p-1);
         i++;
         aux[a]=2; aux[b]=2; aux[c]=2;
      }
   //printf("%d\n", i);
   root=better_root(p);
   //printf("%d %d\n", root, p);
   for (j=2; j<(p-1); j++)
      if (aux[j]<=2) {
         a=j; b=(p-root+inverse[j])%p; c=inverse[(j+root)%p];
         result->lines[i]=make_line3(a, 1, b, 1, c, 1, p-1);
         i++;
         aux[a]=3; aux[b]=3; aux[c]=3;
      }
   if (i!=result->lines_no) {
      printf("%d %d\n", i, result->lines_no);
      exit(0);
   }
   //print_Matrix(result);
   //free(aux);
   free(inverse);
   return result;
}

void print_Matrix(Matrix M) {
   int i, j;
   for (i=0; i<M->lines_no; i++) {
      if (M->lines[i]==NULL)
         printf("void line");
      else
         for (j=0; j<M->lines[i]->length; j++)
            if (M->lines[i]->values[j]!=0)
               printf("%d:%d ", j, M->lines[i]->values[j]);
   }
   printf("\n");
   return;
}

int main() {
   static int primes2[]={6577, 7517, 7573, 7561, 7541};
   int *primes, i, j, r0, r, count;
   Matrix M;
   primes=sieve(bigN);
   count=primes[0];
   op_no=0;
   for (i=1; primes[i]<smallN; i++);
   for (; i<=count; i++) {
      //if (primes[i]==41)
      //   continue;
      M=init_Matrix(primes[i]);
      r0=M->max_column;
      for (j=0; (r0>0)&&(j<5);
           j++, M=init_Matrix(primes[i])) {
         r=rank_Matrix2(M, primes2[j]);
         printf("r<=%d; op_no=%d\n", r, op_no);
         fprintf(stderr, "%d %d", r, op_no);
         if (r<r0)
            r0=r;
         delete_Matrix(&M);
         op_no=0;
      }
      printf("\n");
      printf("p=%d rank is at most %d\n", primes[i], r0);
      fprintf(stderr, "p=%d rank is at most %d\n", primes[i], r0);
   }
   free(primes);
   return 0;
}
