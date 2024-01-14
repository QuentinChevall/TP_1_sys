#include <stdio.h>
void Binary(void)
{
    int recherche=10;
        int ma_liste[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                          16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
                          31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
                          46,47,48,49,50};
        int a;
        int cond = 0;
        int index=0;
        
        for (int i = 0; i < sizeof(ma_liste) / sizeof(ma_liste[0]); i++) {
            if (ma_liste[i] == recherche) {
                printf("La valeur %d est dans le tableau à la position %d\n", recherche, i);
                cond = 1;
                index=i;
            }
            
        }  
}

int main(void)
{
    Binary();
    return 0;
}
