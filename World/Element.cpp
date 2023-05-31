#include "Element.h"

#include <cmath>
#include <iostream>

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

// Only works with positive velocities currently
void Element::updateCell(CellAutomaton& cellAutomaton, 
Cell& cell, int x, int y, float deltaTime)
{
	if (cell.vx == 0 && cell.vy == 0) return;
	
	float dx = cell.vx * deltaTime + cell.progressX;
	float dy = cell.vy * deltaTime + cell.progressY;
	
	float mx = 1;
	float my = 1;
	if (dx == 0) {
		mx = 0;
	} else if (dy == 0) {
		my = 0;
	} else if (dx > dy) {
		my = dy / dx;
	} else if (dy > dx) {
		mx = dx / dy;
	}
	
	while (dx >= 1 || dy >= 1) {
		int targetX = x + mx;
		int targetY = y + my;
		Element* targetCell = Element::elements[cellAutomaton.getCell(targetX, targetY)->element];
		if (targetCell != nullptr && !targetCell->isSolid() && targetCell->getMass() < mass) {
			cellAutomaton.swapCells(x, y, targetX, targetY);
			x = targetX;
			y = targetY;
		}
		
		dx -= mx;
		dy -= my;
	}
	
	cell.progressX = dx;
	cell.progressY = dy;
	
	/*
	int endX = x + floor(cell.vx * deltaTime);
	int endY = y + floor(cell.vy * deltaTime);
	float progress = 0.0f;
	float step;
	if (endX > endY) {
		step = 1.0f/abs(endX - x);
	} else {
		step = 1.0f/abs(endY - y);
	}
	
	float dx = 0.0f;
	float dy = 0.0f;
	
	while (progress < 1.0f) {
		progress += step;
		dx += step * (endX - x);
		dy += step * (endY - y);
		int targetX = x;
		int targetY = y;
		if (dx >= 1.0f) {
			dx--;
			targetX++;
		} else if (dx <= -1.0f) {
			dx++;
			targetX--;
		}
		if (dy >= 1.0f) {
			dy--;
			targetY++;
		} else if (dy <= -1.0f) {
			dy++;
			targetY--;
		}
		
		if (targetX != x || targetY != y) { // redundant
			Element* targetCell = Element::elements[cellAutomaton.getCell(targetX, targetY)->element];
			if (targetCell != nullptr && !targetCell->isSolid() && targetCell->getMass() < mass) {
				cellAutomaton.swapCells(x, y, targetX, targetY);
				x = targetX;
				y = targetY;
			} else {
				progress = 1.0f;
			}
		}
	}
	*/
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
Cell& cell, int x, int y, float deltaTime)
{
	int order[] = {0, -1, 1};
	if (rand() % 2 == 0) {
		order[1] = 1;
		order[2] = -1;
	}
	
	// Down and down to the left and right
	for (int i = 0; i < 3; i++) {
		int targetX = x + order[i];
		int targetY = y + 1;
		Element* targetCell = Element::elements[cellAutomaton.getCell(targetX, targetY)->element];
		if (!targetCell->isSolid() && targetCell->getMass() < mass) {
		    cellAutomaton.swapCells(x, y, targetX, targetY);
		    break;
		}
	}
	
	//cell.vy += 0.001f * deltaTime;
	//cell.vy = 1.0f;
	
	//Element::updateCell(cellAutomaton, cell, x, y, deltaTime);
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
