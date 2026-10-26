#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE -1

struct _Person;
typedef struct _Person* Position;

typedef struct _Person {
    char name[50];
    char lastname[50];
    int birthYear;
    Position next;
} Person;

// Deklaracije funkcija
Position CreatePerson(const char* name, const char* lastname, int birthYear);
int PrintList(Position first);
int AddAfter(Position head, const char* search, const char* name, const char* lastname, int birthYear);
int AddBefore(Position head, const char* search, const char* name, const char* lastname, int birthYear);
int SortList(Position head);
int WriteToFile(const char* filePath, Position first);
int ReadFromFile(const char* filePath, Position head);

// Pomoćne funkcije
static Position FindPersonByLastName(Position first, const char* lastname) {
    Position temp = first;
    while (temp != NULL) {
        if (strcmp(temp->lastname, lastname) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

static Position FindPrevious(Position head, const char* lastname) {
    Position temp = head;
    while (temp->next != NULL) {
        if (strcmp(temp->next->lastname, lastname) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

static int AddToList(Position position, Position newPerson) {
    newPerson->next = position->next;
    position->next = newPerson;
    return SUCCESS;
}

// Kreiranje novog elementa
Position CreatePerson(const char* name, const char* lastname, int birthYear) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) {
        return NULL;
    }
    strcpy(newPerson->name, name);
    strcpy(newPerson->lastname, lastname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;

    return newPerson;
}

// Ispis liste
int PrintList(Position first) {
    Position temp = NULL;
    if (first == NULL) {
        printf("Lista je prazna!\n");
        return FAILURE;
    }
    for (temp = first; temp != NULL; temp = temp->next) {
        printf("%s %s %d\n", temp->name, temp->lastname, temp->birthYear);
    }
    return SUCCESS;
}

// A. Dinamički dodaje novi element iza određenog elementa
int AddAfter(Position head, const char* search, const char* name, const char* lastname, int birthYear) {
    Position oldPerson = FindPersonByLastName(head->next, search);
    Position newPerson = CreatePerson(name, lastname, birthYear);

    if (oldPerson) {
        AddToList(oldPerson, newPerson);
        return SUCCESS;
    }
    else {
        free(newPerson);
        return FAILURE;
    }
}

// B. Dinamički dodaje novi element ispred određenog elementa
int AddBefore(Position head, const char* search, const char* name, const char* lastname, int birthYear) {
    Position oldPerson = FindPrevious(head, search);
    Position newPerson = CreatePerson(name, lastname, birthYear);

    if (oldPerson) {
        AddToList(oldPerson, newPerson);
        return SUCCESS;
    }
    else {
        free(newPerson);
        return FAILURE;
    }
}

// C. Sortira listu po prezimenima osoba
int SortList(Position head) {
    Position i, j, last;
    char tempName[50], tempLastname[50];
    int tempYear;

    if (head->next == NULL) {
        return SUCCESS;
    }

    last = NULL;
    int swapped;

    do {
        swapped = 0;
        i = head;
        while (i->next != last && i->next->next != NULL) {
            if (strcmp(i->next->lastname, i->next->next->lastname) > 0) {
                Position p1 = i->next;
                Position p2 = i->next->next;

                strcpy(tempName, p1->name);
                strcpy(tempLastname, p1->lastname);
                tempYear = p1->birthYear;

                strcpy(p1->name, p2->name);
                strcpy(p1->lastname, p2->lastname);
                p1->birthYear = p2->birthYear;

                strcpy(p2->name, tempName);
                strcpy(p2->lastname, tempLastname);
                p2->birthYear = tempYear;

                swapped = 1;
            }
            i = i->next;
        }
        last = i->next;
    } while (swapped);

    return SUCCESS;
}

// D. Upisuje listu u datoteku
int WriteToFile(const char* filePath, Position first) {
    FILE* file = fopen(filePath, "w");
    if (!file) {
        printf("Greska pri otvaranju datoteke za pisanje!\n");
        return FAILURE;
    }

    Position temp = first;
    while (temp != NULL) {
        fprintf(file, "%s %s %d\n", temp->name, temp->lastname, temp->birthYear);
        temp = temp->next;
    }

    fclose(file);
    return SUCCESS;
}

// E. Čita listu iz datoteke
int ReadFromFile(const char* filePath, Position head) {
    FILE* file = NULL;
    char buffer[1024] = { 0 };
    char name[50] = { 0 };
    char lastname[50] = { 0 };
    int birthYear = 0;
    int n = 0;

    file = fopen(filePath, "r");
    if (!file) {
        printf("Can not find or open file.\n");
        return FAILURE;
    }

    while (fgets(buffer, 1024, file)) {
        n = sscanf(buffer, "%s %s %d", name, lastname, &birthYear);
        if (n == 3) {
            Position current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            Position newPerson = CreatePerson(name, lastname, birthYear);
            AddToList(current, newPerson);
        }
    }

    fclose(file);
    return SUCCESS;
}

// Glavna funkcija (Main) koja pokreće program
int main() {
    Position head = CreatePerson("", "", 0); // Kreiramo head čvor

    // Testni unos par osoba da odmah vidiš ispis
    Position p1 = CreatePerson("Ivan", "Ivic", 1998);
    Position p2 = CreatePerson("Ana", "Anic", 2000);
    AddToList(head, p1);
    AddToList(head, p2);

    printf("Inicijalna lista:\n");
    PrintList(head->next);

    // Test funkcije A (Dodaj iza Anić)
    AddAfter(head, "Anic", "Marko", "Maric", 1995);
    printf("\nNakon AddAfter (Marko Maric iza Anic):\n");
    PrintList(head->next);

    // Test funkcije C (Sortiranje)
    SortList(head);
    printf("\nNakon sortiranja liste:\n");
    PrintList(head->next);

    // Test funkcije D (Upis u datoteku)
    WriteToFile("popis.txt", head->next);
    printf("\nLista je uspjesno zapisana u 'popis.txt'.\n");

    return 0;
}