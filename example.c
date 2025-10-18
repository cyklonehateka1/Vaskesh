#include <stdio.h>
#include <unistd.h>
#include <string.h>

char getLastChar(const char *str) {
    // This is the core "one code" line. It finds the length, subtracts 1 for the index.
    return str[strlen(str) - 1];
}

int main() {
    char myString[] = "Compact";
    printf("The last character is: %c\n", getLastChar(myString)); // Output: t
    return 0;
}