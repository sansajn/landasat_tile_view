// Graphics View Framework input (and custom item) sample based on *Foundations of Qt Development* book from Chapter 7, Listings 7-22, ..., 7-31
#include <vector>
#include <utility>
#include <filesystem>
#include <array>
#include <memory>
#include <cmath>
#include <cassert>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QMainWindow>
#include <QMouseEvent>
#include <spdlog/spdlog.h>
#include "tile_item.hpp"
#include "tile_grid.hpp"
#include "tiff_tiled_file.hpp"

using std::array, std::vector, std::pair, std::byte;
using std::move, std::unique_ptr;
using std::filesystem::path;
using std::ceil;

constexpr size_t tile_size = 256,
	scene_w = 5*tile_size,
	scene_h = 3*tile_size;

constexpr char app_title[] = "ltview (Landsat Tile Viewer)";


constexpr int default_zvalue = 100;


class map_graphics_view : public QGraphicsView {
public:
	map_graphics_view(size_t width, size_t height, tiff_tiled_file const & tiff, QGraphicsScene & scene) {
		// populate view by tile items
		size_t const tile_count_x = ceil((float)width / tile_size),
			tile_count_y = ceil((float)height / tile_size);

		_tiles.reserve(tile_count_y * tile_count_y);

		for (size_t y = 0; y < tile_count_y; ++y) {
			for (size_t x = 0; x < tile_count_x; ++x) {
				tile_item * tile = new tile_item{tiff, 300};  // TODO: handle tile-idx
				_tiles.push_back(tile);
				QTransform T;
				T.translate(x * tile_size, y * tile_size);
				tile->setTransform(T);  // TODO: can we create translate transformation by kind of translated() function?
				scene.addItem(tile);
			}
		}
	}

protected:
	void mouseMoveEvent(QMouseEvent * event) override {  //!< \note left/right/midddle mouse button must be pressed to receive event
		QPoint pos = event->pos();  // pozicia vo widgete
		QPointF screen_pos = event->screenPos();
		spdlog::info("mouse moved: view-pos=({},{}), screen-pos=({},{})", pos.x(), pos.y(), screen_pos.x(), screen_pos.y());

		QGraphicsView::mouseMoveEvent(event);
	}

	void mousePressEvent(QMouseEvent * event) override {
		QPointF local_pos = event->localPos();
		spdlog::info("mouse button clicked at: local-pos=({},{})", local_pos.x(), local_pos.y());

		_mouse_click_pos = event->localPos();

		QGraphicsView::mousePressEvent(event);
	}

	void mouseReleaseEvent(QMouseEvent * event) override {
		if (_mouse_click_pos != QPointF{}) {
			setTransformationAnchor(QGraphicsView::NoAnchor);
			QPointF dp = event->pos() - _mouse_click_pos;
			spdlog::info("view moved by: ({},{})", dp.x(), dp.y());
			// TODO: we want there to change tile positions for testing purpose
		}

		QGraphicsView::mouseReleaseEvent(event);
	}

private:
	void pan_by(QPointF pos);  //!< translate/pan view by \c pos position

	QPointF _mouse_click_pos;
	vector<tile_item *> _tiles;  //!< list of tiles visible in view
};


int main(int argc, char * argv[]) {
	QApplication app{argc, argv};
	QGraphicsScene scene{QRectF{0, 0, scene_w, scene_h}};

	// tile data
	tiff_tiled_file tiff{"/home/ja/gis/data/landsat8/LC08_L2SP_191025_20210615_20210622_02_T1/LC08_L2SP_191025_20210615_20210622_02_T1_SR_B1.TIF"};

	// fill scene with tile items
	size_t const tile_count_x = ceil((float)scene_w / tile_size),
		tile_count_y = ceil((float)scene_h / tile_size);

	assert(tile_count_x == 5 && tile_count_y == 3);

	spdlog::info("tile-count-x={0}, tile-count-y={1}", tile_count_x, tile_count_y);

	// for testing purpose
	scene.addEllipse(QRectF{-4, -4, 8, 8}, QPen{Qt::blue}, Qt::blue);  // origin (0,0) point mark

	map_graphics_view view{scene_w, scene_h, tiff, scene};
	view.setBackgroundBrush(Qt::white);
	view.setScene(&scene);

	// disable scrollbars
	view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QMainWindow w;
	w.resize(scene_w, scene_h);
	w.setCentralWidget(&view);
	w.show();

	spdlog::info("w={0}, h={1}", w.width(), w.height());  // print window geometry

	return app.exec();
}
