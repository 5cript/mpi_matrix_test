#pragma once

/** 
 *	Matrix View Concept.
 *	Do not actually derive from this, use templates instead.
 *
 *	A matrix view is a non owning view of data to some form of data representation.
 */
class MatrixView
{
	MatrixView() = default;
	virtual ~MatrixView() = default;

	virtual void get_line(int y) = 0;

	virtual int dimension() const = 0;
};
