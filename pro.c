#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function prototypes for demos
void demoCaesar();
void demoMonoalphabetic();
void demoVigenere();
void demoPlayfair();
void demoHill();
void demoRSA();

int main() {
    int choice;
    do {
        printf("\n=== Cipher Demos ===\n");
        printf("1. Caesar Cipher\n");
        printf("2. Monoalphabetic Cipher\n");
        printf("3. Vigenere Cipher (Polyalphabetic)\n");
        printf("4. Playfair Cipher\n");
        printf("5. Hill Cipher (2x2)\n");
        printf("6. RSA Algorithm\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1: demoCaesar(); break;
            case 2: demoMonoalphabetic(); break;
            case 3: demoVigenere(); break;
            case 4: demoPlayfair(); break;
            case 5: demoHill(); break;
            case 6: demoRSA(); break;
            case 0: break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 0);
    return 0;
}

// 1. Caesar Cipher Demo
void demoCaesar() {
    char text[100];
    int shift;
    printf("\n[Caesar Cipher]\nEnter message: ");
    fgets(text, sizeof(text), stdin);
    printf("Enter shift: ");
    scanf("%d", &shift);
    getchar(); // consume newline
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = (text[i] - base + shift) % 26 + base;
        }
    }
    printf("Encrypted: %s\n", text);
}

// 2. Monoalphabetic Cipher Demo
void demoMonoalphabetic() {
    char text[100];
    // Fixed substitution maps
    char mapU[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
    char mapL[] = "qwertyuiopasdfghjklzxcvbnm";
    printf("\n[Monoalphabetic Cipher]\nEnter message: ");
    fgets(text, sizeof(text), stdin);
    for (int i = 0; text[i]; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z')
            text[i] = mapU[text[i] - 'A'];
        else if (text[i] >= 'a' && text[i] <= 'z')
            text[i] = mapL[text[i] - 'a'];
    }
    printf("Encrypted: %s\n", text);
}

// 3. Vigenere Cipher Demo (Polyalphabetic)
void demoVigenere() {
    char text[100], key[100];
    printf("\n[Vigenere Cipher]\nEnter message: ");
    fgets(text, sizeof(text), stdin);
    printf("Enter key: ");
    fgets(key, sizeof(key), stdin);
    int j = 0, k = strlen(key);
    if (key[k - 1] == '\n') { key[k - 1] = '\0'; k--; }
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            int shift = tolower(key[j % k]) - 'a';
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = (text[i] - base + shift) % 26 + base;
            j++;
        }
    }
    printf("Encrypted: %s\n", text);
}

// 4. Playfair Cipher Demo
#define N 5
void createMatrix(char key[], char mat[N][N]) {
    int used[26] = {0}; 
    used['J' - 'A'] = 1; // Treat J as I
    char out[26];
    int idx = 0;
    for (int i = 0; key[i]; i++) {
        if (isalpha(key[i])) {
            char ch = toupper(key[i]);
            if (ch == 'J') ch = 'I';
            if (!used[ch - 'A']) { out[idx++] = ch; used[ch - 'A'] = 1; }
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!used[ch - 'A']) { out[idx++] = ch; used[ch - 'A'] = 1; }
    }
    idx = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            mat[i][j] = out[idx++];
}

void findPos(char mat[N][N], char ch, int *r, int *c) {
    if (ch == 'J') ch = 'I';
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (mat[i][j] == ch) { *r = i; *c = j; return; }
}

void demoPlayfair() {
    char key[100], text[100], proc[200];
    char matrix[N][N];
    printf("\n[Playfair Cipher]\nEnter key: ");
    fgets(key, sizeof(key), stdin);
    printf("Enter message: ");
    fgets(text, sizeof(text), stdin);
    createMatrix(key, matrix);
    int k = 0;
    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i])) {
            char ch = toupper(text[i]);
            if (ch == 'J') ch = 'I';
            proc[k++] = ch;
        }
    }
    if (k % 2) proc[k++] = 'X';
    proc[k] = '\0';
    for (int i = 0; i < k; i += 2) {
        int r1, c1, r2, c2;
        findPos(matrix, proc[i], &r1, &c1);
        findPos(matrix, proc[i + 1], &r2, &c2);
        if (r1 == r2) {
            proc[i] = matrix[r1][(c1 + 1) % N];
            proc[i + 1] = matrix[r2][(c2 + 1) % N];
        } else if (c1 == c2) {
            proc[i] = matrix[(r1 + 1) % N][c1];
            proc[i + 1] = matrix[(r2 + 1) % N][c2];
        } else {
            proc[i] = matrix[r1][c2];
            proc[i + 1] = matrix[r2][c1];
        }
    }
    printf("Encrypted: %s\n", proc);
}

// 5. Hill Cipher Demo (using a 2x2 matrix)
void demoHill() {
    char text[100];
    int key[2][2] = { {3, 3}, {2, 5} };
    printf("\n[Hill Cipher]\nEnter message (letters only): ");
    scanf("%s", text);
    getchar();
    int len = strlen(text);
    if (len % 2) { text[len++] = 'X'; text[len] = '\0'; }
    for (int i = 0; i < len; i += 2) {
        int a = toupper(text[i]) - 'A';
        int b = toupper(text[i + 1]) - 'A';
        int c1 = (key[0][0] * a + key[0][1] * b) % 26;
        int c2 = (key[1][0] * a + key[1][1] * b) % 26;
        text[i] = c1 + 'A';
        text[i + 1] = c2 + 'A';
    }
    printf("Encrypted: %s\n", text);
}

// 6. RSA Algorithm Demo
// Helper functions for RSA
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }

int modExp(int base, int exp, int mod) {
    int res = 1;
    while(exp) {
        if(exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int modInv(int a, int m) {
    int m0 = m, x0 = 0, x1 = 1, q, t;
    while (a > 1) {
        q = a / m;
        t = m; m = a % m; a = t;
        t = x0; x0 = x1 - q * x0; x1 = t;
    }
    return (x1 < 0) ? x1 + m0 : x1;
}

void demoRSA() {
    // Small primes for demonstration (not secure for real use)
    int p = 17, q = 11;
    int n = p * q, phi = (p - 1) * (q - 1);
    int e = 7; // Choose e such that gcd(e, phi) == 1
    int d = modInv(e, phi);
    int m;
    printf("\n[RSA Demo]\nEnter message (number < %d): ", n);
    scanf("%d", &m);
    getchar();
    int c = modExp(m, e, n);
    int m2 = modExp(c, d, n);
    printf("Encrypted: %d\nDecrypted: %d\n", c, m2);
}