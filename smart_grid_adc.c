#include <linux/device.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/iio/iio.h>
#include <linux/mutex.h>

struct bbAdc_state {
    struct spi_device *spi;
    struct mutex lock;
};

static int bbAdc_setup(struct bbAdc_state *st)
{
    return 0;
}

static int bbAdc_read_raw(struct iio_dev *indio_dev,
               const struct iio_chan_spec *chan,
               int *val, int *val2, long info)
{
    // Logic for reading from SPI will go here
    return 0;
}

static int bbAdc_write_raw(struct iio_dev *indio_dev,
                struct iio_chan_spec const *chan,
                int val, int val2, long info)
{
    return 0;
}

/* --- 2. IIO Info (references the functions above) --- */

static const struct iio_info bbAdc_info = {
    .read_raw = bbAdc_read_raw,
    .write_raw = bbAdc_write_raw,
};

/* --- 3. Probe and Remove --- */

static int bbAdc_probe(struct spi_device *spi)
{
    struct iio_dev *indio_dev;
    struct bbAdc_state *st;

    indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*st));
    if (!indio_dev)
        return -ENOMEM;

    st = iio_priv(indio_dev);
    st->spi = spi;
    mutex_init(&st->lock);

    indio_dev->name = "smart-grid-adc";
    indio_dev->info = &bbAdc_info;
    indio_dev->modes = INDIO_DIRECT_MODE;

    // Optional: Call your setup function here
    bbAdc_setup(st);

    return devm_iio_device_register(&spi->dev, indio_dev);
}

static int bbAdc_remove(struct spi_device *spi)
{
    // devm handles the cleanup, but you can add custom logic here
    return 0;
}  

/* --- 4. Boilerplate and Registration --- */

static const struct spi_device_id bbAdc_id[] = {
    { "smart-grid-adc", 0 },
    { }
};
MODULE_DEVICE_TABLE(spi, bbAdc_id);

static struct spi_driver bbAdc_driver = {
    .driver = {
        .name = "smart-grid-adc",
        .owner = THIS_MODULE,
    },
    .probe = bbAdc_probe,
    .remove = bbAdc_remove,
    .id_table = bbAdc_id,
};

module_spi_driver(bbAdc_driver);

MODULE_AUTHOR("Amogh Sadhu <amoghsadhu27@gmail.com>");
MODULE_DESCRIPTION("ADC driver for beaglebone black");
MODULE_LICENSE("GPL");