// Graphics View Framework input (and custom item) sample based on *Foundations of Qt Development* book from Chapter 7, Listings 7-22, ..., 7-31
#include <vector>
#include <utility>
#include <filesystem>
#include <array>
#include <memory>
#include <boost/gil.hpp>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include "tile_stats.hpp"
#include "tiff_tiled_file.hpp"

using std::array, std::vector, std::pair, std::byte;
using std::move, std::unique_ptr;
using std::filesystem::path;
using boost::gil::interleaved_view, boost::gil::gray16_view_t, boost::gil::gray16_pixel_t;

class tile_item : public QGraphicsItem {
public:
	//! \param image Landsat 8 image.
	tile_item(tiff_tiled_file const & tiff, size_t tile_idx);
	void paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) override;
	QRectF boundingRect() const override;

private:
	QPixmap _pixels;
};

constexpr int default_zvalue = 100;


tile_item::tile_item(tiff_tiled_file const & tiff, size_t tile_idx) {
	// get tile data
	unique_ptr<uint16_t> tile_data = tiff.tile(tile_idx);
	size_t const tile_w = tiff.tile_size();  // in pixels

	// create QPixmap
	QImage tile_img(reinterpret_cast<uint8_t *>(tile_data.get()), tile_w, tile_w, sizeof(uint16_t)*tile_w, QImage::Format_Grayscale16);
	_pixels = QPixmap::fromImage(move(tile_img));  // TODO: check whether _pixels are data owner

	// print tile stats ...
	gray16_view_t pixel_view = interleaved_view(tile_w, tile_w,
		reinterpret_cast<gray16_pixel_t *>(tile_data.get()), sizeof(gray16_pixel_t)*tile_w);
	print_tile_stats(pixel_view);
}

QRectF tile_item::boundingRect() const {
	return QRectF(0, 0, _pixels.width(), _pixels.height());
}

void tile_item::paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) {
	paint->drawPixmap(0, 0, _pixels.width(), _pixels.height(), _pixels);
}


int main(int argc, char * argv[]) {
	QApplication app{argc, argv};
	QGraphicsScene scene{QRectF{0, 0, 256, 256}};

	QGraphicsRectItem * rectItem = scene.addRect(QRectF{10, 10, 50, 100});
	QGraphicsEllipseItem * elItem = scene.addEllipse(QRectF{80, 40, 100, 80});

	tiff_tiled_file tiff{"/home/ja/gis/data/landsat8/LC08_L2SP_191025_20210615_20210622_02_T1/LC08_L2SP_191025_20210615_20210622_02_T1_SR_B1.TIF"};
	tile_item * tile = new tile_item{tiff, 300};
	scene.addItem(tile);

	QGraphicsView view;
	view.setBackgroundBrush(Qt::white);
	view.setScene(&scene);
	view.show();

	return app.exec();
}
