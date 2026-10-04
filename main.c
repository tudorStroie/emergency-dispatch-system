/*STROIE Tudor-Andrei - 314CC*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct unit {
    int id;
    char type;
    int availability;
};

struct incident {
    int id;
    char priority[7];
    char *description;
    char status[11];
};

typedef struct nodIncident {
    struct incident elem;
    struct nodIncident *next;
    struct nodIncident *prev;
}NodIncident, *TListIncident;

struct intervention {
    struct incident *incident;
    struct unit *unit;
};

typedef struct nodIntervention {
    struct intervention elem;
    struct nodIntervention *next;
    struct nodIntervention *prev;
}NodIntervention, *TListIntervention;

struct system {
    struct unit* units;
    struct incident* incidents;
    struct intervention* interventions;
};

typedef struct cellIncident {
    struct nodIncident *elem;
    struct cellIncident *next;
} QueueCellIncident, *PQueueIncident;

typedef struct queueIncident {
    PQueueIncident front;
    PQueueIncident rear;
} TQueueIncident;

/*Initializeaza lista dublu inlantuita circulara de incidente cu un nod santinela*/

void initListIncident (struct system *sys, TListIncident *sentinel) {
    *sentinel = malloc(sizeof(NodIncident));
    if (*sentinel == NULL) {
        printf("Eroare alocare sentinel\n");
        exit(1);
    }
    (*sentinel)->elem.id = 0;
    (*sentinel)->elem.description = malloc(strlen("test incident") + 1);
    strcpy((*sentinel)->elem.description, "test incident");
    strcpy((*sentinel)->elem.priority, "low");
    strcpy((*sentinel)->elem.status, "solved");
    (*sentinel)->next = *sentinel;
    (*sentinel)->prev = *sentinel;

    sys->incidents = &((*sentinel)->elem);
}

/*Initializeaza lista dublu inlantuita circulara de interventii cu un nod santinela*/

void initListIntervention (struct system *sys, TListIntervention *sentinel) {
    *sentinel = malloc(sizeof(NodIntervention));
    if (*sentinel == NULL) {
        printf("Eroare alocare sentinel\n");
        exit(1);
    }
    (*sentinel)->elem.incident = NULL;
    (*sentinel)->elem.unit = NULL;
    (*sentinel)->next = *sentinel;
    (*sentinel)->prev = *sentinel;

    sys->interventions = &((*sentinel)->elem);

}

/*Initializeaza o coada de incidente cu front si rear NULL*/

void initQueueIncident (TQueueIncident *q) {
    q->front = NULL;
    q->rear = NULL;
}

/*Adauga un nod de incident la finalul cozii*/

void enqueueIncident (TQueueIncident *q, TListIncident element) {
    PQueueIncident p = malloc(sizeof(QueueCellIncident));
    if (p == NULL){
        printf("Eroare\n");
        exit(1);
    }
    p->elem = element;
    p->next = NULL;
    
    if (q->front == NULL) {
        q->front = p;
        q->rear = p;
    }
    else {
        q->rear->next = p;
        q->rear = p;
    }
}

/*Extrage primul nod din coada de incidente*/

