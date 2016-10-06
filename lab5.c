#include <stdio.h>
#include <stdlib.h>

void readFile(double data[], char* str);
void printFile(char* str, double data[]);
void offset(double data[], double offset);
void scale(double data[], double n);
void stats(double data[], char* str);
void center(double data[]);
void normal(double data[]);
void displayhelp();

int main(int argc, char* argv[]){
        char str[15], newstr[20];
        int i, filenum=0, s=0, c=0, n=0, r=0;
        double data[2000];
        double offset_num=0, scale_num=0;
        if(argc <= 1){
            printf("Invalid number of arguments.\n");
            printf("Type './My_Lab5_program -h' for more info\n");
            return 0;
        }
        for(i=1; i<argc; i++){
                if(argv[i][0] == '-'){
                        switch(argv[i][1]){
                        case 'n':
                                i++;//increment i by one, should be a num
                                if(i>=argc || (filenum = atoi(argv[i]))==0){
                                        printf("Invalid format. Number must follow -n\n");
                                        return 0;//if no num follows -n
                                }
                                //printf("File chosen was: %d\n", filenum);
                                break;
                        case 'o':
                                i++;//increment i by one, should be a num
                                if(i>=argc || (offset_num = strtod(argv[i], NULL))==0){
                                        printf("Invalid format. Number>0 must follow -o\n");
                                        printf("Ignoring -o command\n");
                                        offset_num = 0;//if no num follows
                                        break;
                                }
                                //printf("Offset value is: %lf\n", offset_num);
                                break;
                        case 's':
                                i++;//increment i by one, should be a num
                                if(i>=argc|| (scale_num = strtod(argv[i], NULL))==0){//if no num follows  
                                    printf("Invalid format. Number>0 must follow -s\n");
                                    printf("Ignoring -s command\n");
                                    scale_num=0;
                                    break;
                                }
                                //printf("Scale value is: %lf\n", scale_num);
                                break;

                        case 'S':
                                s=1;//statistic command was selected
                                break;
                        case 'C':
                                c=1;//center command was selected
                                break;
                        case 'N':
                                n=1;//normal data command was selected
                                break;
                        case 'r':
                                r=1;//copy file command was selected
                                i++;
                                if(i>=argc || argv[i][0] == '-'){//name did not follow
                                    printf("Invalid format. File name must follow -r\n");
                                    r=0;//if no name follows ignore
                                    printf("Ignoring -r command\n");
                                    break;
                                }
                                sprintf(newstr, "%s.txt", argv[i]);
                                printf("New file: %s\n", newstr);
                                break;
                        case 'h'://help command was selected
                                displayhelp();
                                return 0;
                                break;
                        default:
                                printf("WARNING: argument '%s' was not recognized\n", argv[i]);
                                break;//display warnings
                        }
                }
                else
                    printf("WARNING: argument '%s' was not recognized\n", argv[i]);
                
        }
        if(filenum==0){
            printf("Invalid arguments. No file was chosen\n");
            printf("Type './My_Lab5_program -h' for more info\n");
            return 0;
        }
        sprintf(str, "Raw_data_%02d.txt", filenum);
        printf("File chosen: %s\n", str);
        readFile(data, str);//read file
        if(offset_num != 0){//if offset was on command line
            offset(data, offset_num);
            sprintf(str, "Offset_data_%02d.txt", filenum);
            printFile(str, data);
        }
        if(scale_num != 0){//if scale was on command line
            scale(data, scale_num);
            sprintf(str, "Scaled_data_%02d.txt", filenum);
            printFile(str, data);
        }
        if(s == 1){//if statistics was on command line
            sprintf(str, "Statistics_data_%02d.txt", filenum);
            stats(data, str);
        }
        if(c == 1){//if center data was on command line
            sprintf(str, "Centered_data_%02d.txt", filenum);
            center(data);
            printFile(str, data);
        }
        if(n == 1){//if normalize data was on command line
            sprintf(str, "Normalized_data_%02d.txt", filenum);
            normal(data);
            printFile(str, data);
        }
        if(r == 1){//if copy file was on command line
            printFile(newstr, data);
            printf("Data from %s was copied to %s\n", str, newstr);
        }
        
        return 0;
}
void displayhelp(){
        printf("\n\n***HELP***\nHow to call program: ./a.out {options}\n");
        printf("Options:\n-n: File number (value needed)\n-o: offset value (value needed)\n");
        printf("-s: scale factor (value needed)\n-S: get statistics\n-C: center the signal\n");
        printf("-N: normalize signal\n-r: Rename files (name needed)\n-h: help\n\n");
}
void readFile(double data[], char* str){
        FILE* fp;
        fp = fopen(str, "r");//open file
        double size, max;
        fscanf(fp, "%lf %lf", &size, &max);//get size and max value
        data[0] = size;
        data[1] = max;//first two values in array 
        int i;
        for(i=2; i<data[0]+2; i++){
                fscanf(fp,"%lf", &data[i]);//get data from file
        }
        fclose(fp);//close file

}

void printFile(char* str, double* data){
        FILE* fp = fopen(str, "w");//open file
        if(fp==NULL)
                printf("error opening file\n");
        fprintf(fp,"%d %.4lf\n", (int)data[0], data[1]);//print first row

        int i;
        for(i=2; i<(data[0]+2); i++){
            fprintf(fp, "%.4lf\n", data[i]);//print data to file
        }
        fclose(fp);//close file
        printf("Data printed to %s\n", str);
}

void offset(double* data, double offset){
    int i;
    for(i=2; i<data[0]+2; i++){
        data[i]+=offset;
    }
}
void scale(double* data, double n){
    int i;
    for(i=2; i<data[0]+2; i++){
        data[i]*=n;
    }
}
void stats(double* data, char* str){
    double max, total=0, average;
    max = data[2];//set max to first value
    int i;
    for(i=2; i<data[0]+2; i++){
        total += data[i];//get total for average
        if(data[i]>max)
            max = data[i];//reset max if larger number is found
    }
    average = total/data[0];
    FILE* fp = fopen(str, "w");//write info to file
    fprintf(fp, "%.4lf %.4lf\n", average, max);
    printf("Data printed to %s\n\n", str);
    fclose(fp);//close file
}
void center(double data[]){
    double total=0, average;
    int i;
    for(i=2; i<data[0]+2; i++){
        total += data[i];//get total for average
    }
    average = total/data[0];
    offset(data, (average*-1));//offset data with average to center
}

void normal(double data[]){
    double max;
    max = data[2];//set max to first value
    int i;
    for(i=2; i<data[0]+2; i++){
        if(data[i]>max)
            max = data[i];//reset max if larger number is found
    }
    scale(data, (1/max));//call scale function with scale of 1/max

}
                                                              

