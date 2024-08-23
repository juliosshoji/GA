#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

int letterperLine(FILE* archive);
int matrixSize(char* string, int lineSize);
void separateValues(FILE* archive, double* matrix, double* resMatrix, int maxlineSize);
void doGaussJordan(double* matrix, double* vetor, int maxIndex);

int main(int argc, char* argv[]){

        struct timespec beginprocess;
        timespec_get(&beginprocess, TIME_UTC);
        struct timespec begincpu;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &begincpu);

        FILE* linearSytem = fopen(argv[1], "r");
        if(linearSytem == NULL){
            printf("Error opening file\n");
            return 1;
        }
        
        int lettersLine1 = letterperLine(linearSytem);
        rewind(linearSytem);
        
        char* line = (char*)malloc(lettersLine1*sizeof(char));
        if(line == NULL){
            printf("Error alocating memory\n");
            return 1;
        }
        fscanf(linearSytem, "%s", line);
        rewind(linearSytem);

        int maxMatrixSize = matrixSize(line, lettersLine1);
        
        double* resMatrix = (double*)malloc(maxMatrixSize*sizeof(double));
        if(resMatrix == NULL){
            printf("Error alocating memory");
            return 1;
        }
        double* matrix = (double*)malloc(maxMatrixSize*maxMatrixSize*sizeof(double));
        if(matrix == NULL){
            printf("Error alocating memory\n");
            return 1;
        }

        separateValues(linearSytem, matrix, resMatrix, maxMatrixSize*6);

        for(int line = 0; line < maxMatrixSize*maxMatrixSize; line++){
            if(line%maxMatrixSize == 0)
                printf("\n");
            printf("%lf ", matrix[line]);
        }
        printf("\n\n");

        for(int index = 0; index < maxMatrixSize; index++){
            printf("%lf ", resMatrix[index]);
        }
        printf("\n\n");

        doGaussJordan(matrix, resMatrix, maxMatrixSize);
    
        printf("\n\n");
        fclose(linearSytem);
        free(resMatrix);
        free(matrix);
        free(line);

        struct timespec endprocess;
        timespec_get(&endprocess, TIME_UTC);
        struct timespec endcpu;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endcpu);

        double processtime = (endprocess.tv_sec-beginprocess.tv_sec)+(endprocess.tv_nsec-beginprocess.tv_nsec)/1000000000.0;
        double cputime = (endcpu.tv_sec-begincpu.tv_sec)+(endcpu.tv_nsec-begincpu.tv_nsec)/1000000000.0;

        printf("Process total time: %lf\n", processtime);
        printf("CPU Total Time: %lf\n\n", cputime);

        return 0;
}




void doGaussJordan(double* matrix, double* vetor, int maxIndex){

    int index = 0;
    double gauss[maxIndex][maxIndex];
    
    for(int line = 0; line < maxIndex; line++){
        for(int column = 0; column < maxIndex; column++){
            gauss[line][column] = matrix[index];
            index++;
        }
    }
    double temp = 0;
    double linetemp = 0;
    int changeID = 1;

    
    for(int index = 0; index <  maxIndex; index++){
        if(gauss[index][index] != 1.0){
            if(gauss[index][index] == 0.0){
                if(index == maxIndex-1.0){
                    for(int line = 0; line < maxIndex; line++){
                        for(int column = 0; column < maxIndex; column++){
                            printf("%lf ", gauss[line][column]);
                            if(column == maxIndex-1)
                                printf("\n");
                        }
                    }
                    printf("\n\n");
                    for(int index = 0; index < maxIndex; index++){
                        printf("%lf ", vetor[index]);
                    }
                    printf("\n\nNOT COMPLETED\n\n");
                }
                else{
                    while(gauss[index][index] == 0.0){
                        for(int column = 0; column < maxIndex; column++){
                            temp = gauss[index][column];
                            gauss[index][column] = gauss[index+changeID][column];
                            gauss[index+changeID][column] = temp;
                        }
                        temp = vetor[index];
                        vetor[index] = vetor[index+1];
                        vetor[index+1] = temp;
                        changeID++;
                    }
                    for(int line = 0; line < maxIndex; line++){
                        for(int column = 0; column < maxIndex; column++){
                            printf("%lf ", gauss[line][column]);
                            if(column == maxIndex-1)
                                printf("\n");
                        }
                    }
                    printf("\n\n");
                    for(int index = 0; index < maxIndex; index++){
                        printf("%lf ", vetor[index]);
                    }
                    printf("\n\nNOT COMPLETED\n\n");
                    changeID = 1;
                }                   
            }
            linetemp = gauss[index][index];
            for(int column = 0; column < maxIndex; column++){
                gauss[index][column] = gauss[index][column]/linetemp;
            }
            vetor[index] = vetor[index]/linetemp;
        }
        
        for(int line = 0; line < maxIndex; line++){
            if(line != index){
                if(gauss[line][index] != 0.0){
                    linetemp = gauss[line][index]/gauss[index][index];
                    for(int column = 0; column < maxIndex; column++){
                        gauss[line][column] = gauss[line][column] + (gauss[index][column]*(-linetemp));
                    }
                    vetor[line] = vetor[line] + (vetor[index]*(-linetemp));
                }
            }
        }
        
    }

    for(int line = 0; line < maxIndex; line++){
        for(int column = 0; column < maxIndex; column++){
          printf("%lf ", gauss[line][column]);
            if(column == maxIndex-1)
                printf("\n");
        }
    }
    printf("\n\n");

    for(int index = 0; index < maxIndex; index++){
        printf("%lf ", vetor[index]);
    }
    printf("\n\n");
}




void separateValues(FILE* archive, double* matrix, double* resMatrix, int maxlineSize){

    char* readLine = (char*)malloc(maxlineSize*sizeof(char));
    if(readLine == NULL){
        exit(1);
    }
    char* num = (char*)malloc(20*sizeof(char));
    if(num == NULL){
        exit(1);
    }
    

    int numIndex = 0;
    int matrixIndex = 0;
    int resIndex = 0;

    while(fscanf(archive, "%s\n", readLine) != EOF)
    {
        for(int index2 = 0; index2 < maxlineSize; index2++){
            if(readLine[index2] == '\0')
                break;
            if(readLine[index2] == ';'){
                num[numIndex] == '\0';
                resMatrix[resIndex] = atoi(num);
                numIndex = 0;
            }
            else{
                if((readLine[index2] == ',')){
                    num[numIndex] = '\0';
                    matrix[matrixIndex] = atoi(num);
                    numIndex = 0;
                    matrixIndex++;
                }else{ 
                    num[numIndex] = readLine[index2];
                    numIndex++;
            }
            }
            
        }
        resIndex++;
    }
    free(num);
    free(readLine);
}
//Return the size of the matrix in the archive
int matrixSize(char* string, int lineSize){

    int size = 0;

    for(int index = 0; index < lineSize; index++){
        if(string[index] == ','||string[index] == ';'){
            size++;
        }
    }

    return --size;
}

//Conta quantidade caracteres e retorna em int
int letterperLine(FILE* archive){
    int count = 0;
    while(fgetc(archive) != ';'){
        count++;
    }
    return ++count;
}