void dequeueIncident (TQueueIncident *q, TListIncident *val) {
    PQueueIncident temp;
    if (q->front == NULL) {
        *val = NULL;
        return;
    }
    *val = q->front->elem;
    if(q->front == q->rear) {
        temp = q->front;
        q->front = NULL;
        q->rear = NULL;
        free(temp);
    }
    else {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
}

/*Adauga un incident nou in lista si in coada corespunzatoare prioritatii sale*/

void add_incident (struct system *sys, int id, char *priority, char*description, TListIncident sentinel, TQueueIncident *queue_high, TQueueIncident *queue_medium, TQueueIncident *queue_low) {
    TListIncident nou = malloc(sizeof(NodIncident));
    if(nou == NULL) {
        printf("Eroare\n");
        exit(1);
    }
    nou->elem.id = id;
    nou->elem.description = malloc(strlen(description) + 1);
    strcpy(nou->elem.description, description);
    strcpy(nou->elem.priority, priority);
    strcpy(nou->elem.status, "queued");

    TListIncident temp = sentinel->prev;
    nou->next = sentinel;
    nou->prev = temp;
    temp->next = nou;
    sentinel->prev = nou;

    if(strcmp(priority, "high") == 0)
        enqueueIncident(queue_high, nou);
    else if(strcmp(priority, "medium") == 0)
        enqueueIncident(queue_medium, nou);
    else
        enqueueIncident(queue_low, nou);
}

typedef struct cellUnit {
    struct unit *elem;
    struct cellUnit *next;
} QueueCellUnit, *PQueueUnit;

typedef struct queueUnit {
    PQueueUnit front;
    PQueueUnit rear;
} TQueueUnit;

/*Initializeaza o coada de echipaje cu front si rear NULL*/

void initQueueUnit (TQueueUnit *q) {
    q->front = NULL;
    q->rear = NULL;
}

/*Adauga un echipaj la finalul cozii de echipaje disponibile*/

void enqueueUnit (TQueueUnit *q, struct unit *element) {
    PQueueUnit p = malloc(sizeof(QueueCellUnit));
   if (p == NULL){
        printf("Eroare\n");
        exit(1);
    }
    p->elem = element;
    p->next = NULL;
    
    if (q->front == NULL) {
        q->front = p;
        q->rear = p;
    }
    else {
        q->rear->next = p;
        q->rear = p;
    }
}

/*Extrage primul echipaj din coada de echipaje disponibile*/

void dequeueUnit (TQueueUnit *q, struct unit **val) {
    PQueueUnit temp;
    if (q->front == NULL) {
        *val = NULL;
        return;
    }
    *val = q->front->elem;
    if(q->front == q->rear) {
        temp = q->front;
        q->front = NULL;
        q->rear = NULL;
        free(temp);
    }
    else {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
}

/*Afiseaza numarul de echipaje disponibile din coada*/

void check_units_availability (TQueueUnit *q, FILE *fout) {
    int k = 0;
    PQueueUnit temp = q->front;
    while (temp != NULL) {
        k++;
        temp = temp->next;
    }
    fprintf(fout, "Number of available units: %d\n", k);
}

typedef struct cellStack {
    TListIntervention elem;
    struct cellStack *next;
} StackCell, *TStack;

/*Initializeaza stiva de dispatch cu head NULL*/

void initStack (TStack *head) {
    *head = NULL;
}

void push(TStack *head, TListIntervention elem) {
    TStack temp = malloc(sizeof(StackCell));
    if (temp == NULL) {
        printf("Eroare alocare\n");
        exit(1);
    }
    temp->elem = elem;
    temp->next = *head;
    *head = temp;
}

void pop (TStack *head, TListIntervention *val) {
    TStack temp;
    if (*head == NULL) {
        *val = NULL;
        return;
    }
    *val = (*head)->elem;
    temp = *head;
    *head = (*head)->next;
    free(temp);

}

/*Trimite cel mai prioritar incident (in ordinea high, medium, low) catre primul echipaj disponibil
si retine interventia in lista si in sitva*/

void dispatch (struct system *sys, TQueueIncident *queue_high, TQueueIncident *queue_medium, TQueueIncident *queue_low, TQueueUnit *available_units, TListIntervention sentinel, TStack *stack_dispatch, FILE *fout) {
    TListIncident incident = NULL;
    if ((queue_high->front == NULL && queue_medium->front == NULL && queue_low->front == NULL) || available_units->front == NULL) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
        return;
    } 
    if (queue_high->front != NULL)
        dequeueIncident (queue_high, &incident);
    else if (queue_medium->front != NULL)
        dequeueIncident (queue_medium, &incident);
    else if (queue_low->front != NULL)
        dequeueIncident (queue_low, &incident);
    struct unit *u;
    dequeueUnit(available_units, &u);
    TListIntervention interventii = malloc(sizeof(NodIntervention));
    if (interventii == NULL) {
        printf("Eroare alocare\n");
        exit(1);
    }
    interventii->elem.incident = &(incident->elem);
    interventii->elem.unit = u;
    TListIntervention tail = sentinel->prev;
    interventii->next = sentinel;
    interventii->prev = tail;
    tail->next = interventii;
    sentinel->prev = interventii;
    strcpy(incident->elem.status, "intervened");
    u->availability = 0;
    push(stack_dispatch, interventii);

}

/*Adauga un nod de incident la incputul cozii (comanda undo)*/

void enqueueFrontIncident (TQueueIncident *q, TListIncident element) {
    PQueueIncident temp = malloc(sizeof(QueueCellIncident));
    if (temp == NULL) {
        printf("Eroare alocare\n");
        exit(1);
    }
    temp->elem = element;
    temp->next = q->front;
    if (q->front == NULL) {
        q->front = temp;
        q->rear = temp;
    }
    else 
        q->front = temp;
}

/*Anuleaza ultima interventie activa, pune incidentul inapoi in capul cozii
si echipajul respectiv in coada de echipaje disponibile, apoi sterge
nodul de interventie*/

void undo_last_dispatch (TStack *s, TListIncident sentinel, TQueueIncident *queue_high, TQueueIncident *queue_medium, TQueueIncident *queue_low, TQueueUnit *available_units, FILE *fout) {
    TListIntervention interventii = NULL;
    while (*s != NULL) {
        pop(s, &interventii);
        if (!strcmp(interventii->elem.incident->status, "intervened"))
            break;
        interventii = NULL;
    }
    if (interventii == NULL){
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
        return;
    }
    struct incident *incident = interventii->elem.incident;
    struct unit *u = interventii->elem.unit;
    TListIncident nod_incident = sentinel->next;
    while (nod_incident != sentinel) {
        if (&(nod_incident->elem) == incident)
            break;
        nod_incident = nod_incident->next;
    }
    strcpy(incident->status, "queued");
    if (!strcmp(incident->priority, "high"))
        enqueueFrontIncident(queue_high, nod_incident);
    else if (!strcmp(incident->priority, "medium"))
        enqueueFrontIncident(queue_medium, nod_incident);
    else if (!strcmp(incident->priority, "low"))
        enqueueFrontIncident(queue_low, nod_incident);
    u->availability = 1;
    enqueueUnit(available_units, u);
    interventii->prev->next = interventii->next;
    interventii->next->prev = interventii->prev;
    free(interventii);

}

/*Marcheaza un incident ca rezolvat si elibereaza echipajul asociat*/

void solved_incident (int id, TListIncident sentinel_incident, TListIntervention sentinel_interventii, TQueueUnit *available_units, FILE *fout) {
    TListIncident nod_incident = sentinel_incident->next;
    while (nod_incident != sentinel_incident) {
        if (nod_incident->elem.id == id)
            break;
        nod_incident = nod_incident->next;
    }
    if (nod_incident == sentinel_incident) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
        return;
    }
    if (strcmp(nod_incident->elem.status, "intervened")) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
        return;
    }
    strcpy(nod_incident->elem.status, "solved");
    TListIntervention nod_interventii = sentinel_interventii->next;
    while (nod_interventii != sentinel_interventii) {
        if (nod_interventii->elem.incident == &(nod_incident->elem))
            break;
        nod_interventii = nod_interventii->next;
    }
    struct unit *u = nod_interventii->elem.unit;
    u->availability = 1;
    enqueueUnit(available_units, u);
}

