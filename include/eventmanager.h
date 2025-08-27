#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#define EM_INITIAL_CAPACITY 32

#include <stddef.h>
#include <stdbool.h>

typedef void (*EventFn)(void* userData, void* eventData);

typedef struct EventManager EventManager;

EventManager* EventManagerCreate(void);
void EventManagerFree(EventManager* em);
void EventManagerClear(EventManager* em);
void* EventManagerSubscribe(EventManager* em, EventFn fn, void* userData);
void EventManagerUnsubscribe(EventManager* em, void* handle);
void EventManagerUnsubscribeAllForUserData(EventManager* em, void* userData);
void EventManagerDispatch(EventManager* em, void* eventData);

#endif

