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
