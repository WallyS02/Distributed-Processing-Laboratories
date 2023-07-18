#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main() {
    printf("Podaj tekst: ");
    char text[200];
    scanf("%199s", text);

    FILE *file = fopen("file.txt", "w+");

    fprintf(file, "%s", text);
    rewind(file);

    int pid1 = fork();
    if (pid1 == 0) {
        fscanf(file, "%s", text);
        for (int i = 0; i < strlen(text); i++) {
            if ('A' <= text[i] && text[i] <= 'Z')
                text[i] = (char) (text[i] + 32);
            else if ('a' <= text[i] && text[i] <= 'z')
                text[i] = (char) (text[i] - 32);
        }
        ftruncate(fileno(file), 0);
        rewind(file);
        fprintf(file, "%s", text);
        rewind(file);

        int pid2 = fork();
        if (pid2 == 0) {
            fscanf(file, "%s", text);
            printf("%s\n", text);
            fclose(file);
            return 0;
        } else {
            waitpid(pid2, NULL, 0);
            return 0;
        }
    } else {
        waitpid(pid1, NULL, 0);
        return 0;
    }
}