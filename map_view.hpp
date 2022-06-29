#pragma once
#include <vector>
#include <QGraphicsView>
#include <QMouseEvent>
#include "tiff_tiled_file.hpp"
#include "tile_item.hpp"
#include "tile_grid.hpp"

class map_graphics_view : public QGraphicsView {  // TODO: move to map_view module
public:
	using tile_type = index_tile_item;

	map_graphics_view(size_t width, size_t height, tiff_tiled_file const & tiff, QGraphicsScene & scene);

protected:
	void mouseMoveEvent(QMouseEvent * event) override;  //!< \note left/right/midddle mouse button must be pressed to receive event
	void mousePressEvent(QMouseEvent * event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;

private:
	void pan_by(QPointF pos);  //!< Translate/pan view by \c pos position.

	QPointF _origin,
		_mouse_click_pos;
	std::vector<tile_type *> _tiles;  //!< list of tiles visible in view
	tile_grid<tile_type *> _grid;  //!< grid structure to make map scrolling possible
};
