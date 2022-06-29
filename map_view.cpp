#include <spdlog/spdlog.h>
#include "map_view.hpp"

map_graphics_view::map_graphics_view(size_t width, size_t height, tiff_tiled_file const & tiff, QGraphicsScene & scene)
	: _origin{.0f, .0f}
	, _mouse_click_pos{.0f, .0f} {

	// populate view by tile items
	size_t const tile_size = tiff.tile_size(),
		tile_count_x = ceil((float)width / tile_size),  // TODO: use more tiles to suppor smooth scrolling
		tile_count_y = ceil((float)height / tile_size);

	_tiles.reserve(tile_count_y * tile_count_y);
	for (size_t y = 0; y < tile_count_y; ++y) {
		for (size_t x = 0; x < tile_count_x; ++x) {
			index_tile_item * tile = new index_tile_item{300 + y*tile_count_x + x};  // TODO: handle tile-idx
			_tiles.push_back(tile);
			QTransform T;
			T.translate(x * tile_size, y * tile_size);
			tile->setTransform(T);  // TODO: can we create translate transformation by kind of translated() function?
			scene.addItem(tile);
		}
	}

	// create grid to supoprt scrolling
	_grid = tile_grid{tile_count_y, tile_count_x, &_tiles[0]};
}

void map_graphics_view::mouseMoveEvent(QMouseEvent * event) {
	QPoint pos = event->pos();  // widget position
	QPointF screen_pos = event->screenPos();
	QPointF position = event->position();
	spdlog::info("mouse moved: view-pos=({},{}), screen-pos=({},{}), position=({},{})", pos.x(), pos.y(), screen_pos.x(), screen_pos.y(), position.x(), position.y());

	QPointF const dp = event->position() - _mouse_click_pos;
	if (dp != QPointF{}) {
		pan_by(dp);
		_mouse_click_pos = event->position();
	}

	QGraphicsView::mouseMoveEvent(event);
}

void map_graphics_view::mousePressEvent(QMouseEvent * event) {
	QPointF local_pos = event->localPos();
	spdlog::info("mouse button clicked at: local-pos=({},{})", local_pos.x(), local_pos.y());

	_mouse_click_pos = event->localPos();

	QGraphicsView::mousePressEvent(event);
}

void map_graphics_view::mouseReleaseEvent(QMouseEvent * event) {
	QGraphicsView::mouseReleaseEvent(event);
}

void map_graphics_view::pan_by(QPointF pos) {
	_origin += pos;
	for (index_tile_item * tile : _tiles) {
		QTransform T;
		T.translate(pos.x(), pos.y());
		tile->setTransform(T, true);  // NOTE: we assume there is not any rotation there
	}

	spdlog::info("origin=({},{})", _origin.x(), _origin.y());

	// TODO: reshape tiles ...

	// TODO: notify tiles not visible ...
}
