#ifndef CELL_OBJECT_H
#define CELL_OBJECT_H

class cell;

class cell_object
{
	public:

	virtual char getDisplay(cell& holder) = 0;
	virtual bool hasParentCell();
	virtual cell& getParentCell();
	virtual void setParentCell(cell* c);

	// Events
	virtual void onAdd(cell& holder) = 0;


	private:

	cell* parent_cell;
};

#endif
