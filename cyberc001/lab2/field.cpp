#include "field.h"

#include "cells/cell_entrance.h"
#include "cells/cell_exit.h"
#include "cells/cell_view.h"

#include "actors/actor.h"

#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <ctime>

field::field(const field& other)
{
	cells_w = other.cells_w; cells_h = other.cells_h;

	cells = new cell*[cells_w*cells_h];
	std::memcpy(cells, other.cells, cells_w*cells_h*sizeof(cell));
}
field::field(field&& other)
{
	std::swap(cells_w, other.cells_w);
	std::swap(cells_h, other.cells_h);

	std::swap(cells, other.cells);
}
field::~field()
{
	if(cells)
		delete[] cells;
}
field& field::operator=(const field& other)
{
	if(this == &other)
		return *this;

	set_size(other.cells_w, other.cells_h);
	std::memcpy(cells, other.cells, cells_w*cells_h*sizeof(cell));

	return *this;
}
field& field::operator=(field&& other)
{
	if(this == &other)
		return *this;

	std::swap(cells_w, other.cells_w);
	std::swap(cells_h, other.cells_h);

	std::swap(cells, other.cells);

	return *this;
}

field::field(size_t w, size_t h) : cells_w(w), cells_h(h)
{
	if(w > 0 && h > 0) 	cells = new cell*[w*h];
	else 			cells = nullptr;

	for(size_t i = 0; i < cells_w * cells_h; ++i)
		cells[i] = new cell(this, i % w, i / w, true);
}


// Basic manipulation methods

size_t field::get_w() const { return cells_w; }
size_t field::get_h() const { return cells_h; }
void field::set_size(size_t w, size_t h)
{
	cells_w = w; cells_h = h;
	delete[] cells;
	cells = new cell*[w*h];
}

cell& field::get_cell(size_t x, size_t y) const
{
	if(x >= cells_w || y >= cells_h)
		throw "coordinates are out of bounds";
	return *(cells[y*cells_h + x]);
}
void field::set_cell(size_t x, size_t y, cell* c)
{
	delete cells[y*cells_h + x];
	cells[y*cells_h + x] = c;
}


// Generation

cell_entrance* field::generate_entrance_exit(size_t min_dist)
{
	if(cells_w == 0 || cells_h == 0)
		return nullptr;

	std::srand(std::time(nullptr));

	size_t ex = std::rand() % cells_w, ey = std::rand() % cells_h;
	cell_entrance* ce = new cell_entrance(this, ex, ey, true);
	set_cell(ex, ey, ce);
	size_t x, y;
	if(cells_w > 3 && cells_h > 3){
		min_dist = std::min(min_dist, std::max(ex, cells_w - 1 - ex) + std::max(ey, cells_h - 1 - ey));

		for(x = std::rand() % cells_w, y = std::rand() % cells_h;
			(x > ex ? x - ex : ex - x) + (y > ey ? y - ey : ey - y) < min_dist;
			x = std::rand() % cells_w, y = std::rand() % cells_h) {}
	}
	else if(cells_w > 1 && cells_h > 1)
		for(x = std::rand() % cells_w, y = std::rand() % cells_h;
			x == ex || y == ey;
			x = std::rand() % cells_w, y = std::rand() % cells_h) {}
	else
		x = 0, y = 0;

	set_cell(x, y, new cell_exit(this, x, y, true));
	return ce;
}


// Gamesim

void field::tickActors()
{
	std::vector<actor*> actors;

	for(size_t i = 0; i < cells_w*cells_h; ++i)
	{
		if(!cells[i]->has_object()) continue;
		actor* a = dynamic_cast<actor*>(&cells[i]->get_object());
		if(!a) continue;

		actors.push_back(a);
	}

	for(size_t i = 0; i < actors.size(); ++i)
		actors[i]->onTurn();

	actors.clear();
}
