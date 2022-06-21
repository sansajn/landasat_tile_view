#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include "tile_stats.hpp"

using std::cout, std::endl;
using boost::gil::gray16_view_t;


void print_tile_stats(gray16_view_t pixels) {
	using namespace boost::accumulators;

	accumulator_set<double, features<
		tag::min, tag::max, tag::mean, tag::median, tag::variance>> acc;
	acc = for_each(begin(pixels), end(pixels), acc);

	auto const n = count(acc);
	double const var = static_cast<double>(n)/(n-1) * variance(acc),
		sd = sqrt(var);

	cout << "tile statistics:\n"
		<< "  samples=" << n << "\n"
		<< "  min=" << min(acc) << "\n"
		<< "  max=" << max(acc) << "\n"
		<< "  range=" << max(acc) - min(acc) << "\n"
		<< "  mean=" << mean(acc) << "\n"
		<< "  median=" << median(acc) << "\n"
		<< "  sd=" << sd << endl;
}
