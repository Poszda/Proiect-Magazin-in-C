#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int nr_produse = 0; // nr produse in total;
int nr_categorii = 0;
int nr_cos=0;
float pret_total = 0;
typedef struct produs
{
    char nume[100];
    char categorie[100];
    float pret;
    int cantitate;

}produs;

produs v[100]; //primul element este a[1] // produse
produs cart[100];
char a[100][100]; //primul element este a[1] // vector de categorii

void lowercase(char str[])
{
    int i;
    for(i=0;i<=strlen(str);i++)
      if(str[i]>=65&&str[i]<=90)
            str[i]=str[i]+32;
}
void printCart()
{
    printf("COSUL DE CUMPARATURI:\n\n");
    int i;
    for(i=1;i<=nr_cos;i++)
        printf("DENUMIRE PRODUS: %s       PRET PRODUS: %.2f RON       BUC: %d       NR. %d\n",cart[i].nume,cart[i].pret,cart[i].cantitate,i);
    printf("\n        SUMA TOTALA        \n\n");
    printf("        |||||||||||        \n\n");
    printf("        %.2f RON",pret_total);
}
void addCart(produs x)
{
    int i, ok = 0;
    for(i=1;i<=nr_cos;i++)
        if(strcmp(cart[i].nume,x.nume)==0)
            {
                printf("ACEST PRODUS SE AFLA DEJA IN COS\n");
                ok = 1;
            }
    if(ok==0)
    {
        printf("Cantitatea pe care doriti sa o adaugati: \n");
        int nr;
        scanf("%d",&nr);
        if(nr <= x.cantitate) // se adauga in cart daca cantitatea ceruta este mai mica sau egala cu cea disponibila
            {
                nr_cos++;
                strcpy(cart[nr_cos].nume,x.nume);
                strcpy(cart[nr_cos].categorie,x.categorie);
                cart[nr_cos].pret = x.pret;
                cart[nr_cos].cantitate = nr;
                pret_total = pret_total + nr*x.pret;
            }
        else
            {
                printf("Va rugam introduceti o alta cantitate. Nu sunt disponibile %d BUC de produs\n",nr);
                addCart(x);
            }
    }
}
void deleteCart()
{
    printf("INTRODUCETI NR-ul PRODUSULUI PE CARE DORITI SA IL STERGETI:\n");
    int opt;
    scanf("%d",&opt);
    if(opt<=nr_cos && opt > 0)
    {
       pret_total = pret_total - (cart[opt].pret * cart[opt].cantitate);
       int i;
       for(i=opt;i<=nr_cos-1;i++)
       {
           strcpy(cart[i].nume,cart[i+1].nume);
           strcpy(cart[i].categorie,cart[i+1].categorie);
           cart[i].pret = cart[i+1].pret;
           cart[i].cantitate = cart[i+1].cantitate;
       }
       nr_cos--;
    }
    else printf("NU EXISTA UN PRODUS IN COS CU ACEST NUMAR\n");

}
void updateCart()
{
    printf("INTRODUCETI NR-ul PRODUSULUI A CARUI CANTITATE DORITI SA O ACTUALIZATI:\n");
    int opt;
    scanf("%d",&opt);
    if(opt<=nr_cos && opt >0)
    {
        printf("INTRODUCETI CANTITATEA NOUA:\n");
        int opt2;
        int aux;
        scanf("%d",&opt2);

        int i,ok=0;
        for(i=1;i<=nr_produse;i++)
            if(strcmp(cart[opt].nume,v[i].nume)==0 && opt2 > v[i].cantitate)
            {
                printf("Va rugam introduceti o alta cantitate. Nu sunt disponibile %d BUC de produs\n\n",opt2);
                ok = 1;
                break;
            }

        if(ok==0)
        {
            aux = cart[opt].cantitate;
            cart[opt].cantitate = opt2;
            pret_total = pret_total - (aux * cart[opt].pret);
            pret_total = pret_total + (opt2 *cart[opt].pret);
        }
        if(ok==1) updateCart();

    }
    else
        {
            printf("NU EXISTA UN PRODUS IN COS CU ACEST NUMAR\n");
            updateCart();
        }
}
void updateFile()
{
    FILE *fpointer;
    int i;int j,ok;
    fpointer = fopen("magazin2.txt","a");
    for(i=1;i<=nr_produse;i++)
    {
        ok=0;
        for(j=1;j<=nr_cos;j++)
            if(strcmp(v[i].nume,cart[j].nume)==0)
                {
                    fprintf(fpointer,"%s,%s,%.2f,%d",v[i].nume,v[i].categorie,v[i].pret,v[i].cantitate - cart[j].cantitate);
                    ok=1;
                }
        if(ok==0) fprintf(fpointer,"%s,%s%,%.2f,%d",v[i].nume,v[i].categorie,v[i].pret,v[i].cantitate);
        if(i!=nr_produse) fprintf(fpointer,"\n");
    }
    fclose(fpointer);
    remove("magazin.txt");
    rename("magazin2.txt","magazin.txt");
}
void searchProduct()
{
    printf("CAUTATI DUPA DENUMIREA PRODUSULUI:\n");
    printf("0 - inapoi\n\n");
    char s[100];
    fflush(stdin);
    gets(s);
    lowercase(s);
    if(s[0]!='0')
    {
    int i,ok=0;
    for(i=1;i<=nr_produse;i++)
        if(strcmp(v[i].nume,s)==0)
        {
            ok=1;//am gasit produsul cautat
            printf("produs: %s       categorie: %s       pret: %.2f RON       cantitate: %d BUC\n\n",v[i].nume,v[i].categorie,v[i].pret,v[i].cantitate);
            printf("Doriti sa adaugati produsul in cos?\n");
            printf("1 - da\n");
            printf("2 - nu\n");
            int opt;
            scanf("%d",&opt);
            switch(opt)
            {
            case 1:
                if(v[i].cantitate == 0) {printf("\nPRODUSUL ESTE MOMENTAN INDISPONIBIL\n");}
                else addCart(v[i]);
                break;

            case 2:
                searchProduct();
                break;
            default:
                printf("Error\n\n");
            }
        }
    if(ok==0)
    {
        printf("PRODUSUL NU EXISTA\n\n");
        searchProduct();
    }
    }
}
void searchCategory()
{
    printf("CAUTA DUPA CATEGORIE:\n\n");
    int i,n;
    printf("0 - inpoi\n");
    for(i=1;i<=nr_categorii;i++)
        printf("%d - %s\n",i,a[i]);

   scanf("%d",&n);
   if(n==0){}
   else if(n>0 && n<=nr_categorii)
   {
        int k=0;
        produs w[100];//fac un vector temporar cu produsele din categoria "x"
        for(i=1;i<=nr_produse;i++)
            if(strcmp(a[n],v[i].categorie)==0)
            {
                k++;
                strcpy(w[k].nume,v[i].nume);
                strcpy(w[k].categorie,v[i].categorie);
                w[k].pret = v[i].pret;
                w[k].cantitate =v[i].cantitate;
            }
        //afisare produse din categorie:
        printf("0 - inapoi\n");
        for(i=1;i<=k;i++)
            printf("%d - %s\n",i,w[i].nume);
        scanf("%d",&n); // indicele produsului care urmeaza sa fie ales
        if(n==0) searchCategory();
        else
        {
        printf("produs: %s       categorie: %s       pret: %.2f RON       cantitate: %d BUC\n\n",w[n].nume,w[n].categorie,w[n].pret,w[n].cantitate);
        printf("Doriti sa adaugati produsul in cos?\n");
        printf("1 - da\n");
        printf("2 - nu\n");
        int opt;
        scanf("%d",&opt); // da sau nu
        switch(opt)
        {
        case 1:
            if(w[n].cantitate == 0) {printf("\nPRODUSUL ESTE MOMENTAN INDISPONIBIL \n");}
            else addCart(w[n]);
            break;
        case 2:
            searchCategory();
            break;
        default:
            printf("Error\n\n");
        }
        }
   }
}
void meniu()
{
    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("-------------BINE ATI VENIT LA MAGAZINUL NOSTRU-------------\n");
    printf("------------------------------------------------------------\n");
    printf("0 - EXIT\n");
    printf("1 - CAUTATI UN PRODUS DUPA DENUMIREA SA\n");
    printf("2 - CAUTATI UN PRODUS DUPA CATEGORIA DIN CARE FACE PARTE\n");
    printf("3 - VIZUALIZATI COSUL DE CUMPARATURI\n");
    printf("------------------------------------------------------------\n");
    int OPTIUNE,OPT;
    scanf("%d",&OPTIUNE);
    switch (OPTIUNE)
        {
        case 0:
            break;
        case 1:
            searchProduct();
            meniu();
            break;
        case 2:
            searchCategory();
            meniu();
            break;
        case 3:
            printCart();
            printf("\n\n");
            printf("0 - EXIT\n");
            printf("1 - ACTUALIZATI CANTITATEA UNUI PRODUS DIN COS\n");
            printf("2 - STERGETI UN PRODUS DIN COS\n");
            printf("3 - FINALIZEAZA COMANDA\n");
            printf("4 - INAPOI LA MENIUL PRINCIPAL\n");
            printf("-----------------------------------------------------------\n");
            scanf("%d",&OPT);
            if (OPT == 0) {}
            else if(OPT == 1) {updateCart();meniu();}
            else if(OPT == 2) {deleteCart();meniu();}
            else if(OPT == 3) {updateFile();printf("***COMANDA DUMNEAVOASTRA A FOST INREGISTRATA.VA MULTUMIM***");}
            else if(OPT == 4) meniu();
            break;
        default:
            printf("Ati introdus o optiune inexistenta");
            meniu();
        }
}
int finded(char s[],int n) // functie ajutatoare pentru loadProducs()
{
    int i,ok=0;
    for(i=1;i<=n;i++)
        if(strcmp(a[i],s)==0)
        {
            ok=1;
            break;
        }
    return ok; // daca e 0 adaug produsul in categorie
}
void loadProducts() // imi face un vector cu produse si un vector de char cu numele categoriilor
{
    FILE *fpointer;
    int i,j;
    fpointer = fopen("magazin.txt","r");
    i=0;j=0;
    char str[100];
    while(fgets(str,100,fpointer))
    {
     i++;
     sscanf(str,"%[^,],%[^,],%f,%d",v[i].nume,v[i].categorie,&v[i].pret,&v[i].cantitate);
     if(finded(v[i].categorie,j)==0) strcpy(a[++j],v[i].categorie);
    }
    fclose(fpointer);
    nr_produse = i;
    nr_categorii = j;
}
int main()
{
loadProducts();
meniu();
}

