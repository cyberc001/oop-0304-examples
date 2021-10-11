#include "cell_object.h"

#include "cells/cell.h"

bool cell_object::hasParentCell() { return parent_cell != nullptr; }
cell& cell_object::getParentCell() { return *parent_cell; }
void cell_object::setParentCell(cell* c) { parent_cell = c; }
