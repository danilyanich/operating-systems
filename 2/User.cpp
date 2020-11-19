//
// Created by nicolas on 24/10/2020.
//

#include <cstdio>
#include "User.h"
void User::leave() const
{
    printf("User%d:Leave\n", ID);
}

void User::barbering() const
{
    printf("User%d:Barbering\n", ID);
}

void User::stopBarbering() const
{
    printf("User%d:Done\n", ID);
}

void User::inQueue() const
{
    printf("User%d:AtQueue\n", ID);
}
