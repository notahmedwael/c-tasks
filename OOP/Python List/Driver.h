#ifndef DRIVER_H
#define DRIVER_H

#include "List.h"

void displayMenu();
void handleAppend(List& list);
void handleInsert(List& list);
void handleRemove(List& list);
void handleParse(List& list);
void handleGet(List& list);
void run();

#endif