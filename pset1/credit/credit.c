#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(){
    long num;
    int i;
    int sum1, sum2, digit1, digit2, digit1_1, digit1_2, digit2_1, digit2_2, sum_total;
    long long_pow;

    printf("Number: ");
    scanf("%ld", &num);
    
    sum1 = 0;
    sum2 = 0;
    digit1 = 0;
    digit2 = 0;
    for (i=1; i <= 16; i+=2){ // a "for" loop for even digits
        long_pow = pow(10,i); // pow is needed not ^(XOR)
        digit1 =((num/long_pow) % 10)*2;

        if (digit1 >= 10){
            digit1_1 = digit1 % 10;
            digit1_2 = (digit1/10) %10;
            digit1 = digit1_1 + digit1_2;
        }
        
        sum1 = digit1 + sum1;
    }

    for (i=0; i <=16; i+=2){ // a "for" loop for odd digits
        long_pow = pow(10,i);
        digit2 = (num/long_pow) % 10;

        sum2 = digit2 + sum2;
    }

    sum_total = sum1 + sum2; 
    
    int count =0; // count digits of num
    long num_count = num;
    while (num_count != 0){
        num_count /= 10;
        ++count;
    }
    
    int start_num1, start_num2;
    int check_sum = sum_total % 10;
    start_num1 = num / (pow(10,count-2));
    start_num2 = num / (pow(10,count-1));
    if (check_sum == 0){ // check sum
        
        if (count == 15){
            if (start_num1 == 34 || start_num1 == 37){
                printf("AMEX\n");
            }
            else{printf("INVALID\n");}
        }
        else if (count == 16){
            if (start_num1 == 51 || start_num1 == 52 || start_num1 == 53 || start_num1 == 54 || start_num1 == 55){
                printf("MASTERCARD\n");
            }

            else if (start_num2 == 4){
                printf("VISA\n");
            }
            else{printf("INVALID\n");}
        }
        else if (count == 13 && start_num2==4){
            printf("VISA\n");
        }
        else{
            printf("INVALID\n");
        }
        
        
    }
    else{
        printf("INVALID\n");
    }
}