#include <string>
#include <boost/gil.hpp>
#include <QPainter>
#include "tile_stats.hpp"
#include "tile_item.hpp"
using std::move, std::unique_ptr, std::string, std::to_string;
using boost::gil::interleaved_view, boost::gil::gray16_view_t, boost::gil::gray16_pixel_t;

tile_item::tile_item(tiff_tiled_file const & tiff, size_t tile_idx) {
	// get tile data
	unique_ptr<uint16_t> tile_data = tiff.tile(tile_idx);
	size_t const tile_w = tiff.tile_size();  // in pixels

	// create QPixmap
	QImage tile_img(reinterpret_cast<uint8_t *>(tile_data.get()), tile_w, tile_w, sizeof(uint16_t)*tile_w, QImage::Format_Grayscale16);

	// TODO: add tile index (this canbe only done for RGB images :(
	{
		QPainter painter{&tile_img};
		QPen pen;
		pen.setWidth(20);
		pen.setColor(Qt::red);
		painter.setPen(pen);
		painter.drawText(0, 0, "hello");
		painter.end();
	}

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

checker_tile_item::checker_tile_item(size_t tile_idx) {
	// get tile data
	size_t const tile_w = 256;  // in pixels

	// create QPixmap
	QImage tile_img(tile_w, tile_w, QImage::Format_ARGB32_Premultiplied);

	// TODO: populate with checker pattern

	draw_tile_idx(tile_idx, tile_img);

	_pixels = QPixmap::fromImage(move(tile_img));  // TODO: check whether _pixels are data owner
}

QRectF checker_tile_item::boundingRect() const {
	return QRectF(0, 0, _pixels.width(), _pixels.height());
}

void checker_tile_item::paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) {
	paint->drawPixmap(0, 0, _pixels.width(), _pixels.height(), _pixels);
}

void checker_tile_item::draw_tile_idx(size_t tile_idx, QImage & tile) {
	QPainter painter{&tile};
	QPen pen;
	pen.setWidth(20);
	pen.setColor(Qt::red);
	painter.setPen(pen);
	string text = to_string(tile_idx);
	painter.drawText(tile.width()/2, tile.height()/2, text.c_str());
	painter.end();
}
