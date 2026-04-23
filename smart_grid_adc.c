#include <linux/device.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

#include <linux/iio/iio.h>

struct bbAdc_state{

};

static int bbAdc_setup(struct ad7292_state *st)
{
	return 0;
}
 
static int bbAdc_read_raw(struct iio_dev *indio_dev,
			   const struct iio_chan_spec *chan,
			   int *val, int *val2, long info)
{
	return 0;
}

static int bbAdc_write_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int val, int val2, long info)
{
	return 0;
}

static int bbAdc_probe(struct spi_device *spi){
	
}

static int bbAdc_remove( struct spi_device *spi){

}

static const struct iio_sw_device_ops iio_dummy_device_ops = {
	.probe = bbAdc_probe,
	.remove = bbAdc_remove,
};

static struct iio_sw_device_type iio_dummy_device = {
	.name = "smart-grid-adc",
	.owner = THIS_MODULE,
	.ops = &iio_dummy_device_ops,
};

module_iio_sw_device_driver(iio_dummy_device);

MODULE_AUTHOR("Amogh Sadhu <amoghsadhu27@gmail.com>");
MODULE_DESCRIPTION("ADC driver for beaglebone black");
MODULE_LICENSE("GPL v3");