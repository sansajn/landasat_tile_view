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
#include "map_view.hpp"

using std::array, std::vector, std::pair, std::byte;
using std::move, std::unique_ptr;
using std::filesystem::path;
using std::ceil;

constexpr size_t tile_size = 256,
	scene_w = 5*tile_size,
	scene_h = 3*tile_size;

constexpr char app_title[] = "ltview (Landsat Tile Viewer)";

constexpr int default_zvalue = 100;


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
