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
    STONE,
    ENUM_SIZE
};

class CellAutomaton;
class Element
{
public:
    // Stop removing pointer, is needed for polymorphism
    static Element* elements[ELEMENTS::ENUM_SIZE];

    Element();
    Element(ty::ElementId _id, bool _solid, float _mass);
    virtual ~Element();

    virtual void initCell(Cell& cell);

    // x and y are relative to the chunk the cell started in
    virtual void updateCell(CellAutomaton& cellAutomaton, 
    Cell& cell, int x, int y, float deltaTime);

    bool isSolid();
    float getMass();

protected:
    ty::ElementId id;
    bool solid;
    float mass;
};

class FallingElement : public Element
{
public:
    FallingElement(ty::ElementId _id, bool _solid, float _mass);

    virtual void initCell(Cell& cell);

    virtual void updateCell(CellAutomaton& CellAutomaton,
    Cell& cell, int x, int y, float deltaTime);
};

class StoneElement : public Element
{
public:
    StoneElement(ty::ElementId _id, bool _solid, float _mass);

    virtual void initCell(Cell& cell);
};
