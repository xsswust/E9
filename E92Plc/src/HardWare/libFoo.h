#ifndef __LIBFOO_H__
#define __LIBFOO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** use gpio, first gpio_init, and set state use gpio_set
 * get gpio state use gpio_get.
 * 
 * exampe use GPIO
 *
 * gpio_init();
 * gpio_set(0, 0);
 * gpio_set(0, 1);
 * gpio_get(3);
 * gpio_deinit();
 *
 * watch gpio input  
 *
 * char input[40]
 * gpio_init();
 * gpio_read(input, sizeof(input), NULL);
 * gpio_deinit();
 *
 * the gpio index from 0.
 * when finished, use gpio_deinit
 */

/** initial gpio
 * @return less 0 if failed
 */
int gpio_init(void);

/** deinitial gpio
 */
int gpio_deinit(void);
/** set gpio state
 * @param[in] num  gpio index from 0..MAX
 * @param[in] value 1 set gpio high, and 0 set gpio low
 *
 * @return 0 if successfully
 */
int gpio_set(int num, int value);

/** set gpio state
 * @param[in] num  gpio index from 0..MAX
 *
 * @return 1 if gpio high, 0 if gpio low
 */
int gpio_get(int num);

/** read gpio value
 * gpio_read will block till input change or timeout.
 *
 * param[in] buf point to io value
 * @param[in] milliseconds milli seconds to wait, 0 wait forever
 * @return  length of read. less 0 indication error
 */
int gpio_read(char *buf, int len, int milliseconds);

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

/** get ad voltage value
 *
 * @param[out] voltage  point to wait single result, it can be NULL if not care
 * @return  < 0 error
 */

int ead_read_adc(int channel, double* volt);

int ead_read_raw(int channel, double* value);

#ifdef __cplusplus
}
#endif

#endif // __LIBFOO_H__

