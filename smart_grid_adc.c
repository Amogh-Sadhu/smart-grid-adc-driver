#include <linux/device.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

#include <linux/iio/iio.h>

MODULE_LICENSE("GPL");

struct ad7292_state {
	struct spi_device *spi;
};

static int ad7292_setup(struct ad7292_state *st)
{
	return 0;
}
 
static int ad7292_read_raw(struct iio_dev *indio_dev,
			   const struct iio_chan_spec *chan,
			   int *val, int *val2, long info)
{
	return 0;
}

static int ad7292_write_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int val, int val2, long info)
{
	return 0;
}