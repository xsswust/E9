#ifndef AD
#define AD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "common.h"
//#include <ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EAD_IOC_MAGIC            'k'
#define IOCTL_EAD_CHANNEL       _IO(EAD_IOC_MAGIC, 1 )
#define IOCTL_EAD_SET_CONTROL   _IO(EAD_IOC_MAGIC, 2 )
#define IOCTL_EAD_GET_CONTROL   _IO(EAD_IOC_MAGIC, 3 )
#define IOCTL_EAD_GET_AD        _IO(EAD_IOC_MAGIC, 4 )
#define IOCTL_EAD_DRDY          _IO(EAD_IOC_MAGIC, 5 )

enum AD_OperatigMode {
    NORMAL_MODE,
    ACTIVATE_SELF_CALIBRATION,
    ACTIVATE_SYSTEM_CALIBRATION_1,
    ACTIVATE_SYSTEM_CALIBRATION_2,
    ACTIVATE_ACTIVATE_SYSTEM_OFFSET_CALIBRATION,
    ACTIVATE_BACKGROUND_CALIBRATION,
    READ_WRITE_ZERO_SCALE,
    READ_WRITE_FULL_SCALE
};


enum AD_Channel {
    AD_Channel_AIN1,
    AD_Channel_AIN2
};

enum AD_BurnoutCurrent {
    AD_BurnoutCurrent_off,
    AD_BurnoutCurrent_on
};

enum AD_WordLength {
    AD_WordLength_16Bit,
    AD_WordLength_24Bit
};

enum AD_Gain {
    AD_Gain_1,
    AD_Gain_2,
    AD_Gain_4,
    AD_Gain_8,
    AD_Gain_16,
    AD_Gain_32,
    AD_Gain_64,
    AD_Gain_128,
};


enum AD_Bipolar {
    AD_Bipolar,     //双极
    AD_Unipolar     //单极
};

/** use reference of +2.5 V  MAX 4*VRef
 */
/** set adc gain
 *
 * @param[in] gain the adc internal PGA gain
 * The input voltage range is ±4 VREF/GAIN
 * @return 0 if successfully
 */
int ead_set_control_gain(enum AD_Gain gain);

/** get adc gain
 *
 *  get the adc internal PGA gain.
 *
 * @return adc gain
 */
int ead_get_control_gain(void);

/** set adc bipolar
 *
 * @param[in] bipolar the adc bipolar
 * The input voltage range is 0 mV to 10v unipolar signals and  ±10 V with bipolar
 * when gain is 1
 * Programming the part for either unipolar or bipolar
 * operation does not change any of the input signal conditioning;
 * it simply changes the data output coding
 *
 * @return 0 if successfully
 */
int ead_set_control_bipolar( enum AD_Bipolar bipolar);
int ead_get_control_bit_bipolar(void);

int ead_set_control_channel( enum AD_Channel channel);

void ead_activateselfcalibration();

int  ead_get_control(void);
int  ead_set_control(void);

int ead_set_control_bit_length(enum AD_WordLength len);
int ead_get_control_bit_length(void);

int ead_set_control_mode(enum AD_OperatigMode mode);
int ead_get_control_mode(void);

int ead_set_control_filter( WORD filter);
int ead_get_control_filter(void);


typedef struct ControlReg {
    uint8_t operation_mode;
    uint8_t pga_gain;
    uint8_t channel_selection;
    BOOL power_down;
    BOOL word_length;
    BOOL burnout_current;
    BOOL bipolar;
    uint16_t filter_selection;
} ControlReg;


#if 1

/** adc
 *
 * use adc, first initial use ead_init. when done, use ead_deinit for clean.
 *
 * set sample mode use  ead_set_sample_mode, the default sample bit is 16bit.
 * set sample voltage range use ead_set_sample_range
 * set sample channel use ead_set_channel
 * get adc voltage use ead_read_adc
 * wait adc done use WaitForSingleObject( ead_get_event_handle(), TIMEOUT);
 */

/* sample mode */
enum AD_SAMPLE_MODE {
    AD_SAMPLE_MODE_FAST,
    AD_SAMPLE_MODE_NORMAL
};

enum AD_SAMPLE_RANGE{
    AD_SAMPLE_RANGE_Bipolar_150mV,   // -150 ~ 150 mV
    AD_SAMPLE_RANGE_Bipolar_500mV,
    AD_SAMPLE_RANGE_Bipolar_1V,
    AD_SAMPLE_RANGE_Bipolar_2V5,
    AD_SAMPLE_RANGE_Bipolar_5V,
    AD_SAMPLE_RANGE_Bipolar_10V,

    AD_SAMPLE_RANGE_Unipolar_150mV, // 0 ~ 150 mV
    AD_SAMPLE_RANGE_Unipolar_500mV,
    AD_SAMPLE_RANGE_Unipolar_1V,
    AD_SAMPLE_RANGE_Unipolar_2V5,
    AD_SAMPLE_RANGE_Unipolar_5V,
    AD_SAMPLE_RANGE_Unipolar_10V,
};
#define  AD_SAMPLE_RANGE_Bipolar_20mA  AD_SAMPLE_RANGE_Bipolar_5V
#define  AD_SAMPLE_RANGE_Unipolar_20mA AD_SAMPLE_RANGE_Unipolar_5V

/** initial adc

 */
int ead_init(void);

/** deinitial adc
 */
int ead_deinit(void);

/** get ad voltage value
 *
 * @param[out] voltage  point to wait single result, it can be NULL if not care
 * @return  < 0 error
 */
#if !TEST_NEW
int ead_read_adc(double* volt);
#endif
/** get ad voltage raw
 *
 * @return  < 0 error
 */
#if !TEST_NEW
int ead_read_raw(unsigned long *value);
#endif
/* convert raw value to voltage accord current configure */
double ead_convert_raw_voltage(unsigned long value);

/** set adc channel
 *
 * it has 8 channel, from 0~7. adc can only convert one channel simultaneously
 * @param[in] channel Channel of the adc
 *
 * @return 0 if successfully
 */
int ead_set_channel(uint8_t channel);

/** set adc sample mode
 *
 * it has two mode of adc sample
 * FAST for sample 16bit
 * NORMAL for sample 24bit
 *
 * @param[in] mode sample mode
 *
 * @return 0 if successfully
 */
int ead_set_sample_mode(enum AD_SAMPLE_MODE mode);

/**
 * @return sample mode
 */
int ead_get_sample_mode(void);


/** set sample range
 *
 * @param[in] range sample voltage range
 *
 * @return 0 if successfully
 */
int ead_set_sample_range(enum AD_SAMPLE_RANGE range);
#endif
#ifdef __cplusplus
}
#endif

#endif // AD

