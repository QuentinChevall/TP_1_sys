#include <stdio.h>

int main() {
    int ma_liste[] = {1,2,3,4,5,6,7,8,9,10};
    int a;
    int cond = 0;
    int index=0;

    for (int i = 0; i < sizeof(ma_liste) / sizeof(ma_liste[0]); i++) {
        printf("%d ", ma_liste[i]);
    }

    printf("\nEntrez la valeur que vous cherchez : ");
    scanf("%d", &a);

   
    for (int i = 0; i < sizeof(ma_liste) / sizeof(ma_liste[0]); i++) {
        if (ma_liste[i] == a) {
            printf("La valeur %d est dans le tableau à la position %d\n", a, i);
            cond = 1;
            index=i;
        }
        
    }


    if (cond == 0) {
        printf("La valeur %d n'est pas dans le tableau\n", a);
    }
    else
    {
    if (index>=0.5*(sizeof(ma_liste) / sizeof(ma_liste[0])))
    	{	
    	printf("Le nombre est dans la partie droite ! \n");	
    	}
    else
    {
    	printf("Le nombre est dans la partie gauche ! \n");
    }
    
    }


    return 0;
}

