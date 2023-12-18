#include <stdio.h>

int main() {
    int ma_liste[9] = {5, 6, 7, 8, 1, 2, 0, 10, 15};
    int a;
    int cond = 0;

    for (int i = 0; i < sizeof(ma_liste) / sizeof(ma_liste[0]); i++) {
        printf("%d ", ma_liste[i]);
    }

    printf("\nEntrez la valeur que vous cherchez : ");
    scanf("%d", &a);

   
    for (int i = 0; i < sizeof(ma_liste) / sizeof(ma_liste[0]); i++) {
        if (ma_liste[i] == a) {
            printf("La valeur %d est dans le tableau à la position %d\n", a, i);
            cond = 1;
        }
    }


    if (cond == 0) {
        printf("La valeur %d n'est pas dans le tableau\n", a);
    }

    return 0;
}

