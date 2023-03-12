// Part of Ironpedal
// https://github.com/snail23/ironpedal

#define PIN_FOOT_SWITCH_1 daisy::seed::D25
#define PIN_FOOT_SWITCH_2 daisy::seed::D26

#define PIN_KNOB_1 daisy::seed::A1
#define PIN_KNOB_2 daisy::seed::A2
#define PIN_KNOB_3 daisy::seed::A3
#define PIN_KNOB_4 daisy::seed::A4
#define PIN_KNOB_5 daisy::seed::A5
#define PIN_KNOB_6 daisy::seed::A6

#define PIN_LED_1 daisy::seed::A7
#define PIN_LED_2 daisy::seed::A8

#define PIN_SPI_CS daisy::seed::D3
#define PIN_SPI_MISO daisy::seed::D1
#define PIN_SPI_MOSI daisy::seed::D6
#define PIN_SPI_RST daisy::seed::D30
#define PIN_SPI_SCLK daisy::seed::D2

#define PIN_SWITCH_1 daisy::seed::D10
#define PIN_SWITCH_2 daisy::seed::D9
#define PIN_SWITCH_3 daisy::seed::D8
#define PIN_SWITCH_4 daisy::seed::D7

void OnAudio(daisy::AudioHandle::InputBuffer in, daisy::AudioHandle::OutputBuffer out, size_t size);

namespace PedalPCB
{
    enum KnobId
    {
        KNOB_1,
        KNOB_2,
        KNOB_3,
        KNOB_4,
        KNOB_5,
        KNOB_6,
        KNOB_LAST
    };

    enum LedId
    {
        LED_1,
        LED_2,
        LED_LAST
    };

    enum SwitchId
    {
        FOOT_SWITCH_1,
        FOOT_SWITCH_2,

        SWITCH_1,
        SWITCH_2,
        SWITCH_3,
        SWITCH_4,
        SWITCH_LAST
    };

    class Led : public dsy_gpio
    {
    public:
        void Init(daisy::Pin pin)
        {
            this->mode = DSY_GPIO_MODE_OUTPUT_PP;
            this->pin = pin;
            this->pull = DSY_GPIO_NOPULL;

            dsy_gpio_init(this);
        }

        void Set(bool state)
        {
            dsy_gpio_write(this, state);
        }
    };

    class Terrarium : public daisy::DaisySeed
    {
    public:
        daisy::AnalogControl knobs[KNOB_LAST];

        daisy::GPIO spi_cs;
        daisy::GPIO spi_dc;
        daisy::GPIO spi_rst;

        daisy::SpiHandle spi;
        daisy::Switch switches[SWITCH_LAST];

        Led leds[LED_LAST];

        Terrarium()
        {
            this->Init(true);
            this->SetLed(true);

            daisy::AdcChannelConfig adc_config[KNOB_LAST];

            adc_config[KNOB_1].InitSingle(PIN_KNOB_1);
            adc_config[KNOB_2].InitSingle(PIN_KNOB_2);
            adc_config[KNOB_3].InitSingle(PIN_KNOB_3);
            adc_config[KNOB_4].InitSingle(PIN_KNOB_4);
            adc_config[KNOB_5].InitSingle(PIN_KNOB_5);
            adc_config[KNOB_6].InitSingle(PIN_KNOB_6);

            this->adc.Init(adc_config, KNOB_LAST);
            this->adc.Start();

            for (auto i = 0; i < KNOB_LAST; ++i)
                this->knobs[i].Init(this->adc.GetPtr(i), 1000.0f);

            this->leds[LED_1].Init(PIN_LED_1);
            this->leds[LED_2].Init(PIN_LED_2);

            this->switches[FOOT_SWITCH_1].Init(PIN_FOOT_SWITCH_1, 1000.0f, daisy::Switch::TYPE_MOMENTARY, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
            this->switches[FOOT_SWITCH_2].Init(PIN_FOOT_SWITCH_2, 1000.0f, daisy::Switch::TYPE_MOMENTARY, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);

            this->switches[SWITCH_1].Init(PIN_SWITCH_1, 1000.0f, daisy::Switch::TYPE_TOGGLE, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
            this->switches[SWITCH_2].Init(PIN_SWITCH_2, 1000.0f, daisy::Switch::TYPE_TOGGLE, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
            this->switches[SWITCH_3].Init(PIN_SWITCH_3, 1000.0f, daisy::Switch::TYPE_TOGGLE, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);
            this->switches[SWITCH_4].Init(PIN_SWITCH_4, 1000.0f, daisy::Switch::TYPE_TOGGLE, daisy::Switch::POLARITY_INVERTED, daisy::Switch::PULL_UP);

            daisy::SpiHandle::Config spi_config;

            spi_config.baud_prescaler = daisy::SpiHandle::Config::BaudPrescaler::PS_2;
            spi_config.direction = daisy::SpiHandle::Config::Direction::TWO_LINES;
            spi_config.mode = daisy::SpiHandle::Config::Mode::MASTER;
            spi_config.nss = daisy::SpiHandle::Config::NSS::SOFT;
            spi_config.periph = daisy::SpiHandle::Config::Peripheral::SPI_3;

            spi_config.pin_config.sclk = PIN_SPI_SCLK;
            spi_config.pin_config.miso = PIN_SPI_MISO;
            spi_config.pin_config.mosi = PIN_SPI_MOSI;

            this->spi.Init(spi_config);
            this->spi_cs.Init(PIN_SPI_CS, daisy::GPIO::Mode::OUTPUT, daisy::GPIO::Pull::NOPULL, daisy::GPIO::Speed::VERY_HIGH);
            this->spi_dc.Init(PIN_SPI_MISO, daisy::GPIO::Mode::OUTPUT, daisy::GPIO::Pull::NOPULL, daisy::GPIO::Speed::VERY_HIGH);
            this->spi_rst.Init(PIN_SPI_RST, daisy::GPIO::Mode::OUTPUT, daisy::GPIO::Pull::NOPULL, daisy::GPIO::Speed::VERY_HIGH);

            this->SetAudioBlockSize(64);
            this->SetLed(false);
        }

        void Start()
        {
            this->StartAudio(OnAudio);
        }
    };
}
