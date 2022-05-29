#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

// dimensions of both matrices as global variables 
int r1,c1,r2,c2;

// scan file name into this variable
char file_name[256];

// variable to keep track of row in row multiplication we can increment it insted of sending array
int row_counter=0;

// total rows and columns could have been just r1 and c2 
int rows;
int columns;
int add; // no of c1 or r2
// allocate memory for array

int **m1;
int **m2;
int **ans;

// variablke to store time for element by element
double time_spent = 0.0;
clock_t begin;
clock_t end;
// variables for row by row
double time_spent_row = 0.0;
clock_t begin_row;
clock_t end_row;

void load_arrays(){

printf("please enter input file name\n");

fgets(file_name,256,stdin);
strtok(file_name,"\n"); 
 
FILE *fp=fopen(file_name,"r");

fscanf(fp,"%d",&r1);
fscanf(fp,"%d\n",&c1);

add=c1;
rows=r1;

m1=(int**)malloc(r1*sizeof(int*));
 for(int i=0;i<r1;i++)
    m1[i]=(int*)malloc(c1*sizeof(int));

 

for(int i=0;i<r1;i++){
  
 
for(int j=0;j<c1;j++){

 
  
  fscanf(fp,"%d\n",& m1[i][j]);
 
 
           
 
}

}
    
fscanf(fp,"%d",&r2);
fscanf(fp,"%d\n",&c2);
columns=c2;

// if dimensions are not compatible print in file and terminate prorgram

if(c1 != r2 ){

 FILE *f;
 f = fopen("output.txt", "w");
 fprintf(f," cannot multiply invalid dimensions");
fclose(f);
exit(0);
}
 
m2=(int**)malloc(r2*sizeof(int*));
 for(int i=0;i<r2;i++)
    m2[i]=(int*)malloc(c2*sizeof(int));

 
 
for(int i=0;i<r2;i++){
  
 
 
for(int j=0;j<c2;j++){

 
 
  fscanf(fp,"%d\n",& m2[i][j]);
 
 
}

}

 ans=(int**)malloc(rows*sizeof(int*));
 for(int i=0;i<rows;i++)
    ans[i]=(int*)malloc(columns*sizeof(int));


 
}
// end of load
 
 // function for row by row 
// thread for each row 
void *mult2(void *arg){


// we need to identify which row we are in so we need to increment it every cycle easier than sending an array

int *p=arg;
int row=row_counter++;

  for (int i = 0; i<columns ; i++){
      for (int j=0; j<add; j++){
      
      
      
      
        ans[row][i] =ans[row][i]+m1[row][j]*m2[j][i];      
        
                
        }

}



}


// thread for each element 
// function to calculate value of each element
void *mult(void *arg){

    int * rowColumn= (int *)arg;
    int element = 0 ;
     
     
    for (int i = 0; i <add ; i++)
           element =element + rowColumn[i]*rowColumn[i+add];
     
    int *result = (int*)malloc(sizeof(int));
         *result = element;
   
  // we need to return value from thread in a pointer
  pthread_exit(result);
}

int main(){

//   load all arrays and allocate memory for them
     load_arrays();     

   
   
//declaring array of threads of size rows * columns    
    
    
     pthread_t *threads;
     threads = (pthread_t*)malloc(rows*columns*sizeof(pthread_t));
  
     int thread_count = 0;
     int *array;
     
     
   
 
    for (int i = 0; i < r1; i++){
    
    
        for (int j = 0; j < c2; j++)
               {
                
               //storing row and column elements in array
            array = (int *)malloc((add)*(2)*sizeof(int));
         
     
            for (int a = 0; a < c1; a++)
                array[a] = m1[i][a];
     
            for (int b = 0; b < r2; b++)
                array[b+add] = m2[b][j];
              
             //creating threads
                pthread_create(&threads[thread_count++], NULL,&mult,array);
               if(i==0 && j==0){
                 begin = clock();
                   }
                   
                    
                    }
                    
                                
    }
 // print result in file
 
 
    FILE *f;
    f = fopen("output.txt", "w"); // write into file
    if (f == NULL) { /* Something is wrong   */}
 
 
  fprintf(f,"element by element \n");
    for (int i = 0; i < rows*columns; i++)
    {
      void *answer;
       
      //Joining the threads and getting our results
      
      pthread_join(threads[i], &answer);
      if(i==(rows*columns)-1){
        end = clock(); 
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;    
           }
           
          int *p = (int *)answer;
          fprintf(f,"%d ",*p);
      if ((i + 1) % c2 == 0) // check if we need to separate different rows by new line
         fprintf(f,"\n");
    }

 fprintf(f,"time spent is %f \n",time_spent);
    
// another array of threads for row by row 
pthread_t *threads2;
threads2 = (pthread_t*)malloc(rows*sizeof(pthread_t));
   
for(int i=0;i<rows;i++){

int *arr2;
pthread_create(&threads2[i],NULL,&mult2,arr2);
if(i==0){
 begin_row = clock();
}


}
for(int i=0;i<rows;i++){

 
pthread_join(threads2[i],NULL);
if(i==rows-1){
 end_row = clock(); 
  time_spent_row += (double)(end_row - begin_row) / CLOCKS_PER_SEC; 
}


}

// save answer in file for row by row 
 fprintf(f,"row by row \n");
for(int i=0;i<rows;i++){

for(int j=0;j<columns;j++){


fprintf(f,"%d ",ans[i][j]);



}
// new row
fprintf(f,"\n");
}


fprintf(f," time for row by row is %f\n", time_spent_row); 



// close file

fclose(f);


  return 0;
}
