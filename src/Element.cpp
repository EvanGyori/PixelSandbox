#include "Element.h"

Element* Element::elements[ELEMENTS::ENUM_SIZE];

Element::Element()
{
    mass = 0.0f;
}

Element::Element(ty::ElementId _id, bool _solid, float _mass)
{
    id = _id;
    solid = _solid;
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

bool Element::isSolid()
{
    return solid;
}

float Element::getMass()
{
    return mass;
}

FallingElement::FallingElement(ty::ElementId _id, bool _solid, float _mass)
: Element(_id, _solid, _mass)
{
}

void FallingElement::initCell(Cell& cell)
{
    cell.element = id;
    if (rand() % 2 == 0) {
        cell.color = 0xdeb100;
    } else {
        cell.color = 0xffcc00;
    }
}

void FallingElement::updateCell(CellAutomaton& cellAutomaton, 
Cell& cell, int x, int y)
{
    // Down
    Element* element =
    Element::elements[cellAutomaton.getCell(x, y+1)->element];
    if (!element->isSolid() && element->getMass() < mass) {
        cellAutomaton.swapCells(x, y, x, y + 1);
        return;
    }

    // Left
    element =
    Element::elements[cellAutomaton.getCell(x-1, y+1)->element];
    if (!element->isSolid() && element->getMass() < mass) {
        cellAutomaton.swapCells(x, y, x-1, y+1);
        return;
    }

    // Right
    element =
    Element::elements[cellAutomaton.getCell(x+1, y+1)->element];
    if (!element->isSolid() && element->getMass() < mass) {
        cellAutomaton.swapCells(x, y, x+1, y+1);
        return;
    }
}

StoneElement::StoneElement(ty::ElementId _id, bool _solid, float _mass)
: Element(_id, _solid, _mass)
{
}

void StoneElement::initCell(Cell& cell)
{
    cell.element = id;
    cell.color = 0x707070;
}