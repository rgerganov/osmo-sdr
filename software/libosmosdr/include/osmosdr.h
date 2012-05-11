/*
 * Copyright (C) 2012 by Dimitri Stolnikov <horiz0n@gmx.net>
 * Copyright (C) 2012 by Steve Markgraf <steve@steve-m.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __OSMOSDR_H
#define __OSMOSDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <osmosdr_export.h>

typedef struct osmosdr_dev osmosdr_dev_t;

OSMOSDR_API uint32_t osmosdr_get_device_count(void);

OSMOSDR_API const char* osmosdr_get_device_name(uint32_t index);

OSMOSDR_API int osmosdr_open(osmosdr_dev_t **dev, uint32_t index);

OSMOSDR_API int osmosdr_close(osmosdr_dev_t *dev);

/* configuration functions */

/*!
 * Set clock frequencies used for the ADC and the tuner ICs.
 *
 * NOTE: Call this function only if you know what you are doing.
 *
 * \param dev the device handle given by osmosdr_open()
 * \param adc_clock frequency value used to clock the ADC in Hz
 * \param tun_clock frequency value used to clock the tuner IC in Hz
 * \return 0 on success
 */
OSMOSDR_API int osmosdr_set_clock_freq(osmosdr_dev_t *dev, uint32_t adc_clock,
					uint32_t tun_clock);

/*!
 * Get clock frequencies used for the ADC and the tuner IC.
 *
 * \param dev the device handle given by osmosdr_open()
 * \param adc_clock frequency value used to clock the ADC in Hz
 * \param tun_clock frequency value used to clock the tuner IC in Hz
 * \return 0 on success
 */
OSMOSDR_API int osmosdr_get_clock_freq(osmosdr_dev_t *dev, uint32_t *adc_clock,
					uint32_t *tun_clock);

/*!
 * Set the frequency the device is tuned to.
 *
 * \param dev the device handle given by osmosdr_open()
 * \param freq frequency in Hz the device should be tuned to
 * \return 0 on error, frequency in Hz otherwise
 */
OSMOSDR_API int osmosdr_set_center_freq(osmosdr_dev_t *dev, uint32_t freq);

/*!
 * Get the actual frequency the device is tuned to.
 *
 * \param dev the device handle given by osmosdr_open()
 * \return 0 on error, frequency in Hz otherwise
 */
OSMOSDR_API uint32_t osmosdr_get_center_freq(osmosdr_dev_t *dev);

/*!
 * Set the gain for the device.
 * Manual gain mode must be enabled for this to work.
 *
 * Valid gain values (in tenths of a dB) for the E4000 tuner:
 * -10, 15, 40, 65, 90, 115, 140, 165, 190,
 * 215, 240, 290, 340, 420, 430, 450, 470, 490
 *
 * \param dev the device handle given by osmosdr_open()
 * \param gain in tenths of a dB, 115 means 11.5 dB.
 * \return 0 on success
 */
OSMOSDR_API int osmosdr_set_tuner_gain(osmosdr_dev_t *dev, int gain);

/*!
 * Get actual gain the device is configured to.
 *
 * \param dev the device handle given by osmosdr_open()
 * \return 0 on error, gain in tenths of a dB, 115 means 11.5 dB.
 */
OSMOSDR_API int osmosdr_get_tuner_gain(osmosdr_dev_t *dev);

/*!
 * Set the gain mode (automatic/manual) for the device.
 * Manual gain mode must be enabled for the gain setter function to work.
 *
 * \param dev the device handle given by osmosdr_open()
 * \param manual gain mode, 1 means manual gain mode shall be enabled.
 * \return 0 on success
 */
OSMOSDR_API int osmosdr_set_tuner_gain_mode(osmosdr_dev_t *dev, int manual);

/* this will select the baseband filters according to the requested sample rate */
OSMOSDR_API int osmosdr_set_sample_rate(osmosdr_dev_t *dev, uint32_t rate);

/*!
 * Get actual sample rate the device is configured to.
 *
 * \param dev the device handle given by osmosdr_open()
 * \return 0 on error, sample rate in Hz otherwise
 */
OSMOSDR_API uint32_t osmosdr_get_sample_rate(osmosdr_dev_t *dev);

/* streaming functions */

OSMOSDR_API int osmosdr_read_sync(osmosdr_dev_t *dev, void *buf, int len, int *n_read);

typedef void(*osmosdr_read_async_cb_t)(unsigned char *buf, uint32_t len, void *ctx);

/*!
 * Read samples from the device asynchronously. This function will block until
 * it is being canceled using osmosdr_cancel_async()
 *
 * \param dev the device handle given by osmosdr_open()
 * \param cb callback function to return received samples
 * \param ctx user specific context to pass via the callback function
 * \param buf_num optional buffer count, buf_num * buf_len = overall buffer size
 *		  set to 0 for default buffer count (32)
 * \param buf_len optional buffer length, must be multiple of 512,
 *		  set to 0 for default buffer length (16 * 32 * 512)
 * \return 0 on success
 */
OSMOSDR_API int osmosdr_read_async(osmosdr_dev_t *dev,
				 osmosdr_read_async_cb_t cb,
				 void *ctx,
				 uint32_t buf_num,
				 uint32_t buf_len);

/*!
 * Cancel all pending asynchronous operations on the device.
 *
 * \param dev the device handle given by osmosdr_open()
 * \return 0 on success
 */
OSMOSDR_API int osmosdr_cancel_async(osmosdr_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* __OSMOSDR_H */