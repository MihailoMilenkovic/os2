#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Neke primedbe (ispravite me ako gresim, zato i navodim):
 * 1) u sustini mora postojati neka globalna promenljiva na koju ce
 *    svi procesi da uticu - da ili ne? To je ono sto me je zbunji-
 *    valo na casu, dakle, fork() podeli proces na dva procesa koji
 *    mogu da komuniciraju medjusobno, ali ne postoji nacin da pro-
 *    ces primi vrednost, obradi je, i posalje je dalje jer u mome-
 *    ntu kada on prihvata vrednost on je dete, a da bi je slao da-
 *    lje on mora biti roditelj, u kom slucaju je IZNAD mesta na k-
 *    om dobija tu vrednost u kodu pa je nece videti... Ili ne kap-
 *    iram nesto dobro?
 * 2) da li je ovo moguce resiti rekurzivno?
 */

int res = 1;
bool done = false;

int fact(int num)
{
    int cnt = num;
    bool createChildren = true;

    while (cnt > 0 && createChildren)
    {
        // value passed from parent to child
        int passed_value;

        int fd[2];

        if (pipe(fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        int pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid != 0)
        {
            createChildren = false;

            // parent writes, close read descriptor
            close(fd[0]);

            // pass the value to child
            write(fd[1], &cnt, sizeof(cnt));

            // close write descriptor
            close(fd[1]);

            int return_status;
            waitpid(pid, &return_status, 0);

            if (cnt == 1)
                done = true;
        }
        else
        {
            // child reads, close write descriptor
            close(fd[1]);

            read(fd[0], &passed_value, sizeof(passed_value));
            res *= passed_value;

            printf("step %2d : %d \n", cnt, res);

            // close read descriptor
            close(fd[0]);
        }

        cnt--;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int num = atoi(argv[1]);
    fact(num);

    if (done)
        printf("fact(%d) : %d \n", num, res);

    return 0;
}