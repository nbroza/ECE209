#include <stdio.h>
#include <math.h>

/*
    Shifting nth root algorithm
    ECE209 - Greg Byrd
    Nick Broza

*/

int power(int b, int n){
    if (n == 0) {
        return 1;
    }
    
    if (n == 1) {
        return b;
    }

    int acc = 1;    
    for (int i = 0; i < n; i++) {
        acc *= b;    
    }
    
    return acc;
}

int ceiling(double b) {
    if (b == (int) b) {
        return b;
    }
    return b += 1;
}

int main() {
    int A, n, k;
    
    printf("Number: ");
    scanf("%d", &A);
    printf("Root: ");
    scanf("%d", &n);
    printf("Digits: ");
    scanf("%d", &k);
    
    printf("Compute root %d of %d to %d digits.\n", n, A, k);
    
    int length = 0;
    int tempA = A; //Creates a temp copy of A
    while (tempA != 0) {
        tempA /= 10;
        length++;
    }

    float numgroups = ceiling((float)length / n); //Finds the smallest integer that will fit groups n long
    printf("Number has %.f groups of %d digits.\n", numgroups, n);
    
    int groups[(int)numgroups];
    
    tempA = A; //Resets the temp A value
    int acc = 0;
    int count = 0;
    int placevalue = 1;
    int groupnum = numgroups - 1;
    while(tempA != 0) { //Loop to split up the number, this is very overcomplicated but it works
        int ld = tempA % 10;
        tempA /= 10;
        acc += ld * placevalue;
        groups[groupnum] = acc;
        placevalue *= 10;
        count++;
        
        if (count == n) {
            acc = 0;
            count = 0;
            placevalue = 1;
            groupnum--;
        }
    }

    groupnum = 0;
    int y = 0;
    int beta = 0;
    double r = 0;
    
    for (int i = numgroups + k; i >= 1; i--) { //Main loop for algorithm, handles iterations
        int alpha = groups[groupnum];
        
        if(groupnum > numgroups - 1) {
            alpha = 0;
        }
        
        printf("alpha = %d, ", alpha);
        r = pow(10,n) * r + alpha;
        
        for (int j = 9; j >= 0; j--) { //Loop to find largest beta value
            double betanum = pow(10 * y + j, n) - pow(10 * y, n);
            if (betanum <= r) {
                beta = j;
                printf("beta = %d\n", beta);
                break;
            }
        }
        
        r -= pow(10 * y + beta, n) - pow(10 * y, n);  
        
        if (r < 0) {
            r = 0;
        }
        
        y = 10 * y + beta; 
        
        printf("y = %d, r = %.f\n", y, r);
        groupnum++;
    }

    printf("\n");
    
    double x = (double) y / power(10, k);
    
    printf("Estimated root = %f\n", x);
    printf("Check: %f to the %d = %f (orig = %d)\n", x, n, pow(x, n), A);
    

}