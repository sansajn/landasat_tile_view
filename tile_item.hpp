#pragma once
#include <QGraphicsItem>
#include "tiff_tiled_file.hpp"

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

//! Dummy tile item with index in the middle of tile.
class index_tile_item : public QGraphicsItem {
public:
	//! \param image Landsat 8 image.
	index_tile_item(size_t tile_idx);
	void paint(QPainter * paint, QStyleOptionGraphicsItem const * option, QWidget * widget) override;
	QRectF boundingRect() const override;

private:
	void draw_tile(size_t tile_idx, QImage & tile);

	QPixmap _pixels;
	size_t _tile_size = 256;  //!< tile width/height
};
