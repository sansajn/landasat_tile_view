#include <spdlog/spdlog.h>
#include "map_view.hpp"

constexpr size_t tile_size = 256;  // FIXME: use tiff.size() instead

map_graphics_view::map_graphics_view(size_t width, size_t height, tiff_tiled_file const & tiff, QGraphicsScene & scene) {
	// populate view by tile items
	size_t const tile_count_x = ceil((float)width / tile_size),  // TODO: use more tiles to suppor smooth scrolling
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
}

void map_graphics_view::mouseMoveEvent(QMouseEvent * event) {
	QPoint pos = event->pos();  // pozicia vo widgete
	QPointF screen_pos = event->screenPos();
	QPointF position = event->position();
	spdlog::info("mouse moved: view-pos=({},{}), screen-pos=({},{}), position=({},{})", pos.x(), pos.y(), screen_pos.x(), screen_pos.y(), position.x(), position.y());

	QPointF dp = event->position() - _mouse_click_pos;
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
	for (index_tile_item * tile : _tiles) {
		QTransform T;
		T.translate(pos.x(), pos.y());
		tile->setTransform(T, true);  // NOTE: we assume there is not any rotation there
	}

	// TODO: reshape tiles ...
}
