#include <linux/device.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/io.h>
#include <linux/iio/iio.h>
#include <linux/mutex.h>

struct bbAdc_state {
    void __iomem *base;
    struct mutex lock;
    struct platform_device *pdev;
};

// https://www.ti.com/lit/ug/spruh73q/spruh73q.pdf?ts=1778524544137&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FAM3358
// From page 1831 of the datasheet it is about the TSCADC module.
#define ADC_CTRL          0x40
#define ADC_STEPENABLE    0x54
#define ADC_STEPCONFIG1   0x64
#define ADC_FIFO0DATA     0x100

#define CNTRL_STEPCONFIG_WR_PROT  BIT(2)
#define CNTRL_ENABLE              BIT(0)
#define STEPCONFIG_MODE_SW_ONESHOT (0 << 0)
#define STEPCONFIG_INP_AN0        (0 << 19) // Channel 0
#define STEPCONFIG_FIFO_0         (0 << 26)

static const struct iio_chan_spec bbAdc_channels[] = {
    {
        .type = IIO_VOLTAGE,
        .indexed = 1,
        .channel = 0,
        .info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
        .info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),
    },
};

static int bbAdc_setup(struct bbAdc_state *st){

    iowrite32(CNTRL_STEPCONFIG_WR_PROT, st->base + ADC_CTRL);

    iowrite32(STEPCONFIG_MODE_SW_ONESHOT | STEPCONFIG_INP_AN0 | STEPCONFIG_FIFO_0,

    st->base + ADC_STEPCONFIG1);

    iowrite32(CNTRL_ENABLE | CNTRL_STEPCONFIG_WR_PROT, st->base + ADC_CTRL);

    return 0;
}
 
static int bbAdc_read_raw(struct iio_dev *indio_dev,
               const struct iio_chan_spec *chan,
               int *val, int *val2, long info)
{
    struct bbAdc_state *st = iio_priv(indio_dev);

    switch (info)
    {
    case IIO_CHAN_INFO_RAW:
        mutex_lock(&st->lock);

        iowrite32(BIT(1), st->base + ADC_STEPENABLE);

        //Waiting for hardware to finish conversion
        while (ioread32(st->base + ADC_STEPENABLE) & BIT(1)); 

        *val = ioread32(st->base + ADC_FIFO0DATA) & 0xFFF;

        // Read the raw value from the ADC FIFO register
        mutex_unlock(&st->lock);
        return IIO_VAL_INT;
        break;
    
    case IIO_CHAN_INFO_SCALE:
        
        *val = 1800;   // Millivolts
        *val2 = 4096;  // Total steps for 12-bit
        return IIO_VAL_FRACTIONAL;
        break;
    
    default:
        return -EINVAL;
        break;
    }
}


static const struct iio_info bbAdc_info = {
    .read_raw = bbAdc_read_raw,
};


static int bbAdc_probe(struct platform_device *pdev)
{
    struct iio_dev *indio_dev;
    struct bbAdc_state *st;
    struct resource *res;

    indio_dev = devm_iio_device_alloc(&pdev->dev, sizeof(*st));
    if (!indio_dev)
        return -ENOMEM;

    st = iio_priv(indio_dev);

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

    st->base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(st->base))
        return PTR_ERR(st->base);
    
    st->pdev = pdev;    //storing it just in case we need it later for some reasons
    mutex_init(&st->lock);

    indio_dev->name = "smart_grid_adc";
    indio_dev->info = &bbAdc_info;
    indio_dev->modes = INDIO_DIRECT_MODE;
    indio_dev->channels = bbAdc_channels;
    indio_dev->num_channels = ARRAY_SIZE(bbAdc_channels);

    bbAdc_setup(st);

    return devm_iio_device_register(&pdev->dev, indio_dev);
}

static int bbAdc_remove(struct platform_device *pdev)
{
    return 0;
}  

static const struct of_device_id bbAdc_of_match[] = {
    { .compatible = "ti,am3359-tscadc", }, // Standard BBB ADC string
    { },
};

MODULE_DEVICE_TABLE(of, bbAdc_of_match);

static struct platform_driver bbAdc_driver = {
.driver = {
    .name = "bb-adc",
    .of_match_table = bbAdc_of_match,
    .owner = THIS_MODULE,
},
.probe = bbAdc_probe,
.remove = bbAdc_remove,
};

module_platform_driver(bbAdc_driver);

MODULE_AUTHOR("Amogh Sadhu <amoghsadhu27@gmail.com>");
MODULE_DESCRIPTION("ADC driver for beaglebone black");
MODULE_LICENSE("GPL");