/*Afiseaza tipul si disponibilitatea unui echipaj dupa id*/

void show_unit (struct system *sys, int n_unit, int id, FILE *fout) {
    for (int i = 0; i < n_unit; i++) {
        if (sys->units[i].id == id) {
            if (sys->units[i].availability == 1)
                fprintf(fout, "Unit %d is type %c and is available\n", sys->units[i].id, sys->units[i].type);
            else
                fprintf(fout, "Unit %d is type %c and is unavailable\n", sys->units[i].id, sys->units[i].type);
            return;
        }
    }
    fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

/*Afiseaza detaliile unui incident dupa id*/

void show_incident (TListIncident sentinel, int id, FILE *fout) {
    TListIncident incident = sentinel->next;
    while (incident != sentinel) {
        if(incident->elem.id == id) {
            fprintf(fout, "Incident %d has %s priority, the following description: \"%s\" and is %s\n", incident->elem.id, incident->elem.priority, incident->elem.description, incident->elem.status);
            return;
        }
        incident = incident->next;
    }
    fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

/*Afiseaza toate interventiile din lista*/

void show_interventions (TListIntervention sentinel, FILE *fout) {
    if (sentinel->next == sentinel) {
        fprintf(fout, "No intervention has been initiated\n");
        return;
    }
    TListIntervention interventii = sentinel->next;
    while (interventii != sentinel) {
        fprintf(fout, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n", interventii->elem.incident->id, interventii->elem.unit->id, interventii->elem.incident->status);
        interventii = interventii->next;
    }
}

/*Urmatoarele functii elibereaza memoria alocata pentru fiecare dintre nodurile din listele folosite anterior,
respectiv fiecare dintre celulele din cozile si stiva folosite anterior */

void freeListIncident (TListIncident sentinel) {
    TListIncident current = sentinel->next;
    while (current != sentinel) {
        TListIncident temp = current->next;
        free(current->elem.description);
        free(current);
        current = temp;
    }
    free(sentinel->elem.description);
    free(sentinel);
}

void freeListInterventions (TListIntervention sentinel) {
    TListIntervention current = sentinel->next;
    while (current != sentinel) {
        TListIntervention temp = current->next;
        free(current);
        current = temp;
    }
    free(sentinel);
}

void freeQueueIncident (TQueueIncident *q) {
    PQueueIncident current = q->front;
    while (current != NULL) {
        PQueueIncident temp = current->next;
        free(current);
        current = temp;
    }
}

void freeQueueUnit (TQueueUnit *q) {
    PQueueUnit current = q->front;
    while (current != NULL) {
        PQueueUnit temp = current->next;
        free(current);
        current = temp;
    }
}

void freeStack (TStack *head) {
    TStack current = *head;
    while (current != NULL) {
        TStack temp = current->next;
        free(current);
        current = temp;
    }
}

 int main () {
    FILE *fin = fopen("tema1.in", "r");
    FILE *fout = fopen("tema1.out", "w");
    struct system sys;
    int n_units;
    fscanf(fin, "%d", &n_units);
    sys.units = malloc(n_units * sizeof(struct unit));
    for (int i = 0; i < n_units; i++) {
        fscanf(fin, "%d %c", &sys.units[i].id, &sys.units[i].type);
        sys.units[i].availability = 1;
    }
    TListIncident sentinel_incident;
    TListIntervention sentinel_interventii;
    TQueueIncident queue_high, queue_medium, queue_low;
    TQueueUnit available_units;
    TStack stack_dispatch;

    initListIncident(&sys, &sentinel_incident);
    initListIntervention(&sys, &sentinel_interventii);
    initQueueIncident(&queue_high);
    initQueueIncident(&queue_medium);
    initQueueIncident(&queue_low);
    initQueueUnit(&available_units);
    initStack(&stack_dispatch);

    for (int i = 0; i < n_units; i++)
        enqueueUnit(&available_units, &sys.units[i]);
    
    int n_operations;
    fscanf(fin, "%d", &n_operations);
    char command[50];
    for (int i = 0; i < n_operations; i++) {
        fscanf(fin, "%s", command);
        if (!strcmp(command, "ADD_INCIDENT")) {
            int id;
            char priority[7];
            char description[200];
            char line[256];
            fgets(line, sizeof(line), fin);
            sscanf(line, " %d %s", &id, priority);
            char *p1 = strchr(line, '\"');
            char *p2 = strchr(p1+1, '\"');
            int l = p2 - (p1 + 1);
            memcpy(description, p1 + 1, l);
            description[l] = '\0';
            add_incident(&sys, id, priority, description, sentinel_incident, &queue_high, &queue_medium, &queue_low);

        }
        else if (!strcmp(command, "CHECK_UNITS_AVAILABILITY")) {
            check_units_availability(&available_units, fout);
        } 
        else if (!strcmp(command, "DISPATCH")) {
            dispatch(&sys, &queue_high, &queue_medium, &queue_low, &available_units, sentinel_interventii, &stack_dispatch, fout);
        } 
        else if (!strcmp(command, "UNDO_LAST_DISPATCH")) {
            undo_last_dispatch(&stack_dispatch, sentinel_incident, &queue_high, &queue_medium, &queue_low, &available_units, fout);
        } 
        else if (!strcmp(command, "SOLVED_INCIDENT")) {
            int id;
            fscanf(fin, "%d", &id);
            solved_incident(id, sentinel_incident, sentinel_interventii, &available_units, fout);
        }
        else if (!strcmp(command, "SHOW_UNIT")) {
            int id;
            fscanf(fin, "%d", &id);
            show_unit(&sys, n_units, id, fout);
        } 
        else if (!strcmp(command, "SHOW_INCIDENT")) {
            int id;
            fscanf(fin, "%d", &id);
            show_incident(sentinel_incident, id, fout);
        }
        else if (!strcmp(command, "SHOW_INTERVENTIONS")) {
            show_interventions(sentinel_interventii, fout);
        } 
    }

    fclose(fin);
    fclose(fout);
    free(sys.units);
    freeListIncident(sentinel_incident);
    freeListInterventions(sentinel_interventii);
    freeQueueIncident(&queue_high);
    freeQueueIncident(&queue_medium);
    freeQueueIncident(&queue_low);
    freeQueueUnit(&available_units);
    freeStack(&stack_dispatch);
    return 0;
 }
