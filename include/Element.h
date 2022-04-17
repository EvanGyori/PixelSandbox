#pragma once

#include <cstdint>
#include <iostream>

#include "CellAutomaton.h"
#include "Settings.h"
#include "Cell.h"

enum ELEMENTS
{
    EMPTY,
    SAND,
    ENUM_SIZE
};

class CellAutomaton;
class Element
{
public:
    // Stop removing pointer, is needed for polymorphism
    static Element* elements[ELEMENTS::ENUM_SIZE];

    Element();
    Element(ty::ElementId _id, float _mass);
    virtual ~Element();

    virtual void initCell(Cell& cell);

    // x and y are relative to the chunk the cell started in
    virtual void updateCell(CellAutomaton& cellAutomaton, 
    Cell& cell, int x, int y);

protected:
    ty::ElementId id;
    float mass;
};

class Sand : public Element
{
public:
    Sand(ty::ElementId _id, float _mass);
    virtual ~Sand();

    virtual void initCell(Cell& cell);

    virtual void updateCell(CellAutomaton& CellAutomaton,
    Cell& cell, int x, int y);
};