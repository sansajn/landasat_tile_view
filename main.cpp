// Graphics View Framework input (and custom item) sample based on *Foundations of Qt Development* book from Chapter 7, Listings 7-22, ..., 7-31
#include <vector>
#include <utility>
#include <filesystem>
#include <array>
#include <memory>
#include <cmath>
#include <cassert>
#include <boost/gil.hpp>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QMainWindow>
#include <spdlog/spdlog.h>
#include "tile_stats.hpp"
#include "tiff_tiled_file.hpp"

using std::array, std::vector, std::pair, std::byte;
using std::move, std::unique_ptr;
using std::filesystem::path;
using std::ceil;
using boost::gil::interleaved_view, boost::gil::gray16_view_t, boost::gil::gray16_pixel_t;

constexpr size_t tile_size = 256,
	scene_w = 5*tile_size,
	scene_h = 3*tile_size;

constexpr char app_title[] = "ltview (Landsat Tile Viewer)";

class tile_item : public QGraphicsItem {
public:
	//! \param image Landsat 8 image.
	tile_item(tiff_tiled_file const & tiff, size_t tile_idx);
	void paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) override;
	QRectF boundingRect() const override;

private:
	QPixmap _pixels;

	bool _checker_replacement = true;
};

constexpr int default_zvalue = 100;


tile_item::tile_item(tiff_tiled_file const & tiff, size_t tile_idx) {
	// get tile data
	unique_ptr<uint16_t> tile_data = tiff.tile(tile_idx);
	size_t const tile_w = tiff.tile_size();  // in pixels

	// create QPixmap
	QImage tile_img(reinterpret_cast<uint8_t *>(tile_data.get()), tile_w, tile_w, sizeof(uint16_t)*tile_w, QImage::Format_Grayscale16);
	_pixels = QPixmap::fromImage(move(tile_img));  // TODO: check whether _pixels are data owner

	if (!_checker_replacement) {  // print stats for tiff tiles
		gray16_view_t pixel_view = interleaved_view(tile_w, tile_w,
			reinterpret_cast<gray16_pixel_t *>(tile_data.get()), sizeof(gray16_pixel_t)*tile_w);
		print_tile_stats(pixel_view);
	}
}

QRectF tile_item::boundingRect() const {
	return QRectF(0, 0, _pixels.width(), _pixels.height());
}

void tile_item::paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) {
	paint->drawPixmap(0, 0, _pixels.width(), _pixels.height(), _pixels);
}


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

	// populate window part scene by tile items
	for (size_t y = 0; y < tile_count_y; ++y) {
		for (size_t x = 0; x < tile_count_x; ++x) {
			tile_item * tile = new tile_item{tiff, 300};  // TODO: handle tile-idx
			QTransform T;
			T.translate(x * tile_size, y * tile_size);
			tile->setTransform(T);  // TODO: can we create translate transformation by kind of translated() function?
			scene.addItem(tile);
		}
	}

	// for testing purpose
	scene.addEllipse(QRectF{-4, -4, 8, 8}, QPen{Qt::blue}, Qt::blue);  // origin (0,0) point mark

	QGraphicsView view;
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
