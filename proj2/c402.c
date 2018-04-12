
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**                                         Úpravy: Radek Hranický, říjen 2016
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte 
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"

int solved;

void BTWorkOut(tBTNodePtr Ptr) {
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/

    if (Ptr == NULL)
        printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
    else
        printf("Výpis hodnoty daného uzlu> %d\n", Ptr->Cont);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP(tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{
    S->top = 0;
}

void SPushP(tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{
    /* Při implementaci v poli může dojít k přetečení zásobníku. */
    if (S->top == MAXSTACK)
        printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
    else {
        S->top++;
        S->a[S->top] = ptr;
    }
}

tBTNodePtr STopPopP(tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
    /* Operace nad prázdným zásobníkem způsobí chybu. */
    if (S->top == 0) {
        printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
        return (NULL);
    } else {
        return (S->a[S->top--]);
    }
}

bool SEmptyP(tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
    return (S->top == 0);
}

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB(tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

    S->top = 0;
}

void SPushB(tStackB *S, bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
    /* Při implementaci v poli může dojít k přetečení zásobníku. */
    if (S->top == MAXSTACK)
        printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
    else {
        S->top++;
        S->a[S->top] = val;
    }
}

bool STopPopB(tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
    /* Operace nad prázdným zásobníkem způsobí chybu. */
    if (S->top == 0) {
        printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
        return (NULL);
    } else {
        return (S->a[S->top--]);
    }
}

bool SEmptyB(tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
    return (S->top == 0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/

void BTInit(tBTNodePtr *RootPtr) {
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**	
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,	
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/

    *RootPtr = NULL;
}

void BTInsert(tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/
    if (*RootPtr == NULL) { //Strom je prázdný, uzel vložíme jako první
        *RootPtr = malloc(sizeof(struct tBTNode));

        if (RootPtr == NULL) { //Alokace selhala..
            return;
        }

        (*RootPtr)->Cont = Content;
        (*RootPtr)->RPtr = NULL;
        (*RootPtr)->LPtr = NULL;
    } else {

        tBTNodePtr temp = (*RootPtr);
        tBTNodePtr newNode = NULL;

        while (newNode == NULL) { //Dokud nepřidáme nový uzel
            if (Content < temp->Cont) { //Hodnota je menší, musíme do levého podstromu
                if (temp->LPtr != NULL) { //Pokud můžeme, jdeme do levého podstromu
                    temp = temp->LPtr;
                } else { //Nemá levý podstrom, takže na dané místo dáme nový uzel
                    newNode = malloc(sizeof(struct tBTNode));

                    if (newNode == NULL) { //Alokace selhala..
                        return;
                    }

                    newNode->Cont = Content;
                    newNode->RPtr = NULL;
                    newNode->LPtr = NULL;

                    temp->LPtr = newNode;
                }
            } else if (Content > temp->Cont) { //Hodnota je větší, musíme do pravého podstromu
                if (temp->RPtr != NULL) { //Pokud můžeme, jdeme do pravého podstromu
                    temp = temp->RPtr;
                } else { //Nemá pravý podstrom, takže na dané místo dáme nový uzel
                    newNode = malloc(sizeof(struct tBTNode));

                    if (newNode == NULL) { //Alokace selhala..
                        return;
                    }

                    newNode->Cont = Content;
                    newNode->RPtr = NULL;
                    newNode->LPtr = NULL;

                    temp->RPtr = newNode;
                }
            } else { //Uzel již existuje, neděláme nic
                return;
            }
        }
    }
}

/*                                  PREORDER                                  */

void Leftmost_Preorder(tBTNodePtr ptr, tStackP *Stack) {
/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/

    while (ptr != NULL) {
        SPushP(Stack, ptr); //Uložíme uzel do zásobníku
        BTWorkOut(ptr); //Zpracujeme navštívený uzel
        ptr = ptr->LPtr; //Přejdeme na další uzel doleva
    }
}

void BTPreorder(tBTNodePtr RootPtr) {
/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/

    if (RootPtr != NULL) {
        tStackP *Stack = (tStackP *) malloc(sizeof(tStackP)); //Alokujeme si zásobník
        if (Stack == NULL) { //Alokace se nezdařila!
            return;
        }

        SInitP(Stack); //Inicializujeme zásobník
        Leftmost_Preorder(RootPtr, Stack); //Do zásobníku si naházíme všechno směrem po levé větvi

        while (!SEmptyP(Stack)) { //Projedeme celý zásobník
            RootPtr = STopPopP(Stack);
            Leftmost_Preorder(RootPtr->RPtr, Stack);
        }

        free(Stack);
    }
}


/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack) {
/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku. 
**/

    while (ptr != NULL) {
        SPushP(Stack, ptr); //Uložíme uzel do zásobníku
        ptr = ptr->LPtr; //Přejdeme na další uzel doleva
    }
}

void BTInorder(tBTNodePtr RootPtr) {
/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/

    if (RootPtr != NULL) {
        tStackP *Stack = (tStackP *) malloc(sizeof(tStackP)); //Alokujeme si zásobník
        if (Stack == NULL) { //Alokace se nezdařila!
            return;
        }

        SInitP(Stack); //Inicializujeme zásobník
        Leftmost_Inorder(RootPtr, Stack); //Do zásobníku si naházíme všechno směrem po levé větvi

        while (!SEmptyP(Stack)) { //Projedeme celý zásobník
            RootPtr = STopPopP(Stack);
            BTWorkOut(RootPtr);
            Leftmost_Inorder(RootPtr->RPtr, Stack);
        }

        free(Stack);
    }
}

/*                                 POSTORDER                                  */

void Leftmost_Postorder(tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat. 
**/

    while (ptr != NULL) {
        SPushP(StackP, ptr); //Uložíme uzel do zásobníku
        SPushB(StackB, true); //Uložíme informaci, že byl uzel navšíven poprvé
        ptr = ptr->LPtr; //Přejdeme na další uzel doleva
    }
}

void BTPostorder(tBTNodePtr RootPtr) {
/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
**/

    if (RootPtr != NULL) {
        tStackP *Stack = (tStackP *) malloc(sizeof(tStackP)); //Alokujeme si zásobník
        if (Stack == NULL) { //Alokace se nezdařila!
            return;
        }

        tStackB *StackB = (tStackB *) malloc(sizeof(tStackB)); //Alokujeme si zásobník
        if (StackB == NULL) { //Alokace se nezdařila!
            return;
        }

        SInitP(Stack); //Inicializujeme zásobník
        SInitB(StackB); //Inicializujeme zásobník

        Leftmost_Postorder(RootPtr, Stack, StackB); //Do zásobníku si naházíme všechno směrem po levé větvi

        while (!SEmptyP(Stack)) { //Projedeme celý zásobník
            RootPtr = STopPopP(Stack); //Podíváme se na další prvek v zásobínku

            if (!STopPopB(StackB)) { //Prvek nebyl ještě navštáven
                BTWorkOut(RootPtr);
            } else { //Uzel již byl navštíven, vrátíme ho do zásobníků
                SPushP(Stack, RootPtr);
                SPushB(StackB, false);
                Leftmost_Postorder(RootPtr->RPtr, Stack, StackB);
            }
        }

        free(Stack);
        free(StackB);
    }
}


void BTDisposeTree(tBTNodePtr *RootPtr) {
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku ukazatelů.
**/

    if (*RootPtr != NULL) {
        tStackP *Stack = (tStackP *) malloc(sizeof(tStackP)); //Alokujeme si zásobník
        if (Stack == NULL) { //Alokace se nezdařila!
            return;
        }

        SInitP(Stack); //Inicializujeme zásobník

        SPushP(Stack, *RootPtr); //Vložíme si na zásobník vrchol stromu

        tBTNodePtr node;

        while (!SEmptyP(Stack)) { //Dokud není prázdný...
            node = STopPopP(Stack); //Podíváme se na uzel na vrcholu

            if (node->RPtr != NULL) {
                SPushP(Stack, node->RPtr); //Do zásobníku si dáme pravý poduzel
            }

            if (node->LPtr != NULL) {
                SPushP(Stack, node->LPtr); //Do zásobníku si dáme levý poduzel
            }

            free(node); //Uzel již nepotřebujeme, uvolníme ho z paměti
        }

        *RootPtr = NULL;
        free(Stack);
    }
}

/* konec c402.c */

