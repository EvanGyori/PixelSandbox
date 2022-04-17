#include "Element.h"

Element* Element::elements[ELEMENTS::ENUM_SIZE];

Element::Element()
{
    mass = 0.0f;
}

Element::Element(ty::ElementId _id, float _mass)
{
    id = _id;
    mass = _mass;
}

Element::~Element()
{
}

void Element::initCell(Cell& cell)
{
    cell.element = id;
    cell.color = 0x000000;
}

void Element::updateCell(CellAutomaton& cellAutomaton, 
Cell& cell, int x, int y)
{
}

Sand::Sand(ty::ElementId _id, float _mass)
: Element(_id, _mass)
{
}

Sand::~Sand()
{
}

void Sand::initCell(Cell& cell)
{
    cell.element = id;
    cell.color = 0xffffff;
}

void Sand::updateCell(CellAutomaton& cellAutomaton, 
Cell& cell, int x, int y)
{
    cellAutomaton.swapCells(x, y, x, y + 1);
}