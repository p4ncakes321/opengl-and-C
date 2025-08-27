#include "eventmanager.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct Listener {
    EventFn fn;
    void* userData;
    bool removed;
} Listener;

struct EventManager {
    Listener** list;
    size_t count;
    size_t capacity;
    int inDispatch;
    bool needsCleanup;
};

static void em_grow_if_needed(EventManager* em) {
    if (em->count < em->capacity) return;
    em->capacity *= 2;
    em->list = (Listener**)realloc(em->list, em->capacity * sizeof(Listener*));
}

static void em_remove(EventManager* em, size_t i) {
    Listener* target = em->list[i];
    Listener* last = em->list[em->count - 1];
    if (last != target) em->list[i] = last;
    em->count--;
    free(target);
}

static void em_cleanup(EventManager* em) {
    if (!em->needsCleanup) return;
    size_t write = 0;
    for (size_t read = 0, n = em->count; read < n; ++read) {
        Listener* l = em->list[read];
        if (l->removed) { free(l); continue; }
        em->list[write++] = l;
    }
    em->count = write;
    em->needsCleanup = false;
}

EventManager* EventManagerCreate(void) {
    EventManager* em = malloc(sizeof(EventManager));
    if (!em) return NULL;
    em->list = malloc(EM_INITIAL_CAPACITY * sizeof(Listener*));
    if (!em->list) { free(em); return NULL; }
    em->count = 0;
    em->capacity = EM_INITIAL_CAPACITY;
    em->inDispatch = 0;
    em->needsCleanup = false;
    return em;
}

void EventManagerFree(EventManager* em) {
    if (!em) return;
    for (size_t i = 0, n = em->count; i < n; ++i) free(em->list[i]);
    free(em->list);
    free(em);
}

void EventManagerClear(EventManager* em) {
    if (!em) return;
    for (size_t i = 0, n = em->count; i < n; ++i) free(em->list[i]);
    em->count = 0;
    em->inDispatch = 0;
    em->needsCleanup = false;
}

void* EventManagerSubscribe(EventManager* em, EventFn fn, void* userData) {
    if (!em || !fn) return NULL;
    Listener* l = malloc(sizeof(Listener));
    if (!l) return NULL;
    l->fn = fn;
    l->userData = userData;
    l->removed = false;
    em_grow_if_needed(em);
    em->list[em->count++] = l;
    return l;
}

void EventManagerUnsubscribe(EventManager* em, void* handle) {
    if (!em || !handle) return;
    Listener* target = handle;
    for (size_t i = 0, n = em->count; i < n; ++i) {
        if (em->list[i] == target) {
            if (em->inDispatch) {
                if (!target->removed) { target->removed = true; target->fn = NULL; em->needsCleanup = true; }
            } else em_remove(em, i);
            return;
        }
    }
}

void EventManagerUnsubscribeAllForUserData(EventManager* em, void* userData) {
    for (size_t i = 0; i < em->count;) {
        Listener* l = em->list[i];
        if (l->userData == userData && !l->removed) {
            if (em->inDispatch) { l->removed = true; l->fn = NULL; em->needsCleanup = true; ++i; }
            else { em_remove(em, i); }
        } else ++i;
    }
}

void EventManagerDispatch(EventManager* em, void* eventData) {
    if (!em || !em->count) return;
    em->inDispatch++;
    for (size_t i = 0, n = em->count; i < n; ++i) {
        Listener* l = em->list[i];
        if (!l->removed && l->fn) l->fn(l->userData, eventData);
    }
    em->inDispatch--;
    if (!em->inDispatch && em->needsCleanup) em_cleanup(em);
}

