
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList(tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

    //Inicializujeme prázdný seznam
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLDisposeList(tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    //Nejprve zrušíme ukazatele pro první a aktuální prvek, které nebudeme potřebovat
    L->First = NULL;
    L->Act = NULL;

    //Prvky budeme mazat od posledního
    while (L->Last != NULL) {
        //Uložíme si ukazatel na poslední prvek
        tDLElemPtr last = L->Last;
        //Posuneme se na předchozí prvek
        L->Last = L->Last->lptr;
        //Uvolníme poslední prvek v seznamu
        free(last);
    }
}

void DLInsertFirst(tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //Alokujeme požadované místo pro nový prvek
    tDLElemPtr element = malloc(sizeof(struct tDLElem));

    //Pokud se alokace nezdaří, zavoláme DLError
    if (element == NULL) {
        DLError();
    } else { //Alokace se zdařila, přidáme prvek do seznamu
        //Prvek bude na začátku seznamu, nebude proto ukazovat na žádný předchozí prvek
        element->lptr = NULL;
        //Následujícím prvkem se stane ten, který je aktuálně v seznamu jako první
        element->rptr = L->First;
        //Nakonec přiradíme data
        element->data = val;

        //V případě, že není seznam prázdný
        if (L->First != NULL) {
            //Aktuálně první prvek se stane druhým a bude tak ukazovat na nový prvek jako na předchozí
            L->First->lptr = element;
            //Vložíme nový prvek do seznamu jako první
            L->First = element;
        } else { //Pokud je seznam prázdný, tak bude nový prvek v seznamu prvním i posledním prvkem
            L->First = element;
            L->Last = element;
        }
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //Alokujeme požadované místo pro nový prvek
    tDLElemPtr element = malloc(sizeof(struct tDLElem));

    //Pokud se alokace nezdaří, zavoláme DLError
    if (element == NULL) {
        DLError();
    } else { //Alokace se zdařila, přidáme prvek do seznamu
        //Prvek bude na konci seznamu, nebude proto ukazovat na žádný následující prvek
        element->rptr = NULL;
        //Předchozím prvkem se stane ten, který je aktuálně v seznamu jako poslední
        element->lptr = L->Last;
        //Nakonec přiradíme data
        element->data = val;

        //V případě, že není seznam prázdný
        if (L->First != NULL) {
            //Aktuálně poslední prvek se stane předposledním a bude tak ukazovat na nový prvek jako na následující
            L->Last->rptr = element;
            //Vložíme nový prvek do seznamu jako poslední
            L->Last = element;
        } else { //Pokud je seznam prázdný, tak bude nový prvek v seznamu prvním i posledním prvkem
            L->First = element;
            L->Last = element;
        }
    }
}

void DLFirst(tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    //Ukazatel na aktivní prvek odkážeme na první prvek seznamu
    L->Act = L->First;
}

void DLLast(tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    //Ukazatel na aktivní prvek odkážeme na poslední prvek seznamu
    L->Act = L->Last;
}

void DLCopyFirst(tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    //Pokud je seznam prázdný, zavoláme DLError
    if (L->First == NULL) {
        DLError();
    } //Seznam není prázdný
    else {
        //Ukazatel val odkážeme na hodnotu prvního prvku
        *val = L->First->data;
    }
}

void DLCopyLast(tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    //Pokud je seznam prázdný, zavoláme DLError
    if (L->First == NULL) {
        DLError();
    } //Seznam není prázdný
    else {
        //Ukazatel val odkážeme na hodnotu posledního prvku
        *val = L->Last->data;
    }
}

void DLDeleteFirst(tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    //Pokud je seznam prázdný, tak nebudeme provádět žádnou operaci
    if (L->First != NULL) {
        //Pokud je první prvek aktivní, tak aktivitu zrušíme
        if (L->First == L->Act) {
            L->Act = NULL;
        }

        //Uložíme si ukazatel na první prvek
        tDLElemPtr first = L->First;

        //Podíváme se, zda je v seznamu více prvků
        if (L->First != L->Last) {
            //Druhému prvku zrušíme ukazatel na předchozí prvek
            L->First->rptr->lptr = NULL;
            //Druhý prvek označíme jako první
            L->First = L->First->rptr;
        } //Pokud je prvek v seznamu jediný, pak zůstane seznam prádný
        else {
            L->First = NULL;
            L->Last = NULL;
        }

        //Uvolníme původní první prvek z paměti
        free(first);
    }
}

void DLDeleteLast(tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    //Pokud je seznam prázdný, tak nebudeme provádět žádnou operaci
    if (L->First != NULL) {
        //Pokud je poslení prvek aktivní, tak aktivitu zrušíme
        if (L->Last == L->Act) {
            L->Act = NULL;
        }

        //Uložíme si ukazatel na poslední prvek
        tDLElemPtr last = L->Last;

        //Podíváme se, zda je v seznamu více prvků
        if (L->First != L->Last) {
            //Předposlednímu prvku zrušíme ukazatel na následující prvek
            L->Last->lptr->rptr = NULL;
            //Předposlední prvek označíme jako poslední
            L->Last = L->Last->lptr;
        } //Pokud je prvek v seznamu jediný, pak zůstane seznam prádný
        else {
            L->First = NULL;
            L->Last = NULL;
        }

        //Uvolníme původní první prvek z paměti
        free(last);
    }
}

void DLPostDelete(tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    //Pokud je seznam neaktivní, nebo je aktivní prvek v seznamu poslední, pak se neprovede žádná operace
    if (L->Act != NULL && L->Last != L->Act) {

        //Uložíme si ukazatel na následující prvek
        tDLElemPtr next = L->Act->rptr;

        if (L->Last == next) { //Následující prvek je v seznamu poslední
            //Aktivní prvek se stane v seznamu posledním, proto zrušíme jeho ukazatel na další prvek a označíme ho jako poslední prvek v seznamu
            L->Act->rptr = NULL;
            L->Last = L->Act;
        } else { //Následující prvek není v seznamu poslední
            //Aktivní prvek bude ukazovat na prvek za následujícím jako na následující prvek
            L->Act->rptr = next->rptr;
            //Prvek za následujícím bude ukazovat na aktivní prvek jako na předchozí prvek
            next->rptr->lptr = L->Act;
        }

        //Uvolníme následující prvek
        free(next);
    }
}

void DLPreDelete(tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    //Pokud je seznam neaktivní, nebo je aktivní prvek v seznamu první, pak se neprovede žádná operace
    if (L->Act != NULL && L->First != L->Act) {

        //Uložíme si ukazatel na předchozí prvek
        tDLElemPtr before = L->Act->lptr;

        if (L->First == before) { //Předchozí prvek je v seznamu první
            //Aktivní prvek se stane v seznamu prvním, proto zrušíme jeho ukazatel na předchozí prvek a označíme ho jako poslední prvek v seznamu
            L->Act->lptr = NULL;
            L->First = L->Act;
        } else { //Předchozí prvek není v seznamu poslední
            //Aktivní prvek bude ukazovat na prvek před předchozím jako na předchozí prvek
            L->Act->lptr = before->lptr;
            //Prvek před předchozím bude ukazovat na aktivní prvek jako na následující prvek
            before->lptr->rptr = L->Act;
        }

        //Uvolníme předchozí prvek
        free(before);
    }
}

void DLPostInsert(tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //Pokud není seznam aktivní, neprovádíme žádnou operaci
    if (L->Act != NULL) {
        //Alokujeme požadované místo pro nový prvek
        tDLElemPtr element = malloc(sizeof(struct tDLElem));

        //Pokud se alokace nezdaří, zavoláme DLError
        if (element == NULL) {
            DLError();
        } else { //Alokace se zdařila, přidáme prvek do seznamu
            //Nový prvek bude za aktivním, ukazatel na předchozí prvek tedy bude ukazovat na ten aktivní
            element->lptr = L->Act;
            //Přiradíme data
            element->data = val;

            if (L->Act == L->Last) { //Pokud je aktivní prvek v seznamu poslední, pak se posledním stane nový prvek
                element->rptr = NULL;
                L->Last = element;
            } else { //Pokud Aktivní prvek není v seznamu poslední
                //Prvku za aktivním upravíme ukazatel na předchozí prvek
                L->Act->rptr->lptr = element;
                //Novému prvku upravíme ukazatel na následující prvek
                element->rptr = L->Act->rptr;
            }

            //Aktivní prvek bude ukazovat na nový jako na následující
            L->Act->rptr = element;
        }
    }
}

void DLPreInsert(tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //Pokud není seznam aktivní, neprovádíme žádnou operaci
    if (L->Act != NULL) {
        //Alokujeme požadované místo pro nový prvek
        tDLElemPtr element = malloc(sizeof(struct tDLElem));

        //Pokud se alokace nezdaří, zavoláme DLError
        if (element == NULL) {
            DLError();
        } else { //Alokace se zdařila, přidáme prvek do seznamu
            //Nový prvek bude před aktivním, ukazatel na následující prvek tedy bude ukazovat na ten aktivní
            element->rptr = L->Act;
            //Přiradíme data
            element->data = val;

            if (L->Act == L->First) { //Pokud je aktivní prvek v seznamu první, pak se prvním stane nový prvek
                element->lptr = NULL;
                L->First = element;
            } else { //Pokud Aktivní prvek není v seznamu první
                //Prvku před aktivním upravíme ukazatel na následující prvek
                L->Act->lptr->rptr = element;
                //Novému prvku upravíme ukazatel na předchozí prvek
                element->lptr = L->Act->lptr;
            }

            //Aktivní prvek bude ukazovat na nový jako na předchozí
            L->Act->lptr = element;
        }
    }
}

void DLCopy(tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    //Pokud není seznam aktivní, zavoláme DLError
    if (L->Act == NULL) {
        DLError();
    } else {
        //Ukazatel val nasměrujeme na data aktivního prvku
        *val = L->Act->data;
    }
}

void DLActualize(tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    //Pokud není seznam aktivní, neprovádíme žádnou operaci
    if (L->Act != NULL) {
        //Zmenime data aktivního prvku
        L->Act->data = val;
    }
}

void DLSucc(tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    //Pokud není seznam aktivní, neprovádíme žádnou operaci
    if (L->Act != NULL) {
        //Posuneme aktivní prvek na další
        L->Act = L->Act->rptr;
    }
}


void DLPred(tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    //Pokud není seznam aktivní, neprovádíme žádnou operaci
    if (L->Act != NULL) {
        //Posuneme aktivní prvek na předchozí
        L->Act = L->Act->lptr;
    }
}

int DLActive(tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return (L->Act != NULL) ? 1 : 0;
}

/* Konec c206.c*/
