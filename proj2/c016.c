
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode(tKey key) {
    int retval = 1;
    int keylen = strlen(key);
    for (int i = 0; i < keylen; i++)
        retval += key[i];
    return (retval % HTSIZE);
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit(tHTable *ptrht) {
    for (int i = 0; i < HTSIZE; i++) { //Projedeme všechny indexy a inicializujeme je
        (*ptrht)[i] = NULL;
    }
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem *htSearch(tHTable *ptrht, tKey key) {
    tHTItem *currentItem = (*ptrht)[hashCode(key)]; //Získáme první položku na daném indexu

    while (currentItem != NULL) { //Projedeme položky na daném indexu
        if (strcmp(currentItem->key, key) == 0) { //Pokud má položka požadovaný klíč, pak ji vrátíme
            return currentItem;
        }

        currentItem = currentItem->ptrnext;
    }

    //Prvek nebyl nalezen
    return NULL;
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert(tHTable *ptrht, tKey key, tData data) {
    tHTItem *item = htSearch(ptrht, key);

    if (item != NULL) { //Prvek je již v tabulce
        item->data = data;
    } else { //Prvek v tabulce není
        item = (tHTItem *) malloc(sizeof(tHTItem));

        if (item == NULL) {
            return; //Selhala alokace..
        }

        //Naplníme prvek
        item->key = key;
        item->data = data;

        //Následně se podíváme, jestli už na daném indexu nějaký prvek je
        int index = hashCode(key);
        if ((*ptrht)[index] == NULL) { //Na daném indexu je seznam prázdný, neukazujeme tedy na další prvek
            item->ptrnext = NULL;
        } else { //Na daném indexu v seznamu již prvek je, musíme tedy na tento prvek odkazovat
            item->ptrnext = (*ptrht)[index];
        }

        //Nakonec vložíme nový prvek na začátek seznamu
        (*ptrht)[index] = item;
    }
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData *htRead(tHTable *ptrht, tKey key) {
    tHTItem *item = htSearch(ptrht, key); //Pokusíme se prvek nalézt

    if (item == NULL) { //Položka nebyla nalezena..
        return NULL;
    }

    return &item->data;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete(tHTable *ptrht, tKey key) {
    //Nejprve získáme index
    int index = hashCode(key);

    //Získáme index
    tHTItem *item = (*ptrht)[index];

    //Pomocná proměnná
    tHTItem *previous = NULL;

    while (item != NULL) { //Projedeme položky na daném indexu, dokud nenarazíme na tu s požadovaným klíčem
        if (strcmp(item->key, key) == 0) { //Našli jsme položku určenou ke smazání
            //Podíváme se, jestli je položka v seznamu poslední, nebo ukazuje na další

            if (previous == NULL) { //Pokud mažeme položku, která je v seznamu první
                (*ptrht)[index] = item->ptrnext; //Daný index teď bude odkazovat na druhou položku, která se stane první (nebo NULL, pokud druhá v seznamu není)
            } else { //Položka není v seznamu první
                previous->ptrnext = item->ptrnext; //Předchozí položce nastavíme ukazatel na následující
            }

            free(item); //Uvolníme položku z paměti
            return; //Položka byla smazána, můžeme ukončit..
        }

        previous = item; //Uložíme se aktuální položku jako předchozí
        item = item->ptrnext; //Postoupíme na další položku
    }
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll(tHTable *ptrht) {
    for (int i = 0; i < HTSIZE; i++) { //Projdeme všechny položky v tabulce
        tHTItem *item = (*ptrht)[i];
        tHTItem *nextItem;

        //Projedeme všechny položky na daném indexu
        while (item != NULL) {
            nextItem = item->ptrnext; //Uložíme si následující položku
            free(item); //Uvolníme položku z paměti

            item = nextItem;
        }

        (*ptrht)[i] = NULL; //Daný index je nyní prázdný
    }
}
