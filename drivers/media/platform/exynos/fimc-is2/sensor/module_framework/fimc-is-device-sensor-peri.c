/*
 * Samsung Exynos5 SoC series FIMC-IS driver
 *
 * exynos5 fimc-is video functions
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/videodev2.h>
#include <linux/videodev2_exynos_camera.h>
#include <linux/kthread.h>

#include "interface/fimc-is-interface-library.h"
#include "interface/fimc-is-interface-ddk.h"
#include "fimc-is-device-sensor-peri.h"
#include "fimc-is-device-sensor.h"
#include "fimc-is-video.h"

#if defined(CONFIG_LEDS_S2MU106_FLASH)
#include <linux/leds-s2mu106.h>
#include <linux/mfd/samsung/s2mu106.h>
/* MUIC header file */
#include <linux/muic/muic.h>
#include <linux/muic/s2mu106-muic.h>
#include <linux/muic/s2mu106-muic-hv.h>
#include <linux/ccic/usbpd_ext.h>
#endif

extern struct device *fimc_is_dev;
#ifdef FIXED_SENSOR_DEBUG
extern struct fimc_is_sysfs_sensor sysfs_sensor;
#endif

struct fimc_is_device_sensor_peri *find_peri_by_cis_id(struct fimc_is_device_sensor *device,
							u32 cis)
{
	u32 mindex = 0, mmax = 0;
	struct fimc_is_module_enum *module_enum = NULL;
	struct fimc_is_resourcemgr *resourcemgr = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	BUG_ON(device->instance >= FIMC_IS_SENSOR_COUNT);

	resourcemgr = device->resourcemgr;
	module_enum = device->module_enum;
	BUG_ON(!module_enum);

	if (unlikely(resourcemgr == NULL))
		return NULL;

	mmax = atomic_read(&device->module_count);
	for (mindex = 0; mindex < mmax; mindex++) {
		if (module_enum[mindex].ext.sensor_con.product_name == cis) {
			sensor_peri = (struct fimc_is_device_sensor_peri *)module_enum[mindex].private_data;
			break;
		}
	}

	if (mindex >= mmax) {
		merr("cis(%d) is not found", device, cis);
	}

	return sensor_peri;
}

struct fimc_is_device_sensor_peri *find_peri_by_act_id(struct fimc_is_device_sensor *device,
							u32 actuator)
{
	u32 mindex = 0, mmax = 0;
	struct fimc_is_module_enum *module_enum = NULL;
	struct fimc_is_resourcemgr *resourcemgr = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	resourcemgr = device->resourcemgr;
	module_enum = device->module_enum;
	BUG_ON(!module_enum);

	if (unlikely(resourcemgr == NULL))
		return NULL;

	mmax = atomic_read(&device->module_count);
	for (mindex = 0; mindex < mmax; mindex++) {
		if (module_enum[mindex].ext.actuator_con.product_name == actuator) {
			sensor_peri = (struct fimc_is_device_sensor_peri *)module_enum[mindex].private_data;
			break;
		}
	}

	if (mindex >= mmax) {
		merr("actuator(%d) is not found", device, actuator);
	}

	return sensor_peri;
}

struct fimc_is_device_sensor_peri *find_peri_by_flash_id(struct fimc_is_device_sensor *device,
							u32 flash)
{
	u32 mindex = 0, mmax = 0;
	struct fimc_is_module_enum *module_enum = NULL;
	struct fimc_is_resourcemgr *resourcemgr = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	resourcemgr = device->resourcemgr;
	module_enum = device->module_enum;
	BUG_ON(!module_enum);

	if (unlikely(resourcemgr == NULL))
		return NULL;

	mmax = atomic_read(&device->module_count);
	for (mindex = 0; mindex < mmax; mindex++) {
		if (module_enum[mindex].ext.flash_con.product_name == flash) {
			sensor_peri = (struct fimc_is_device_sensor_peri *)module_enum[mindex].private_data;
			break;
		}
	}

	if (mindex >= mmax) {
		merr("flash(%d) is not found", device, flash);
	}

	return sensor_peri;
}

struct fimc_is_device_sensor_peri *find_peri_by_preprocessor_id(struct fimc_is_device_sensor *device,
							u32 preprocessor)
{
	u32 mindex = 0, mmax = 0;
	struct fimc_is_module_enum *module_enum = NULL;
	struct fimc_is_resourcemgr *resourcemgr = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	resourcemgr = device->resourcemgr;
	module_enum = device->module_enum;
	BUG_ON(!module_enum);

	if (unlikely(resourcemgr == NULL))
		return NULL;

	mmax = atomic_read(&device->module_count);
	for (mindex = 0; mindex < mmax; mindex++) {
		if (module_enum[mindex].ext.preprocessor_con.product_name == preprocessor) {
			sensor_peri = (struct fimc_is_device_sensor_peri *)module_enum[mindex].private_data;
			break;
		}
	}

	if (mindex >= mmax) {
		merr("preprocessor(%d) is not found", device, preprocessor);
		printk("preprocessor is not found");
	}

	return sensor_peri;
}

struct fimc_is_device_sensor_peri *find_peri_by_ois_id(struct fimc_is_device_sensor *device,
							u32 ois)
{
	u32 mindex = 0, mmax = 0;
	struct fimc_is_module_enum *module_enum = NULL;
	struct fimc_is_resourcemgr *resourcemgr = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	resourcemgr = device->resourcemgr;
	module_enum = device->module_enum;
	BUG_ON(!module_enum);

	if (unlikely(resourcemgr == NULL))
		return NULL;

	mmax = atomic_read(&device->module_count);
	for (mindex = 0; mindex < mmax; mindex++) {
		if (module_enum[mindex].ext.ois_con.product_name == ois) {
			sensor_peri = (struct fimc_is_device_sensor_peri *)module_enum[mindex].private_data;
			break;
		}
	}

	if (mindex >= mmax) {
		merr("ois(%d) is not found", device, ois);
	}

	return sensor_peri;
}

static void fimc_is_sensor_init_expecting_dm(struct fimc_is_device_sensor *device,
	struct fimc_is_cis *cis)
{
	int i = 0;
	u32 m_fcount;
	u32 sensitivity;
	u64 exposureTime, long_exposure, short_exposure;
	u32 long_dgain, long_again, short_dgain, short_again;
	struct fimc_is_sensor_ctl *module_ctl;
	camera2_sensor_ctl_t *sensor_ctrl = NULL;
	camera2_sensor_uctl_t *sensor_uctrl = NULL;

	if (test_bit(FIMC_IS_SENSOR_FRONT_START, &device->state)) {
		mwarn("sensor is already stream on", device);
		goto p_err;
	}

	m_fcount = (device->fcount + 1) % EXPECT_DM_NUM;

	module_ctl = &cis->sensor_ctls[m_fcount];
	sensor_ctrl = &module_ctl->cur_cam20_sensor_ctrl;
	sensor_uctrl = &module_ctl->cur_cam20_sensor_udctrl;

	sensitivity = sensor_uctrl->sensitivity;
	exposureTime = sensor_uctrl->exposureTime;
	long_exposure = sensor_uctrl->longExposureTime;
	short_exposure = sensor_uctrl->shortExposureTime;
	long_dgain = sensor_uctrl->longDigitalGain;
	long_again = sensor_uctrl->longAnalogGain;
	short_dgain = sensor_uctrl->shortDigitalGain;
	short_again = sensor_uctrl->shortAnalogGain;

	if (module_ctl->valid_sensor_ctrl == true) {
		if (sensor_ctrl->sensitivity)
			sensitivity = sensor_ctrl->sensitivity;

		if (sensor_ctrl->exposureTime)
			exposureTime = sensor_ctrl->exposureTime;
	}

	for (i = m_fcount + 2; i < m_fcount + EXPECT_DM_NUM; i++) {
		cis->expecting_sensor_dm[i % EXPECT_DM_NUM].sensitivity = sensitivity;
		cis->expecting_sensor_dm[i % EXPECT_DM_NUM].exposureTime = exposureTime;

		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].longExposureTime = long_exposure;
		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].shortExposureTime = short_exposure;
		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].digitalGain = long_dgain;
		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].analogGain = long_dgain;
		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].longDigitalGain = long_dgain;
		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].longAnalogGain = long_dgain;
		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].shortDigitalGain = short_dgain;
		cis->expecting_sensor_udm[i % EXPECT_DM_NUM].shortAnalogGain = short_again;
	}

p_err:
	return;
}

int fimc_is_sensor_wait_streamoff(struct fimc_is_device_sensor *device)
{
	int ret = 0;
	struct fimc_is_device_csi *csi;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct fimc_is_cis cis;
	struct v4l2_subdev *subdev_module;
	struct v4l2_subdev *subdev_cis;

	BUG_ON(!device);
	BUG_ON(!device->subdev_csi);

	csi = v4l2_get_subdevdata(device->subdev_csi);
	if (!csi) {
		err("csi is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		return -EINVAL;
	}

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		return -EINVAL;
	}

	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;
	if (!sensor_peri) {
		err("sensor_peri is NULL");
		return -EINVAL;
	}

	subdev_cis = sensor_peri->subdev_cis;
	if (!subdev_cis) {
		err("subdev_cis is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	cis = sensor_peri->cis;

	/* sensor wait stream off */
	ret = CALL_CISOPS(&cis, cis_wait_streamoff, subdev_cis);
	if (ret) {
		err("[%s] wait stream off fail\n", __func__);
		goto p_err;
	}

p_err:
	return ret;
}

int fimc_is_sensor_wait_streamon(struct fimc_is_device_sensor *device)
{
	int ret = 0;
	struct fimc_is_device_csi *csi;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct fimc_is_cis cis;
	struct v4l2_subdev *subdev_module;
	struct v4l2_subdev *subdev_cis;

	BUG_ON(!device);
	BUG_ON(!device->subdev_csi);

	csi = v4l2_get_subdevdata(device->subdev_csi);
	if (!csi) {
		err("csi is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		return -EINVAL;
	}

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		return -EINVAL;
	}

	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;
	if (!sensor_peri) {
		err("sensor_peri is NULL");
		return -EINVAL;
	}

	subdev_cis = sensor_peri->subdev_cis;
	if (!subdev_cis) {
		err("subdev_cis is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	cis = sensor_peri->cis;

	/* sensor wait stream on */
	ret = CALL_CISOPS(&cis, cis_wait_streamon, subdev_cis);
	if (ret) {
		err("[%s] wait stream on fail\n", __func__);
		goto p_err;
	}

p_err:
	return ret;
}

void fimc_is_sensor_cis_status_dump_work(struct work_struct *data)
{
	int ret = 0;
	struct fimc_is_cis *cis;
	struct fimc_is_device_sensor_peri *sensor_peri;

	BUG_ON(!data);

	cis = container_of(data, struct fimc_is_cis, cis_status_dump_work);
	BUG_ON(!cis);

	sensor_peri = container_of(cis, struct fimc_is_device_sensor_peri, cis);

	if (sensor_peri->subdev_cis) {
		ret = CALL_CISOPS(cis, cis_log_status, sensor_peri->subdev_cis);
		if (ret < 0) {
			err("err!!! log_status ret(%d)", ret);
		}
	}
}

void fimc_is_sensor_set_cis_uctrl_list(struct fimc_is_device_sensor_peri *sensor_peri,
		u32 long_exp, u32 short_exp,
		u32 long_total_gain, u32 short_total_gain,
		u32 long_analog_gain, u32 short_analog_gain,
		u32 long_digital_gain, u32 short_digital_gain)
{
	int i = 0;
	camera2_sensor_uctl_t *sensor_uctl;

	BUG_ON(!sensor_peri);

	for (i = 0; i < CAM2P0_UCTL_LIST_SIZE; i++) {
		if (sensor_peri->cis.sensor_ctls[i].force_update) {
			dbg_sensor(1, "skip uctl_list set, sensor_ctl[%d]->force_update\n", i);
			continue;
		}

		sensor_uctl = &sensor_peri->cis.sensor_ctls[i].cur_cam20_sensor_udctrl;

		if (fimc_is_vender_wdr_mode_on(sensor_peri->cis.cis_data)) {
			sensor_uctl->exposureTime = 0;
			sensor_uctl->longExposureTime = fimc_is_sensor_convert_us_to_ns(long_exp);
			sensor_uctl->shortExposureTime = fimc_is_sensor_convert_us_to_ns(short_exp);

			sensor_uctl->sensitivity = long_total_gain;
			sensor_uctl->analogGain = 0;
			sensor_uctl->digitalGain = 0;

			sensor_uctl->longAnalogGain = long_analog_gain;
			sensor_uctl->shortAnalogGain = short_analog_gain;
			sensor_uctl->longDigitalGain = long_digital_gain;
			sensor_uctl->shortDigitalGain = short_digital_gain;
		} else {
			sensor_uctl->exposureTime = fimc_is_sensor_convert_us_to_ns(long_exp);
			sensor_uctl->longExposureTime = 0;
			sensor_uctl->shortExposureTime = 0;

			sensor_uctl->sensitivity = long_total_gain;
			sensor_uctl->analogGain = long_analog_gain;
			sensor_uctl->digitalGain = long_digital_gain;

			sensor_uctl->longAnalogGain = 0;
			sensor_uctl->shortAnalogGain = 0;
			sensor_uctl->longDigitalGain = 0;
			sensor_uctl->shortDigitalGain = 0;
		}
	}
}

void fimc_is_sensor_sensor_work_fn(struct kthread_work *work)
{
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct fimc_is_device_sensor *device;

	sensor_peri = container_of(work, struct fimc_is_device_sensor_peri, sensor_work);

	device = v4l2_get_subdev_hostdata(sensor_peri->subdev_cis);

	fimc_is_sensor_ctl_frame_evt(device);
}

int fimc_is_sensor_init_sensor_thread(struct fimc_is_device_sensor_peri *sensor_peri)
{
	int ret = 0;
	struct sched_param param = {.sched_priority = FIMC_IS_MAX_PRIO - 1};

	if (sensor_peri->sensor_task == NULL) {
		spin_lock_init(&sensor_peri->sensor_work_lock);
		init_kthread_worker(&sensor_peri->sensor_worker);
		sensor_peri->sensor_task = kthread_run_perf_critical(cpu_perf_mask, kthread_worker_fn,
						&sensor_peri->sensor_worker,
						"fimc_is_sen_sensor_work");
		if (IS_ERR(sensor_peri->sensor_task)) {
			err("failed to create kthread for sensor, err(%ld)",
				PTR_ERR(sensor_peri->sensor_task));
			ret = PTR_ERR(sensor_peri->sensor_task);
			sensor_peri->sensor_task = NULL;
			return ret;
		}

		ret = sched_setscheduler_nocheck(sensor_peri->sensor_task, SCHED_FIFO, &param);
		if (ret) {
			err("sched_setscheduler_nocheck is fail(%d)", ret);
			return ret;
		}

		init_kthread_work(&sensor_peri->sensor_work, fimc_is_sensor_sensor_work_fn);
	}

	return ret;
}

void fimc_is_sensor_deinit_sensor_thread(struct fimc_is_device_sensor_peri *sensor_peri)
{
	if (sensor_peri->sensor_task != NULL) {
		if (kthread_stop(sensor_peri->sensor_task))
			err("kthread_stop fail");

		sensor_peri->sensor_task = NULL;
		sensor_peri->use_sensor_work = false;
		info("%s:\n", __func__);
	}
}

int fimc_is_sensor_init_mode_change_thread(struct fimc_is_device_sensor_peri *sensor_peri)
{
	int ret = 0;
	struct sched_param param = {.sched_priority = FIMC_IS_MAX_PRIO - 1};

	/* Always first applyed to mode change when camera on */
	sensor_peri->mode_change_first = true;

	init_kthread_worker(&sensor_peri->mode_change_worker);
	sensor_peri->mode_change_task = kthread_run(kthread_worker_fn,
						&sensor_peri->mode_change_worker,
						"fimc_is_sensor_mode_change");
	if (IS_ERR(sensor_peri->mode_change_task)) {
		err("failed to create kthread fir sensor mode change, err(%ld)",
			PTR_ERR(sensor_peri->mode_change_task));
		ret = PTR_ERR(sensor_peri->mode_change_task);
		sensor_peri->mode_change_task = NULL;
		return ret;
	}

	ret = sched_setscheduler_nocheck(sensor_peri->mode_change_task, SCHED_FIFO, &param);
	if (ret) {
		err("sched_setscheduler_nocheck is fail(%d)\n", ret);
		return ret;
	}

	init_kthread_work(&sensor_peri->mode_change_work, fimc_is_sensor_mode_change_work_fn);

	return ret;
}

void fimc_is_sensor_deinit_mode_change_thread(struct fimc_is_device_sensor_peri *sensor_peri)
{
	if (sensor_peri->mode_change_task != NULL) {
		if (kthread_stop(sensor_peri->mode_change_task))
			err("kthread_stop fail");

		sensor_peri->mode_change_task = NULL;
		info("%s:\n", __func__);
	}
}

int fimc_is_sensor_initial_setting_low_exposure(struct fimc_is_device_sensor_peri *sensor_peri)
{
	int ret = 0;
	struct fimc_is_device_sensor *device;

	BUG_ON(!sensor_peri);

	device = v4l2_get_subdev_hostdata(sensor_peri->subdev_cis);
	BUG_ON(!device);

	dbg_sensor(1, "[%s] expo(%d), again(%d), dgain(%d)\n", __func__,
			sensor_peri->cis.cis_data->low_expo_start, 100, 100);

	fimc_is_sensor_peri_s_analog_gain(device, 1000, 1000);
	fimc_is_sensor_peri_s_digital_gain(device, 1000, 1000);
	fimc_is_sensor_peri_s_exposure_time(device,
			sensor_peri->cis.cis_data->low_expo_start,
			sensor_peri->cis.cis_data->low_expo_start);

#ifdef CONFIG_COMPANION_USE
	/* update cis_data about preproc */
	cis_data->preproc_auto_exposure[CURRENT_FRAME].long_exposure_coarse = cis_data->cur_long_exposure_coarse;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].short_exposure_coarse = cis_data->cur_short_exposure_coarse;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].long_exposure_analog_gain = 1000;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].short_exposure_analog_gain = 1000;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].long_exposure_digital_gain = 1000;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].short_exposure_digital_gain = 1000;
	memcpy(&cis_data->preproc_auto_exposure[NEXT_FRAME], &cis_data->preproc_auto_exposure[CURRENT_FRAME],
			sizeof(preprocessor_ae_setting));
#endif

#if !defined(DISABLE_LIB)
	sensor_peri->sensor_interface.cis_itf_ops.request_reset_expo_gain(&sensor_peri->sensor_interface,
			sensor_peri->cis.cis_data->low_expo_start,
			1000,
			1000,
			1000,
			sensor_peri->cis.cis_data->low_expo_start,
			1000,
			1000,
			1000);
#endif
	fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			sensor_peri->cis.cis_data->low_expo_start,
			sensor_peri->cis.cis_data->low_expo_start,
			1000, 1000,
			1000, 1000,
			1000, 1000);

	return ret;
}

void fimc_is_sensor_mode_change_work_fn(struct kthread_work *work)
{
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct fimc_is_cis *cis;

	TIME_LAUNCH_STR(LAUNCH_SENSOR_INIT);
	sensor_peri = container_of(work, struct fimc_is_device_sensor_peri, mode_change_work);

	cis = (struct fimc_is_cis *)v4l2_get_subdevdata(sensor_peri->subdev_cis);

	/* cis global setting is only set to first mode change time */
	if (sensor_peri->mode_change_first == true) {
		CALL_CISOPS(cis, cis_set_global_setting, cis->subdev);
	}

	CALL_CISOPS(cis, cis_mode_change, cis->subdev, cis->cis_data->sens_config_index_cur);

	sensor_peri->mode_change_first = false;
	TIME_LAUNCH_END(LAUNCH_SENSOR_INIT);
}

int fimc_is_sensor_mode_change(struct fimc_is_cis *cis, u32 mode)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri;

	BUG_ON(!cis);
	BUG_ON(!cis->cis_data);

	sensor_peri = container_of(cis, struct fimc_is_device_sensor_peri, cis);

	CALL_CISOPS(cis, cis_data_calculation, cis->subdev, cis->cis_data->sens_config_index_cur);
	queue_kthread_work(&sensor_peri->mode_change_worker, &sensor_peri->mode_change_work);

	return ret;
}

void fimc_is_sensor_setting_mode_change(struct fimc_is_device_sensor_peri *sensor_peri)
{
	struct fimc_is_device_sensor *device;
	u32 expo = 0;
	u32 tgain = 0;
	u32 again = 0;
	u32 dgain = 0;
	u32 long_expo = 0;
	u32 long_tgain = 0;
	u32 long_again = 0;
	u32 long_dgain = 0;
	u32 frame_duration = 0;
	cis_shared_data *cis_data = NULL;

	BUG_ON(!sensor_peri);

	device = v4l2_get_subdev_hostdata(sensor_peri->subdev_cis);
	BUG_ON(!device);

	cis_data = sensor_peri->cis.cis_data;
	BUG_ON(!cis_data);

	expo = sensor_peri->cis.mode_chg_expo;
	again = sensor_peri->cis.mode_chg_again;
	dgain = sensor_peri->cis.mode_chg_dgain;
	long_expo = sensor_peri->cis.mode_chg_long_expo;
	long_again = sensor_peri->cis.mode_chg_long_again;
	long_dgain = sensor_peri->cis.mode_chg_long_dgain;

	dbg_sensor(1, "[%s] expo(%d), again(%d), dgain(%d)\n", __func__,
			expo, again, dgain);

	if (expo == 0 || again < 1000 || dgain < 1000 || long_expo == 0 || long_again < 1000 || long_dgain < 1000) {
		err("[%s] invalid mode change sensor settings exp(%d), gain(%d, %d) long_exp(%d), long_gain(%d, %d)\n",
				__func__, expo, again, dgain, long_expo, long_again, long_dgain);
		expo = sensor_peri->cis.cis_data->low_expo_start;
		again = 1000;
		dgain = 1000;
		long_expo = sensor_peri->cis.cis_data->low_expo_start;
		long_again = 1000;
		long_dgain = 1000;
	}

	if (dgain > 1000)
		tgain = dgain * (again / 1000);
	else
		tgain = again;

	if (long_dgain > 1000)
		long_tgain = long_dgain * (long_again / 1000);
	else
		long_tgain = long_again;

	CALL_CISOPS(&sensor_peri->cis, cis_adjust_frame_duration, sensor_peri->subdev_cis, MAX(long_expo, expo), &frame_duration);
	fimc_is_sensor_peri_s_frame_duration(device, frame_duration);

	fimc_is_sensor_peri_s_analog_gain(device, long_again, again);
	fimc_is_sensor_peri_s_digital_gain(device, long_dgain, dgain);
	fimc_is_sensor_peri_s_exposure_time(device, long_expo, expo);

	fimc_is_sensor_peri_s_wb_gains(device, sensor_peri->cis.mode_chg_wb_gains);

#ifdef CONFIG_COMPANION_USE
	/* update cis_data about preproc */
	cis_data->preproc_auto_exposure[CURRENT_FRAME].long_exposure_coarse = cis_data->cur_long_exposure_coarse;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].short_exposure_coarse = cis_data->cur_short_exposure_coarse;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].long_exposure_analog_gain = long_again;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].short_exposure_analog_gain = again;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].long_exposure_digital_gain = long_dgain;
	cis_data->preproc_auto_exposure[CURRENT_FRAME].short_exposure_digital_gain = dgain;
	memcpy(&cis_data->preproc_auto_exposure[NEXT_FRAME], &cis_data->preproc_auto_exposure[CURRENT_FRAME],
			sizeof(preprocessor_ae_setting));
#endif

	sensor_peri->sensor_interface.cis_itf_ops.request_reset_expo_gain(&sensor_peri->sensor_interface,
			long_expo,
			long_tgain,
			long_again,
			long_dgain,
			expo,
			tgain,
			again,
			dgain);

	fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			long_expo, expo,
			long_tgain, tgain,
			long_again, again,
			long_dgain, dgain);
}

void fimc_is_sensor_flash_fire_work(struct work_struct *data)
{
	int ret = 0;
	u32 frame_duration = 0;
	struct fimc_is_flash *flash;
	struct fimc_is_flash_data *flash_data;
	struct fimc_is_device_sensor *device;
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct v4l2_subdev *subdev_flash;
	cis_shared_data *cis_data = NULL;
	u32 step = 0;
	preprocessor_ae_setting *preprocessor_ae_setting_cur;

	BUG_ON(!data);

	flash_data = container_of(data, struct fimc_is_flash_data, flash_fire_work);
	BUG_ON(!flash_data);

	flash = container_of(flash_data, struct fimc_is_flash, flash_data);
	BUG_ON(!flash);

	sensor_peri = flash->sensor_peri;
	BUG_ON(!sensor_peri);

	cis_data = sensor_peri->cis.cis_data;
	BUG_ON(!cis_data);

	subdev_flash = sensor_peri->subdev_flash;

	device = v4l2_get_subdev_hostdata(subdev_flash);
	BUG_ON(!device);

	mutex_lock(&sensor_peri->cis.control_lock);

	if (!sensor_peri->cis.cis_data->stream_on) {
		warn("[%s] already stream off\n", __func__);
		goto already_stream_off;
	}

	/* sensor stream off */
	ret = CALL_CISOPS(&sensor_peri->cis, cis_stream_off, sensor_peri->subdev_cis);
	if (ret < 0) {
		err("[%s] stream off fail\n", __func__);
		goto fail_cis_stream_off;
	}

	ret = fimc_is_sensor_wait_streamoff(device);

#ifdef CONFIG_COMPANION_DIRECT_USE
	/* preprocessor stream off */
	if (sensor_peri->subdev_preprocessor) {
		ret = CALL_PREPROPOPS(sensor_peri->preprocessor, preprocessor_stream_off,
			sensor_peri->subdev_preprocessor);
		if (ret) {
			err("[%s] preprocessor stream off fail\n", __func__);
			goto fail_preprocessor_stream_off;
		}
	}
#endif
	dbg_flash("[%s] steram off done\n", __func__);

	/* flash setting */

	step = flash->flash_ae.main_fls_strm_on_off_step;

	if (sensor_peri->sensor_interface.cis_mode == ITF_CIS_SMIA) {
		preprocessor_ae_setting_cur = &cis_data->preproc_auto_exposure[CURRENT_FRAME];

		CALL_CISOPS(&sensor_peri->cis, cis_adjust_frame_duration, sensor_peri->subdev_cis,
			flash->flash_ae.expo[step], &frame_duration);
		fimc_is_sensor_peri_s_frame_duration(device, frame_duration);

		fimc_is_sensor_peri_s_analog_gain(device, flash->flash_ae.again[step], flash->flash_ae.again[step]);
		fimc_is_sensor_peri_s_digital_gain(device, flash->flash_ae.dgain[step], flash->flash_ae.dgain[step]);
		fimc_is_sensor_peri_s_exposure_time(device, flash->flash_ae.expo[step], flash->flash_ae.expo[step]);

#ifdef CONFIG_COMPANION_USE
		/* update cis_data about preproc */
		preprocessor_ae_setting_cur->long_exposure_coarse = cis_data->cur_long_exposure_coarse;
		preprocessor_ae_setting_cur->short_exposure_coarse = cis_data->cur_short_exposure_coarse;
		preprocessor_ae_setting_cur->long_exposure_analog_gain = flash->flash_ae.again[step];
		preprocessor_ae_setting_cur->short_exposure_analog_gain = flash->flash_ae.again[step];
		preprocessor_ae_setting_cur->long_exposure_digital_gain = flash->flash_ae.dgain[step];
		preprocessor_ae_setting_cur->short_exposure_digital_gain = flash->flash_ae.dgain[step];
		memcpy(&cis_data->preproc_auto_exposure[NEXT_FRAME], preprocessor_ae_setting_cur,
				sizeof(preprocessor_ae_setting));
#endif

		sensor_peri->sensor_interface.cis_itf_ops.request_reset_expo_gain(&sensor_peri->sensor_interface,
			flash->flash_ae.expo[step],
			flash->flash_ae.tgain[step],
			flash->flash_ae.again[step],
			flash->flash_ae.dgain[step],
			flash->flash_ae.expo[step],
			flash->flash_ae.tgain[step],
			flash->flash_ae.again[step],
			flash->flash_ae.dgain[step]);

		fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			flash->flash_ae.expo[step], flash->flash_ae.expo[step],
			flash->flash_ae.tgain[step], flash->flash_ae.tgain[step],
			flash->flash_ae.again[step], flash->flash_ae.again[step],
			flash->flash_ae.dgain[step], flash->flash_ae.dgain[step]);
	} else {
		preprocessor_ae_setting_cur = &cis_data->preproc_auto_exposure[CURRENT_FRAME];

		CALL_CISOPS(&sensor_peri->cis, cis_adjust_frame_duration, sensor_peri->subdev_cis,
			MAX(flash->flash_ae.long_expo[step], flash->flash_ae.short_expo[step]), &frame_duration);
		fimc_is_sensor_peri_s_frame_duration(device, frame_duration);

		fimc_is_sensor_peri_s_analog_gain(device, flash->flash_ae.long_again[step], flash->flash_ae.short_again[step]);
		fimc_is_sensor_peri_s_digital_gain(device, flash->flash_ae.long_dgain[step], flash->flash_ae.short_dgain[step]);
		fimc_is_sensor_peri_s_exposure_time(device, flash->flash_ae.long_expo[step], flash->flash_ae.short_expo[step]);

#ifdef CONFIG_COMPANION_USE
		/* update cis_data about preproc */
		preprocessor_ae_setting_cur->long_exposure_coarse = cis_data->cur_long_exposure_coarse;
		preprocessor_ae_setting_cur->short_exposure_coarse = cis_data->cur_short_exposure_coarse;
		preprocessor_ae_setting_cur->long_exposure_analog_gain = flash->flash_ae.long_again[step];
		preprocessor_ae_setting_cur->short_exposure_analog_gain = flash->flash_ae.short_again[step];
		preprocessor_ae_setting_cur->long_exposure_digital_gain = flash->flash_ae.long_dgain[step];
		preprocessor_ae_setting_cur->short_exposure_digital_gain = flash->flash_ae.short_dgain[step];
		memcpy(&cis_data->preproc_auto_exposure[NEXT_FRAME], preprocessor_ae_setting_cur,
				sizeof(preprocessor_ae_setting));
#endif

		sensor_peri->sensor_interface.cis_itf_ops.request_reset_expo_gain(&sensor_peri->sensor_interface,
			flash->flash_ae.long_expo[step],
			flash->flash_ae.long_tgain[step],
			flash->flash_ae.long_again[step],
			flash->flash_ae.long_dgain[step],
			flash->flash_ae.short_expo[step],
			flash->flash_ae.short_tgain[step],
			flash->flash_ae.short_again[step],
			flash->flash_ae.short_dgain[step]);

		fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			flash->flash_ae.long_expo[step], flash->flash_ae.short_expo[step],
			flash->flash_ae.long_tgain[step], flash->flash_ae.short_tgain[step],
			flash->flash_ae.long_again[step], flash->flash_ae.short_again[step],
			flash->flash_ae.long_dgain[step], flash->flash_ae.short_dgain[step]);
	}

#ifdef CONFIG_COMPANION_USE
	ret = fimc_is_sensor_initial_preprocessor_setting(sensor_peri);
	if (ret) {
		err("[%s] fimc_is_sensor_initial_preprocessor_setting is fail\n", __func__);
	}

#ifdef CONFIG_COMPANION_DIRECT_USE
	if (flash->flash_ae.main_fls_ae_reset == true) {
		SENCMD_CompanionSetSeamlessModeInfoStr seamlessMode;
		seamlessMode.uSeamlessMode = false;
		if (flash->flash_ae.main_fls_strm_on_off_step == 0) {
			seamlessMode.uSeamlessMode = true;
		}
		else if (flash->flash_ae.main_fls_strm_on_off_step == 1) {
			seamlessMode.uSeamlessMode = false;
		}
		if (sensor_peri->subdev_preprocessor) {
			ret = CALL_PREPROPOPS(sensor_peri->preprocessor, preprocessor_set_seamless_mode,
									sensor_peri->subdev_preprocessor, &seamlessMode);
			if (ret) {
				err("[%s] preprocessor_set_seamless_mode fail\n", __func__);
			}
		}
	}
#endif
#endif

	dbg_flash("[%s][FLASH] mode %d, intensity %d, firing time %d us, step %d\n", __func__,
			flash->flash_data.mode,
			flash->flash_data.intensity,
			flash->flash_data.firing_time_us,
			step);

	/* flash fire */
	if (flash->flash_ae.pre_fls_ae_reset == true) {
		if (flash->flash_ae.frm_num_pre_fls != 0) {
			flash->flash_data.mode = CAM2_FLASH_MODE_OFF;
			flash->flash_data.intensity = 0;
			flash->flash_data.firing_time_us = 0;

			info("[%s] pre-flash OFF(%d), pow(%d), time(%d)\n",
					__func__,
					flash->flash_data.mode,
					flash->flash_data.intensity,
					flash->flash_data.firing_time_us);

			ret = fimc_is_sensor_flash_fire(sensor_peri, flash->flash_data.intensity);
			if (ret) {
				err("failed to turn off flash at flash expired handler\n");
			}

			flash->flash_ae.pre_fls_ae_reset = false;
			flash->flash_ae.frm_num_pre_fls = 0;
		}
	} else if (flash->flash_ae.main_fls_ae_reset == true) {
		if (flash->flash_ae.main_fls_strm_on_off_step == 0) {
			if (flash->flash_data.flash_fired == false) {
				flash->flash_data.mode = CAM2_FLASH_MODE_SINGLE;
				flash->flash_data.intensity = 10;
				flash->flash_data.firing_time_us = 500000;

				info("[%s] main-flash ON(%d), pow(%d), time(%d)\n",
					__func__,
					flash->flash_data.mode,
					flash->flash_data.intensity,
					flash->flash_data.firing_time_us);

				ret = fimc_is_sensor_flash_fire(sensor_peri, flash->flash_data.intensity);
				if (ret) {
					err("failed to turn off flash at flash expired handler\n");
#if defined(CONFIG_LEDS_S2MU106_FLASH)
					pdo_ctrl_by_flash(0);
					muic_afc_set_voltage(9);
					info("[%s]%d Down Volatge set Clear \n" ,__func__,__LINE__);
#endif
				}
			} else {
				flash->flash_ae.main_fls_ae_reset = false;
				flash->flash_ae.main_fls_strm_on_off_step = 0;
				flash->flash_ae.frm_num_main_fls[0] = 0;
				flash->flash_ae.frm_num_main_fls[1] = 0;
			}
			flash->flash_ae.main_fls_strm_on_off_step++;
		} else if (flash->flash_ae.main_fls_strm_on_off_step == 1) {
			flash->flash_data.mode = CAM2_FLASH_MODE_OFF;
			flash->flash_data.intensity = 0;
			flash->flash_data.firing_time_us = 0;

			info("[%s] main-flash OFF(%d), pow(%d), time(%d)\n",
					__func__,
					flash->flash_data.mode,
					flash->flash_data.intensity,
					flash->flash_data.firing_time_us);

			ret = fimc_is_sensor_flash_fire(sensor_peri, flash->flash_data.intensity);
			if (ret) {
				err("failed to turn off flash at flash expired handler\n");
			}
#if defined(CONFIG_LEDS_S2MU106_FLASH)
			pdo_ctrl_by_flash(0);
			muic_afc_set_voltage(9);
			info("[%s]%d Down Volatge set Clear \n" ,__func__,__LINE__);
#endif
			flash->flash_ae.main_fls_ae_reset = false;
			flash->flash_ae.main_fls_strm_on_off_step = 0;
			flash->flash_ae.frm_num_main_fls[0] = 0;
			flash->flash_ae.frm_num_main_fls[1] = 0;
		}
	}

#ifdef CONFIG_COMPANION_DIRECT_USE
	/* preprocessor stream on */
	if (sensor_peri->subdev_preprocessor) {
		ret = CALL_PREPROPOPS(sensor_peri->preprocessor, preprocessor_stream_on,
			sensor_peri->subdev_preprocessor);
		if (ret) {
			err("[%s] preprocessor stream on fail\n", __func__);
		}
	}

fail_preprocessor_stream_off:
#endif
	/* sensor stream on */
	ret = CALL_CISOPS(&sensor_peri->cis, cis_stream_on, sensor_peri->subdev_cis);
	if (ret < 0) {
		err("[%s] stream on fail\n", __func__);
	}

fail_cis_stream_off:
already_stream_off:
	mutex_unlock(&sensor_peri->cis.control_lock);

	ret = fimc_is_sensor_wait_streamon(device);
	if (ret < 0)
		err("[%s] sensor wait stream on fail\n", __func__);
}

void fimc_is_sensor_flash_expire_handler(unsigned long data)
{
	struct fimc_is_device_sensor_peri *device = (struct fimc_is_device_sensor_peri *)data;
	struct v4l2_subdev *subdev_flash;
	struct fimc_is_flash *flash;

	BUG_ON(!device);

	subdev_flash = device->subdev_flash;
	BUG_ON(!subdev_flash);

	flash = v4l2_get_subdevdata(subdev_flash);
	BUG_ON(!flash);

	schedule_work(&device->flash->flash_data.flash_expire_work);
}

void fimc_is_sensor_flash_expire_work(struct work_struct *data)
{
	int ret = 0;
	struct fimc_is_flash *flash;
	struct fimc_is_flash_data *flash_data;
	struct fimc_is_device_sensor_peri *sensor_peri;

	BUG_ON(!data);

	flash_data = container_of(data, struct fimc_is_flash_data, flash_expire_work);
	BUG_ON(!flash_data);

	flash = container_of(flash_data, struct fimc_is_flash, flash_data);
	BUG_ON(!flash);

	sensor_peri = flash->sensor_peri;

	ret = fimc_is_sensor_flash_fire(sensor_peri, 0);
	if (ret) {
		err("failed to turn off flash at flash expired handler\n");
	}
}

void fimc_is_sensor_flash_muic_ctrl_and_fire(struct work_struct *data)
{
#if defined(CONFIG_LEDS_S2MU106_FLASH)
	struct fimc_is_flash *flash;
	struct fimc_is_flash_data *flash_data;
	struct fimc_is_device_sensor_peri *sensor_peri;

	FIMC_BUG_VOID(!data);

	flash_data = container_of(data, struct fimc_is_flash_data,
								work_flash_muic_ctrl_and_fire);
	FIMC_BUG_VOID(!flash_data);

	flash = container_of(flash_data, struct fimc_is_flash, flash_data);
	FIMC_BUG_VOID(!flash);

	sensor_peri = flash->sensor_peri;

	/* Pre-flash on */
	if (flash->flash_data.mode == 3) {
		muic_afc_set_voltage(5);
		pdo_ctrl_by_flash(1);
		info("[%s]%d Down Volatge set On \n" ,__func__,__LINE__);
	}

	info("[%s] pre-flash mode(%d), pow(%d), time(%d)\n", __func__,
		flash->flash_data.mode,
		flash->flash_data.intensity, flash->flash_data.firing_time_us);

	/* If pre-flash on failed, set voltage to 9V */
	if (fimc_is_sensor_flash_fire(sensor_peri, flash->flash_data.intensity)) {
		err("failed to turn off flash at flash expired handler\n");
		if(flash->flash_data.mode == 3) {
			pdo_ctrl_by_flash(0);
			muic_afc_set_voltage(9);
			info("[%s]%d Down Volatge set Clear \n" ,__func__,__LINE__);
		}
	}
#endif
}

int fimc_is_sensor_flash_fire(struct fimc_is_device_sensor_peri *device,
				u32 on)
{
	int ret = 0;
	struct v4l2_subdev *subdev_flash;
	struct fimc_is_flash *flash;
	struct v4l2_control ctrl;

	BUG_ON(!device);

	subdev_flash = device->subdev_flash;
	if (!subdev_flash) {
		err("subdev_flash is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	flash = v4l2_get_subdevdata(subdev_flash);
	if (!flash) {
		err("flash is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	if (flash->flash_data.mode == CAM2_FLASH_MODE_OFF && on == 1) {
		err("Flash mode is off");
		flash->flash_data.flash_fired = false;
		goto p_err;
	}

	if (flash->flash_data.flash_fired != (bool)on) {
		ctrl.id = V4L2_CID_FLASH_SET_FIRE;
		ctrl.value = on ? flash->flash_data.intensity : 0;
		ret = v4l2_subdev_call(subdev_flash, core, s_ctrl, &ctrl);
		if (ret < 0) {
			err("err!!! ret(%d)", ret);
			goto p_err;
		}
		flash->flash_data.flash_fired = (bool)on;
	}

	if (flash->flash_data.mode == CAM2_FLASH_MODE_SINGLE ||
			flash->flash_data.mode == CAM2_FLASH_MODE_OFF) {
		if (flash->flash_data.flash_fired == true) {
			/* Flash firing time have to be setted in case of capture flash
			 * Max firing time of capture flash is 1 sec
			 */
			if (flash->flash_data.firing_time_us == 0 || flash->flash_data.firing_time_us > 1 * 1000 * 1000)
				flash->flash_data.firing_time_us = 1 * 1000 * 1000;

			setup_timer(&flash->flash_data.flash_expire_timer, fimc_is_sensor_flash_expire_handler, (unsigned long)device);
			mod_timer(&flash->flash_data.flash_expire_timer, jiffies +  usecs_to_jiffies(flash->flash_data.firing_time_us));
		} else {
			if (flash->flash_data.flash_expire_timer.data) {
				del_timer(&flash->flash_data.flash_expire_timer);
				flash->flash_data.flash_expire_timer.data = (unsigned long)NULL;
			}
		}
	}

p_err:
	return ret;
}

int fimc_is_sensor_peri_notify_actuator(struct v4l2_subdev *subdev, void *arg)
{
	int ret = 0;
	u32 frame_index;
	struct fimc_is_module_enum *module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_actuator_interface *actuator_itf = NULL;

	BUG_ON(!subdev);
	BUG_ON(!arg);

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;
	if (unlikely(!sensor_peri)) {
		err("%s, sensor_peri is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		dbg_sensor(1, "%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		goto p_err;
	}

	actuator_itf = &sensor_peri->sensor_interface.actuator_itf;

	/* Set expecting actuator position */
	frame_index = (*(u32 *)arg + 1) % EXPECT_DM_NUM;
	sensor_peri->cis.expecting_lens_udm[frame_index].pos = actuator_itf->virtual_pos;

	dbg_actuator("%s: expexting frame cnt(%d), algorithm position(%d)\n",
			__func__, (*(u32 *)arg + 1), actuator_itf->virtual_pos);

p_err:

	return ret;
}

int fimc_is_sensor_peri_notify_vsync(struct v4l2_subdev *subdev, void *arg)
{
	int ret = 0;
	u32 vsync_count = 0;
	struct fimc_is_cis *cis = NULL;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!subdev);
	BUG_ON(!arg);

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	vsync_count = *(u32 *)arg;

	cis = (struct fimc_is_cis *)v4l2_get_subdevdata(sensor_peri->subdev_cis);

	cis->cis_data->sen_vsync_count = vsync_count;

	if (sensor_peri->sensor_interface.otf_flag_3aa == false
			|| sensor_peri->use_sensor_work
			|| (cis->cis_data->video_mode == true &&
			cis->cis_data->max_fps >= 60)) {
		/* run sensor setting thread */
		queue_kthread_work(&sensor_peri->sensor_worker, &sensor_peri->sensor_work);
	}

	if (sensor_peri->subdev_flash != NULL) {
	    ret = fimc_is_sensor_peri_notify_flash_fire(subdev, arg);
	    if (unlikely(ret < 0))
		err("err!!!(%s), notify flash fire fail(%d)", __func__, ret);
	}

	if (test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		/* M2M case */
		if (sensor_peri->sensor_interface.otf_flag_3aa == false) {
			if (sensor_peri->actuator->valid_flag == 1)
				do_gettimeofday(&sensor_peri->actuator->start_time);

			ret = fimc_is_actuator_notify_m2m_actuator(subdev);
			if (ret)
				err("err!!!(%s), sensor notify M2M actuator fail(%d)", __func__, ret);
		}
	}

	/* Sensor Long Term Exposure mode(LTE mode) set */
	if (cis->long_term_mode.sen_strm_off_on_enable) {
		if ((cis->long_term_mode.frame_interval == cis->long_term_mode.frm_num_strm_off_on_interval) ||
				(cis->long_term_mode.frame_interval <= 0)) {
			schedule_work(&sensor_peri->cis.long_term_mode_work);
		}
		if (cis->long_term_mode.frame_interval > 0)
			cis->long_term_mode.frame_interval--;
	}

p_err:
	return ret;
}

#define cal_dur_time(st, ed) ((ed.tv_sec - st.tv_sec) + (ed.tv_usec - st.tv_usec))
int fimc_is_sensor_peri_notify_vblank(struct v4l2_subdev *subdev, void *arg)
{
	int ret = 0;
	struct fimc_is_module_enum *module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_actuator *actuator = NULL;

	BUG_ON(!subdev);
	BUG_ON(!arg);

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	if (unlikely(!module)) {
		err("%s, module is NULL", __func__);
		return -EINVAL;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;
	if (unlikely(!sensor_peri)) {
		err("%s, sensor_peri is NULL", __func__);
		return -EINVAL;
	}
	actuator = sensor_peri->actuator;

	if (test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state)) {
		/* M2M case */
		if (sensor_peri->sensor_interface.otf_flag_3aa == false) {
			/* valid_time is calculated at once */
			if (actuator->valid_flag == 1) {
				actuator->valid_flag = 0;

				do_gettimeofday(&actuator->end_time);
				actuator->valid_time = cal_dur_time(actuator->start_time, actuator->end_time);
			}
		}

		ret = fimc_is_sensor_peri_notify_actuator(subdev, arg);
		if (ret < 0) {
			err("%s, notify_actuator is NULL", __func__);
			return -EINVAL;
		}
	}

	return ret;
}

int fimc_is_sensor_peri_notify_flash_fire(struct v4l2_subdev *subdev, void *arg)
{
	int ret = 0;
	u32 vsync_count = 0;
	struct fimc_is_module_enum *module = NULL;
	struct fimc_is_flash *flash = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!subdev);
	BUG_ON(!arg);

	vsync_count = *(u32 *)arg;

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	BUG_ON(!module);

	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	flash = sensor_peri->flash;
	BUG_ON(!flash);

	dbg_flash("[%s](%d), notify flash mode(%d), pow(%d), time(%d), pre-num(%d), main_num(%d)\n",
		__func__, vsync_count,
		flash->flash_data.mode,
		flash->flash_data.intensity,
		flash->flash_data.firing_time_us,
		flash->flash_ae.frm_num_pre_fls,
		flash->flash_ae.frm_num_main_fls[flash->flash_ae.main_fls_strm_on_off_step]);


	if (flash->flash_ae.frm_num_pre_fls != 0) {
		dbg_flash("[%s](%d), pre-flash schedule\n", __func__, vsync_count);

		schedule_work(&sensor_peri->flash->flash_data.flash_fire_work);
	}

	if (flash->flash_ae.frm_num_main_fls[flash->flash_ae.main_fls_strm_on_off_step] != 0) {
		if (flash->flash_ae.frm_num_main_fls[flash->flash_ae.main_fls_strm_on_off_step] == vsync_count) {
			dbg_flash("[%s](%d), main-flash schedule\n", __func__, vsync_count);

			schedule_work(&sensor_peri->flash->flash_data.flash_fire_work);
		}
	}

	return ret;
}

int fimc_is_sensor_peri_notify_actuator_init(struct v4l2_subdev *subdev)
{
	int ret = 0;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!subdev);

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	if (!module) {
		err("%s, module is NULL", __func__);
		ret = -EINVAL;
			goto p_err;
	}

	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	if (test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state) &&
		(sensor_peri->actuator->actuator_data.actuator_init)) {
		ret = v4l2_subdev_call(sensor_peri->subdev_actuator, core, init, 0);
		if (ret)
			warn("Actuator init fail\n");

		sensor_peri->actuator->actuator_data.actuator_init = false;
	}

p_err:
	return ret;
}

int fimc_is_sensor_peri_pre_flash_fire(struct v4l2_subdev *subdev, void *arg)
{
	int ret = 0;
	u32 vsync_count = 0;
	u32 applied_frame_num = 0;
	struct fimc_is_module_enum *module = NULL;
	struct fimc_is_flash *flash = NULL;
	struct fimc_is_sensor_ctl *sensor_ctl = NULL;
	camera2_flash_uctl_t *flash_uctl = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!subdev);
	BUG_ON(!arg);

	vsync_count = *(u32 *)arg;

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	BUG_ON(!module);

	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;
	applied_frame_num = vsync_count - sensor_peri->sensor_interface.diff_bet_sen_isp;

	flash = sensor_peri->flash;
	BUG_ON(!flash);
	
	mutex_lock(&sensor_peri->cis.control_lock);

	sensor_ctl = &sensor_peri->cis.sensor_ctls[applied_frame_num % CAM2P0_UCTL_LIST_SIZE];

	if (sensor_ctl->valid_flash_udctrl == false || vsync_count != sensor_ctl->flash_frame_number)
		goto p_err;

	flash_uctl = &sensor_ctl->cur_cam20_flash_udctrl;

	if ((flash_uctl->flashMode != flash->flash_data.mode) ||
		(flash_uctl->flashMode != CAM2_FLASH_MODE_OFF && flash_uctl->firingPower == 0)) {
		flash->flash_data.mode = flash_uctl->flashMode;
		flash->flash_data.intensity = flash_uctl->firingPower;
		flash->flash_data.firing_time_us = flash_uctl->firingTime;

#if defined(CONFIG_LEDS_S2MU106_FLASH)
		schedule_work(&sensor_peri->flash->flash_data.work_flash_muic_ctrl_and_fire);
#else
		info("[%s](%d) pre-flash mode(%d), pow(%d), time(%d)\n", __func__,
			vsync_count, flash->flash_data.mode,
			flash->flash_data.intensity, flash->flash_data.firing_time_us);
		ret = fimc_is_sensor_flash_fire(sensor_peri, flash->flash_data.intensity);
#endif
	}

	/* HACK: reset uctl */
	flash_uctl->flashMode = 0;
	flash_uctl->firingPower = 0;
	flash_uctl->firingTime = 0;
	sensor_ctl->flash_frame_number = 0;
	sensor_ctl->valid_flash_udctrl = false;

p_err:
	mutex_unlock(&sensor_peri->cis.control_lock);
	return ret;
}

void fimc_is_sensor_peri_m2m_actuator(struct work_struct *data)
{
	int ret = 0;
	int index;
	struct fimc_is_device_sensor *device;
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct fimc_is_actuator_interface *actuator_itf;
	struct fimc_is_actuator *actuator;
	struct fimc_is_actuator_data *actuator_data;
	u32 pre_position, request_frame_cnt;
	u32 cur_frame_cnt;
	u32 i;

	actuator_data = container_of(data, struct fimc_is_actuator_data, actuator_work);
	BUG_ON(!actuator_data);

	actuator = container_of(actuator_data, struct fimc_is_actuator, actuator_data);
	BUG_ON(!actuator);

	sensor_peri = actuator->sensor_peri;
	BUG_ON(!sensor_peri);

	device = v4l2_get_subdev_hostdata(sensor_peri->subdev_actuator);
	BUG_ON(!device);

	actuator_itf = &sensor_peri->sensor_interface.actuator_itf;

	cur_frame_cnt = device->ischain->group_3aa.fcount;
	request_frame_cnt = sensor_peri->actuator->pre_frame_cnt[0];
	pre_position = sensor_peri->actuator->pre_position[0];
	index = sensor_peri->actuator->actuator_index;

	for (i = 0; i < index; i++) {
		sensor_peri->actuator->pre_position[i] = sensor_peri->actuator->pre_position[i+1];
		sensor_peri->actuator->pre_frame_cnt[i] = sensor_peri->actuator->pre_frame_cnt[i+1];
	}

	/* After moving index, latest value change is Zero */
	sensor_peri->actuator->pre_position[index] = 0;
	sensor_peri->actuator->pre_frame_cnt[index] = 0;

	sensor_peri->actuator->actuator_index --;
	index = sensor_peri->actuator->actuator_index;

	if (cur_frame_cnt != request_frame_cnt)
		warn("AF frame count is not match (AF request count : %d, setting request count : %d\n",
				request_frame_cnt, cur_frame_cnt);

	ret = fimc_is_actuator_ctl_set_position(device, pre_position);
	if (ret < 0) {
		err("err!!! ret(%d), invalid position(%d)",
				ret, pre_position);
	}
	actuator_itf->hw_pos = pre_position;

	dbg_sensor(1, "%s: pre_frame_count(%d), pre_position(%d), cur_frame_cnt (%d), index(%d)\n",
			__func__,
			request_frame_cnt,
			pre_position,
			cur_frame_cnt,
			index);
}

void fimc_is_sensor_long_term_mode_set_work(struct work_struct *data)
{
	int ret = 0;
	int i = 0;
	struct fimc_is_cis *cis = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct v4l2_subdev *subdev_cis;
	struct fimc_is_device_sensor *device;
	u32 step = 0;
	u32 frame_duration = 0;

	BUG_ON(!data);

	cis = container_of(data, struct fimc_is_cis, long_term_mode_work);
	BUG_ON(!cis);
	BUG_ON(!cis->cis_data);

	sensor_peri = container_of(cis, struct fimc_is_device_sensor_peri, cis);
	BUG_ON(!sensor_peri);

	device = v4l2_get_subdev_hostdata(sensor_peri->subdev_flash);
	BUG_ON(!device);

	subdev_cis = sensor_peri->subdev_cis;
	if (!subdev_cis) {
		err("[%s]: no subdev_cis", __func__);
		ret = -ENXIO;
		return;
	}

	info("[%s] start\n", __func__);
	/* Sensor stream off */
	ret = CALL_CISOPS(cis, cis_stream_off, subdev_cis);
	if (ret < 0) {
		err("[%s] stream off fail\n", __func__);
		return;
	}

	ret = fimc_is_sensor_wait_streamoff(device);
	if (ret < 0) {
		err("[%s] stream off fail\n", __func__);
		return;
	}

#ifdef CONFIG_COMPANION_DIRECT_USE
	/* Preprocessor stream off */
	if (sensor_peri->subdev_preprocessor) {
		ret = CALL_PREPROPOPS(sensor_peri->preprocessor, preprocessor_stream_off,
				sensor_peri->subdev_preprocessor);
		if (ret) {
			err("[%s] preprocessor stream off fail\n", __func__);
			return;
		}
	}
#endif
	dbg_sensor(1, "[%s] stream off done\n", __func__);

	step = cis->long_term_mode.sen_strm_off_on_step;
	if (step >= 1)
		cis->long_term_mode.sen_strm_off_on_enable = 0;

	/* LTE mode setting */
	ret = CALL_CISOPS(cis, cis_set_long_term_exposure, subdev_cis);
	if (ret < 0) {
		err("[%s] long term exposure set fail\n", __func__);
		return;
	}

#ifdef CONFIG_COMPANION_DIRECT_USE
	/* Preprocessor lemode setting */
	if (sensor_peri->subdev_preprocessor) {
		ret = CALL_PREPROPOPS(sensor_peri->preprocessor, preprocessor_set_le_mode,
				sensor_peri->subdev_preprocessor, &cis->long_term_mode.lemode_set);
		if (ret) {
			err("[%s] preprocessor stream off fail\n", __func__);
			return;
		}
	}
#endif

	CALL_CISOPS(&sensor_peri->cis, cis_adjust_frame_duration, sensor_peri->subdev_cis,
			cis->long_term_mode.expo[step], &frame_duration);
	fimc_is_sensor_peri_s_frame_duration(device, frame_duration);
	fimc_is_sensor_peri_s_analog_gain(device, cis->long_term_mode.again[step], cis->long_term_mode.again[step]);
	fimc_is_sensor_peri_s_digital_gain(device, cis->long_term_mode.dgain[step], cis->long_term_mode.dgain[step]);
	fimc_is_sensor_peri_s_exposure_time(device, cis->long_term_mode.expo[step], cis->long_term_mode.expo[step]);

	sensor_peri->sensor_interface.cis_itf_ops.request_reset_expo_gain(&sensor_peri->sensor_interface,
			cis->long_term_mode.expo[step],
			cis->long_term_mode.tgain[step],
			cis->long_term_mode.again[step],
			cis->long_term_mode.dgain[step],
			cis->long_term_mode.expo[step],
			cis->long_term_mode.tgain[step],
			cis->long_term_mode.again[step],
			cis->long_term_mode.dgain[step]);

	fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			cis->long_term_mode.expo[step], cis->long_term_mode.expo[step],
			cis->long_term_mode.tgain[step], cis->long_term_mode.tgain[step],
			cis->long_term_mode.again[step], cis->long_term_mode.again[step],
			cis->long_term_mode.dgain[step], cis->long_term_mode.dgain[step]);

	step = cis->long_term_mode.sen_strm_off_on_step++;

#ifdef CONFIG_COMPANION_DIRECT_USE
	/* Preprocessor stream on */
	if (sensor_peri->subdev_preprocessor) {
		ret = CALL_PREPROPOPS(sensor_peri->preprocessor, preprocessor_stream_on,
				sensor_peri->subdev_preprocessor);
		if (ret) {
			err("[%s] preprocessor stream on fail\n", __func__);
			return;
		}
	}
#endif

	/* Sensor stream on */
	ret = CALL_CISOPS(cis, cis_stream_on, subdev_cis);
	if (ret < 0) {
		err("[%s] stream off fail\n", __func__);
		return;
	}
	dbg_sensor(1, "[%s] stream on done\n", __func__);

	/* Reset when step value is 2 */
	if (step >= 1) {
		for (i = 0; i < 2; i++) {
			cis->long_term_mode.expo[i] = 0;
			cis->long_term_mode.tgain[i] = 0;
			cis->long_term_mode.again[i] = 0;
			cis->long_term_mode.dgain[i] = 0;
			cis->long_term_mode.sen_strm_off_on_step = 0;
			cis->long_term_mode.frm_num_strm_off_on_interval = 0;
		}
	} else {
		cis->long_term_mode.lemode_set.lemode = 0;
	}

	info("[%s] end\n", __func__);
}

void fimc_is_sensor_peri_init_work(struct fimc_is_device_sensor_peri *sensor_peri)
{
	BUG_ON(!sensor_peri);

	if (sensor_peri->flash) {
		INIT_WORK(&sensor_peri->flash->flash_data.flash_fire_work, fimc_is_sensor_flash_fire_work);
		INIT_WORK(&sensor_peri->flash->flash_data.flash_expire_work, fimc_is_sensor_flash_expire_work);
		INIT_WORK(&sensor_peri->flash->flash_data.work_flash_muic_ctrl_and_fire,
									fimc_is_sensor_flash_muic_ctrl_and_fire);
	}

	INIT_WORK(&sensor_peri->cis.cis_status_dump_work, fimc_is_sensor_cis_status_dump_work);

	if (sensor_peri->actuator) {
		INIT_WORK(&sensor_peri->actuator->actuator_data.actuator_work, fimc_is_sensor_peri_m2m_actuator);
		hrtimer_init(&sensor_peri->actuator->actuator_data.afwindow_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	}

	/* Init to LTE mode work */
	INIT_WORK(&sensor_peri->cis.long_term_mode_work, fimc_is_sensor_long_term_mode_set_work);
}

void fimc_is_sensor_peri_probe(struct fimc_is_device_sensor_peri *sensor_peri)
{
	BUG_ON(!sensor_peri);

	clear_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state);
	clear_bit(FIMC_IS_SENSOR_FLASH_AVAILABLE, &sensor_peri->peri_state);
	clear_bit(FIMC_IS_SENSOR_PREPROCESSOR_AVAILABLE, &sensor_peri->peri_state);
	clear_bit(FIMC_IS_SENSOR_OIS_AVAILABLE, &sensor_peri->peri_state);
	clear_bit(FIMC_IS_SENSOR_APERTURE_AVAILABLE, &sensor_peri->peri_state);

	mutex_init(&sensor_peri->cis.control_lock);
}

int sensor_module_power_reset(struct v4l2_subdev *subdev, struct fimc_is_device_sensor *device)
{
	int ret = 0;
	struct fimc_is_module_enum *module;

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev);
	FIMC_BUG(!module);

	ret = fimc_is_sensor_gpio_off(device);
	if (ret)
		err("gpio off is fail(%d)", ret);

	ret = fimc_is_sensor_mclk_off(device, device->pdata->scenario, module->pdata->mclk_ch);
	if (ret)
		err("fimc_is_sensor_mclk_off is fail(%d)", ret);

	usleep_range(10000, 10000);

	ret = fimc_is_sensor_mclk_on(device, device->pdata->scenario, module->pdata->mclk_ch);
	if (ret)
		err("fimc_is_sensor_mclk_on is fail(%d)", ret);

	ret = fimc_is_sensor_gpio_on(device);
	if (ret)
		err("gpio on is fail(%d)", ret);

	usleep_range(10000, 10000);

	return ret;
}

int fimc_is_sensor_peri_s_stream(struct fimc_is_device_sensor *device,
					bool on)
{
	int ret = 0;
	int i = 0;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct v4l2_subdev *subdev_cis = NULL;
	struct v4l2_subdev *subdev_preprocessor = NULL;
	struct fimc_is_cis *cis = NULL;
	struct fimc_is_preprocessor *preprocessor = NULL;
	struct fimc_is_core *core = NULL;
	struct fimc_is_dual_info *dual_info = NULL;
	int testcnt = 0;

	BUG_ON(!device);

	core = (struct fimc_is_core *)device->private_data;
	dual_info = &core->dual_info;

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	subdev_cis = sensor_peri->subdev_cis;
	if (!subdev_cis) {
		err("[SEN:%d] no subdev_cis(s_stream, on:%d)", module->sensor_id, on);
		ret = -ENXIO;
		goto p_err;
	}
	cis = (struct fimc_is_cis *)v4l2_get_subdevdata(subdev_cis);
	BUG_ON(!cis);
	BUG_ON(!cis->cis_data);

	subdev_preprocessor = sensor_peri->subdev_preprocessor;
	if (subdev_preprocessor) {
		preprocessor = (struct fimc_is_preprocessor *)v4l2_get_subdevdata(subdev_preprocessor);
		BUG_ON(!preprocessor);
	}

	ret = fimc_is_sensor_peri_debug_fixed((struct fimc_is_device_sensor *)v4l2_get_subdev_hostdata(subdev_module));
	if (ret) {
		err("fimc_is_sensor_peri_debug_fixed is fail(%d)", ret);
		goto p_err;
	}

	if (on) {
		fimc_is_sensor_init_expecting_dm(device, cis);

		/* If sensor setting @work is queued or executing,
		   wait for it to finish execution when working s_format */
		flush_kthread_work(&sensor_peri->mode_change_work);

		/* stream on sequence */
		if (cis->need_mode_change == false && cis->use_initial_ae == false)
		{
			/* only first time after camera on */
			fimc_is_sensor_initial_setting_low_exposure(sensor_peri);
			cis->need_mode_change = true;
			if (sensor_peri->actuator)
				sensor_peri->actuator->actuator_data.timer_check = HRTIMER_POSSIBLE;
		} else {
			fimc_is_sensor_setting_mode_change(sensor_peri);
		}

#ifdef CONFIG_COMPANION_DIRECT_USE
		if (subdev_preprocessor) {
			ret = CALL_PREPROPOPS(preprocessor, preprocessor_wait_s_input, subdev_preprocessor);
			if (ret) {
				err("[SEN:%d] preprocessor wait s_input timeout\n", module->sensor_id);
				goto p_err;
			}

			/* Before stream on preprocessor, do stream off and mode change */
			ret = CALL_PREPROPOPS(preprocessor, preprocessor_stream_off, subdev_preprocessor);
			if (ret) {
				err("[SEN:%d] v4l2_subdev_call(preprocessor_stream, off:%d) is fail(%d)",
						module->sensor_id, on, ret);
				goto p_err;
			}

			ret = CALL_PREPROPOPS(preprocessor, preprocessor_mode_change, subdev_preprocessor, device);
			if (ret) {
				err("[SEN:%d] v4l2_subdev_call(preprocessor_mode_change) is fail(%d)",
						module->sensor_id, ret);
				goto p_err;
			}

			ret = fimc_is_sensor_initial_preprocessor_setting(sensor_peri);
			if (ret) {
				err("[SEN:%d] v4l2_subdev_call(sensor_initial_preprocessor_setting) is fail(%d)",
						module->sensor_id, ret);
				goto p_err;
			}

			ret = CALL_PREPROPOPS(preprocessor, preprocessor_stream_on, subdev_preprocessor);
			if (ret) {
				err("[SEN:%d] v4l2_subdev_call(preprocessor_stream, on:%d) is fail(%d)",
						module->sensor_id, on, ret);
				goto p_err;
			}
		}
#endif
		ret = CALL_CISOPS(cis, cis_stream_on, subdev_cis);
		if (ret < 0) {
			err("[%s]: sensor stream on fail\n", __func__);
		} else {
stream_on_retry:
			ret = fimc_is_sensor_wait_streamon(device);
			if (ret < 0) {
				err("[%s]: sensor wait stream on fail\n", __func__);

				/* retry stream on */
				if ((testcnt <= 1) && (module->sensor_id == SENSOR_NAME_S5K3L6)) {
					testcnt++;
#ifdef ENABLE_DTP
					if (device->dtp_check) {
						device->dtp_check = false;
						if (timer_pending(&device->dtp_timer))
							del_timer_sync(&device->dtp_timer);
					}
#endif
					ret = CALL_CISOPS(cis, cis_stream_off, subdev_cis);
					ret |= fimc_is_sensor_wait_streamoff(device);
					if(ret < 0) {
						err("[%s]: sensor wait stream off fail\n", __func__);
					}

					ret = sensor_module_power_reset(subdev_module, device);
					ret |= CALL_CISOPS(cis, cis_set_global_setting, subdev_cis);
					ret |= CALL_CISOPS(cis, cis_mode_change, subdev_cis, cis->cis_data->sens_config_index_cur);
					fimc_is_sensor_setting_mode_change(sensor_peri);
#ifdef ENABLE_DTP
					device->dtp_check = true;
					mod_timer(&device->dtp_timer, jiffies +  msecs_to_jiffies(300));
#endif
					ret |= CALL_CISOPS(cis, cis_stream_on, subdev_cis);

					if (ret < 0) {
						err("[%s]: sensor wait stream on fail - retry fail\n", __func__);
						goto p_err;
					}
					goto stream_on_retry;
				}
			}
		}

	} else {
		/* stream off sequence */
		mutex_lock(&cis->control_lock);
		ret = CALL_CISOPS(cis, cis_stream_off, subdev_cis);
		mutex_unlock(&cis->control_lock);

#ifdef CONFIG_COMPANION_DIRECT_USE
		if (subdev_preprocessor){
			ret = CALL_PREPROPOPS(preprocessor, preprocessor_stream_off, subdev_preprocessor);
			if (ret) {
				err("[SEN:%d] v4l2_subdev_call(preprocessor_stream, off:%d) is fail(%d)",
						module->sensor_id, on, ret);
				goto p_err;
			}
		}
#endif

		mutex_lock(&cis->control_lock);
		if (sensor_peri->flash != NULL && dual_info->mode == FIMC_IS_DUAL_MODE_NOTHING) {
			sensor_peri->flash->flash_data.mode = CAM2_FLASH_MODE_OFF;
			if (sensor_peri->flash->flash_data.flash_fired == true) {
				ret = fimc_is_sensor_flash_fire(sensor_peri, 0);
				if (ret) {
					err("failed to turn off flash at flash expired handler\n");
				}
			}
		}

		if (ret == 0)
			ret = fimc_is_sensor_wait_streamoff(device);

		if (sensor_peri->actuator)
			hrtimer_cancel(&sensor_peri->actuator->actuator_data.afwindow_timer);
		memset(&sensor_peri->cis.cur_sensor_uctrl, 0, sizeof(camera2_sensor_uctl_t));
		memset(&sensor_peri->cis.expecting_sensor_dm[0], 0, sizeof(camera2_sensor_dm_t) * EXPECT_DM_NUM);
		memset(&sensor_peri->cis.expecting_sensor_udm[0], 0, sizeof(camera2_sensor_udm_t) * EXPECT_DM_NUM);
		for (i = 0; i < CAM2P0_UCTL_LIST_SIZE; i++) {
			memset(&sensor_peri->cis.sensor_ctls[i].cur_cam20_sensor_udctrl, 0, sizeof(camera2_sensor_uctl_t));
			sensor_peri->cis.sensor_ctls[i].valid_sensor_ctrl = 0;
			sensor_peri->cis.sensor_ctls[i].force_update = false;
			memset(&sensor_peri->cis.sensor_ctls[i].cur_cam20_flash_udctrl, 0, sizeof(camera2_flash_uctl_t));
			sensor_peri->cis.sensor_ctls[i].valid_flash_udctrl = false;
		}
		mutex_unlock(&cis->control_lock);
		sensor_peri->use_sensor_work = false;
	}
	if (ret) {
		err("[SEN:%d] v4l2_subdev_call(s_stream, on:%d) is fail(%d)",
				module->sensor_id, on, ret);
		goto p_err;
	}

#ifdef HACK_SDK_RESET
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;
	sensor_peri->sensor_interface.reset_flag = false;
#endif

p_err:
	return ret;
}

int fimc_is_sensor_peri_s_frame_duration(struct fimc_is_device_sensor *device,
					u32 frame_duration)
{
	int ret = 0;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

#ifdef FIXED_SENSOR_DEBUG
	if (unlikely(sysfs_sensor.is_en == true)) {
		frame_duration = FPS_TO_DURATION_US(sysfs_sensor.frame_duration);
		dbg_sensor(1, "sysfs_sensor.frame_duration = %d\n", sysfs_sensor.frame_duration);
	}
#endif

	ret = CALL_CISOPS(&sensor_peri->cis, cis_set_frame_duration, sensor_peri->subdev_cis, frame_duration);
	if (ret < 0) {
		err("err!!! ret(%d)", ret);
		goto p_err;
	}
	device->frame_duration = frame_duration;

p_err:
	return ret;
}

int fimc_is_sensor_peri_s_exposure_time(struct fimc_is_device_sensor *device,
				u32 long_exposure_time, u32 short_exposure_time)
{
	int ret = 0;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct ae_param exposure;

	BUG_ON(!device);

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	if ((long_exposure_time <= 0) || (short_exposure_time <= 0)) {
		err("it is wrong exposure time (long:%d, short:%d)", long_exposure_time, short_exposure_time);
		ret = -EINVAL;
		goto p_err;
	}

#ifdef FIXED_SENSOR_DEBUG
	if (unlikely(sysfs_sensor.is_en == true)) {
		long_exposure_time = sysfs_sensor.long_exposure_time;
		short_exposure_time = sysfs_sensor.short_exposure_time;
		dbg_sensor(1, "exposure = %d %d\n", long_exposure_time, short_exposure_time);
	}
#endif

	exposure.long_val = long_exposure_time;
	exposure.short_val = short_exposure_time;
	ret = CALL_CISOPS(&sensor_peri->cis, cis_set_exposure_time, sensor_peri->subdev_cis, &exposure);
	if (ret < 0) {
		err("err!!! ret(%d)", ret);
		goto p_err;
	}
	device->exposure_time = long_exposure_time;

p_err:
	return ret;
}

int fimc_is_sensor_peri_s_analog_gain(struct fimc_is_device_sensor *device,
	u32 long_analog_gain, u32 short_analog_gain)
{
	int ret = 0;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct ae_param again;

	BUG_ON(!device);

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	if (long_analog_gain <= 0 ) {
		err("it is wrong analog gain(%d)", long_analog_gain);
		ret = -EINVAL;
		goto p_err;
	}

#ifdef FIXED_SENSOR_DEBUG
	if (unlikely(sysfs_sensor.is_en == true)) {
		long_analog_gain = sysfs_sensor.long_analog_gain * 10;
		short_analog_gain = sysfs_sensor.short_analog_gain * 10;
		dbg_sensor(1, "again = %d %d\n", sysfs_sensor.long_analog_gain, sysfs_sensor.short_analog_gain);
	}
#endif

	again.long_val = long_analog_gain;
	again.short_val = short_analog_gain;
	ret = CALL_CISOPS(&sensor_peri->cis, cis_set_analog_gain, sensor_peri->subdev_cis, &again);
	if (ret < 0) {
		err("err!!! ret(%d)", ret);
		goto p_err;
	}
	/* 0: Previous input, 1: Current input */
	sensor_peri->cis.cis_data->analog_gain[0] = sensor_peri->cis.cis_data->analog_gain[1];
	sensor_peri->cis.cis_data->analog_gain[1] = long_analog_gain;

p_err:
	return ret;
}

int fimc_is_sensor_peri_s_digital_gain(struct fimc_is_device_sensor *device,
	u32 long_digital_gain, u32 short_digital_gain)
{
	int ret = 0;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct ae_param dgain;

	BUG_ON(!device);

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		ret = -EINVAL;
		goto p_err;
	}

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	if (long_digital_gain <= 0 ) {
		err("it is wrong digital gain(%d)", long_digital_gain);
		ret = -EINVAL;
		goto p_err;
	}

#ifdef FIXED_SENSOR_DEBUG
	if (unlikely(sysfs_sensor.is_en == true)) {
		long_digital_gain = sysfs_sensor.long_digital_gain * 10;
		short_digital_gain = sysfs_sensor.short_digital_gain * 10;
		dbg_sensor(1, "dgain = %d %d\n", sysfs_sensor.long_digital_gain, sysfs_sensor.short_digital_gain);
	}
#endif

	dgain.long_val = long_digital_gain;
	dgain.short_val = short_digital_gain;
	ret = CALL_CISOPS(&sensor_peri->cis, cis_set_digital_gain, sensor_peri->subdev_cis, &dgain);
	if (ret < 0) {
		err("err!!! ret(%d)", ret);
		goto p_err;
	}
	/* 0: Previous input, 1: Current input */
	sensor_peri->cis.cis_data->digital_gain[0] = sensor_peri->cis.cis_data->digital_gain[1];
	sensor_peri->cis.cis_data->digital_gain[1] = long_digital_gain;

p_err:
	return ret;
}

int fimc_is_sensor_peri_s_wb_gains(struct fimc_is_device_sensor *device,
		struct wb_gains wb_gains)
{
	int ret = 0;
	struct v4l2_subdev *subdev_module;

	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	BUG_ON(!device->subdev_module);

	subdev_module = device->subdev_module;

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	ret = CALL_CISOPS(&sensor_peri->cis, cis_set_wb_gains, sensor_peri->subdev_cis, wb_gains);
	if (ret < 0)
		err("failed to set wb gains(%d)", ret);

p_err:
	return ret;
}

int fimc_is_sensor_peri_adj_ctrl(struct fimc_is_device_sensor *device,
		u32 input,
		struct v4l2_control *ctrl)
{
	int ret = 0;
	struct v4l2_subdev *subdev_module;

	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	BUG_ON(!device->subdev_module);
	BUG_ON(!device->subdev_csi);
	BUG_ON(!ctrl);

	subdev_module = device->subdev_module;

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	switch (ctrl->id) {
	case V4L2_CID_SENSOR_ADJUST_FRAME_DURATION:
		ret = CALL_CISOPS(&sensor_peri->cis, cis_adjust_frame_duration, sensor_peri->subdev_cis, input, &ctrl->value);
		break;
	case V4L2_CID_SENSOR_ADJUST_ANALOG_GAIN:
		ret = CALL_CISOPS(&sensor_peri->cis, cis_adjust_analog_gain, sensor_peri->subdev_cis, input, &ctrl->value);
		break;
	default:
		err("err!!! Unknown CID(%#x)", ctrl->id);
		ret = -EINVAL;
		goto p_err;
	}

	if (ret < 0) {
		err("err!!! ret(%d)", ret);
		ctrl->value = 0;
		goto p_err;
	}

p_err:
	return ret;
}

int fimc_is_sensor_peri_compensate_gain_for_ext_br(struct fimc_is_device_sensor *device,
				u32 expo, u32 *again, u32 *dgain)
{
	int ret = 0;
	struct v4l2_subdev *subdev_module;

	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!device);
	BUG_ON(!device->subdev_module);

	subdev_module = device->subdev_module;

	module = v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("module is NULL");
		ret = -EINVAL;
		goto p_err;
	}
	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;

	ret = CALL_CISOPS(&sensor_peri->cis, cis_compensate_gain_for_extremely_br, sensor_peri->subdev_cis, expo, again, dgain);
	if (ret < 0) {
		err("err!!! ret(%d)", ret);
		goto p_err;
	}

p_err:
	return ret;
}

int fimc_is_sensor_peri_debug_fixed(struct fimc_is_device_sensor *device)
{
	int ret = 0;

	if (!device) {
		err("device is null\n");
		goto p_err;
	}

	if (unlikely(sysfs_sensor.is_en == true)) {
		dbg_sensor(1, "sysfs_sensor.frame_duration = %d\n", sysfs_sensor.frame_duration);
		if (fimc_is_sensor_peri_s_frame_duration(device,
					FPS_TO_DURATION_US(sysfs_sensor.frame_duration))) {
			err("failed to set frame duration : %d\n - %d",
				sysfs_sensor.frame_duration, ret);
			goto p_err;
		}

		dbg_sensor(1, "exposure = %d %d\n",
				sysfs_sensor.long_exposure_time, sysfs_sensor.short_exposure_time);
		if (fimc_is_sensor_peri_s_exposure_time(device,
					sysfs_sensor.long_exposure_time, sysfs_sensor.short_exposure_time)) {
			err("failed to set exposure time : %d %d\n - %d",
				sysfs_sensor.long_exposure_time, sysfs_sensor.short_exposure_time, ret);
			goto p_err;
		}

		dbg_sensor(1, "again = %d %d\n", sysfs_sensor.long_analog_gain, sysfs_sensor.short_analog_gain);
		ret = fimc_is_sensor_peri_s_analog_gain(device,
				sysfs_sensor.long_analog_gain * 10, sysfs_sensor.short_analog_gain * 10);
		if (ret < 0) {
			err("failed to set analog gain : %d %d\n - %d",
					sysfs_sensor.long_analog_gain,
					sysfs_sensor.short_analog_gain, ret);
			goto p_err;
		}

		dbg_sensor(1, "dgain = %d %d\n", sysfs_sensor.long_digital_gain, sysfs_sensor.short_digital_gain);
		ret = fimc_is_sensor_peri_s_digital_gain(device,
				sysfs_sensor.long_digital_gain * 10,
				sysfs_sensor.short_digital_gain * 10);
		if (ret < 0) {
			err("failed to set digital gain : %d %d\n - %d",
				sysfs_sensor.long_digital_gain,
				sysfs_sensor.short_digital_gain, ret);
			goto p_err;
		}
	}

p_err:
	return ret;
}

void fimc_is_sensor_peri_actuator_check_landing_time(ulong data)
{
	u32 *check_time_out = (u32 *)data;

	BUG_ON(!check_time_out);

	warn("Actuator softlanding move is time overrun. Skip by force.\n");
	*check_time_out = true;
}

int fimc_is_sensor_peri_actuator_check_move_done(struct fimc_is_device_sensor_peri *device)
{
	int ret = 0;
	struct fimc_is_actuator *actuator;
	struct fimc_is_actuator_interface *actuator_itf;
	struct fimc_is_actuator_data *actuator_data;
	struct v4l2_control v4l2_ctrl;

	BUG_ON(!device);

	actuator = device->actuator;
	actuator_itf = &device->sensor_interface.actuator_itf;
	actuator_data = &actuator->actuator_data;

	v4l2_ctrl.id = V4L2_CID_ACTUATOR_GET_STATUS;
	v4l2_ctrl.value = ACTUATOR_STATUS_BUSY;
	actuator_data->check_time_out = false;

	setup_timer(&actuator_data->timer_wait,
		fimc_is_sensor_peri_actuator_check_landing_time,
		(ulong)&actuator_data->check_time_out);

	mod_timer(&actuator->actuator_data.timer_wait,
		jiffies +
		msecs_to_jiffies(actuator_itf->soft_landing_table.step_delay));
	do {
		ret = v4l2_subdev_call(device->subdev_actuator, core, g_ctrl, &v4l2_ctrl);
		if (ret) {
			err("[SEN:%d] v4l2_subdev_call(g_ctrl, id:%d) is fail",
					actuator->id, v4l2_ctrl.id);
			goto exit;
		}
	} while (v4l2_ctrl.value == ACTUATOR_STATUS_BUSY &&
			actuator_data->check_time_out == false);

exit:
	if (actuator->actuator_data.timer_wait.data) {
		del_timer(&actuator->actuator_data.timer_wait);
		actuator->actuator_data.timer_wait.data = (unsigned long)NULL;
	}

	return ret;
}

int fimc_is_sensor_peri_actuator_softlanding(struct fimc_is_device_sensor_peri *device)
{
	int ret = 0;
	int i;
	struct fimc_is_actuator *actuator;
	struct fimc_is_actuator_data *actuator_data;
	struct fimc_is_actuator_interface *actuator_itf;
	struct fimc_is_actuator_softlanding_table *soft_landing_table;
	struct v4l2_control v4l2_ctrl;

	BUG_ON(!device);

	if (!test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &device->peri_state)) {
		dbg_sensor(1, "%s: FIMC_IS_SENSOR_ACTUATOR_NOT_AVAILABLE\n", __func__);
		return ret;
	}

	actuator_itf = &device->sensor_interface.actuator_itf;
	actuator = device->actuator;
	actuator_data = &actuator->actuator_data;
	soft_landing_table = &actuator_itf->soft_landing_table;

	if (!soft_landing_table->enable) {
		soft_landing_table->position_num = 1;
		soft_landing_table->step_delay = 200;
		soft_landing_table->hw_table[0] = 0;
	}

#ifdef USE_CAMERA_ACT_DRIVER_SOFT_LANDING
	v4l2_ctrl.id = V4L2_CID_ACTUATOR_SOFT_LANDING;
	ret = v4l2_subdev_call(device->subdev_actuator, core, s_ctrl, &v4l2_ctrl);

	if(ret != HW_SOFTLANDING_FAIL){
		if(ret)
			err("[SEN:%d] v4l2_subdev_call(s_ctrl, id:%d) is fail(%d)",
				actuator->id, v4l2_ctrl.id, ret);
		return ret;
	}
#endif

	ret = fimc_is_sensor_peri_actuator_check_move_done(device);
	if (ret) {
		err("failed to get actuator position : ret(%d)\n", ret);
		return ret;
	}

	for (i = 0; i < soft_landing_table->position_num; i++) {
		if (actuator->position < soft_landing_table->hw_table[i])
			continue;

		dbg_sensor(1, "%s: cur_pos(%d) --> tgt_pos(%d)\n",
					__func__,
					actuator->position, soft_landing_table->hw_table[i]);

		v4l2_ctrl.id = V4L2_CID_ACTUATOR_SET_POSITION;
		v4l2_ctrl.value = soft_landing_table->hw_table[i];
		ret = v4l2_subdev_call(device->subdev_actuator, core, s_ctrl, &v4l2_ctrl);
		if (ret) {
			err("[SEN:%d] v4l2_subdev_call(s_ctrl, id:%d) is fail(%d)",
					actuator->id, v4l2_ctrl.id, ret);
			return ret;
		}

		actuator_itf->virtual_pos = soft_landing_table->virtual_table[i];
		actuator_itf->hw_pos = soft_landing_table->hw_table[i];

		/* The actuator needs a delay time when lens moving for soft landing. */
		msleep(soft_landing_table->step_delay);

		ret = fimc_is_sensor_peri_actuator_check_move_done(device);
		if (ret) {
			err("failed to get actuator position : ret(%d)\n", ret);
			return ret;
		}
	}

	return ret;
}

/* M2M AF position setting */
int fimc_is_sensor_peri_call_m2m_actuator(struct fimc_is_device_sensor *device)
{
	int ret = 0;
	int index;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct v4l2_subdev *subdev_module;

	BUG_ON(!device);

	subdev_module = device->subdev_module;
	if (!subdev_module) {
		err("subdev_module is NULL");
		return -EINVAL;
	}

	module = (struct fimc_is_module_enum *)v4l2_get_subdevdata(subdev_module);
	if (!module) {
		err("subdev_module is NULL");
		return -EINVAL;
	}

	sensor_peri = (struct fimc_is_device_sensor_peri *)module->private_data;
	if (!sensor_peri) {
		err("sensor_peri is NULL");
		return -EINVAL;
	}

	index = sensor_peri->actuator->actuator_index;

	if (index >= 0) {
		dbg_sensor(1, "%s: M2M actuator set schedule\n", __func__);
		schedule_work(&sensor_peri->actuator->actuator_data.actuator_work);
	}
	else {
		/* request_count zero is not request set position in FW */
		dbg_sensor(1, "actuator request position is Zero\n");
		sensor_peri->actuator->actuator_index = -1;

		return ret;
	}

	return ret;
}

int fimc_is_sensor_initial_preprocessor_setting(struct fimc_is_device_sensor_peri *sensor_peri)
{
	int ret = 0;
	cis_shared_data *cis_data = NULL;
#ifdef CONFIG_COMPANION_USE
	SENCMD_CompanionSetWdrAeInfoStr wdrParam;
	preprocessor_ae_setting *preprocessor_ae_setting_cur;
#endif

	BUG_ON(!sensor_peri);

	cis_data = sensor_peri->cis.cis_data;
	BUG_ON(!cis_data);

#ifdef CONFIG_COMPANION_USE
	preprocessor_ae_setting_cur = &cis_data->preproc_auto_exposure[CURRENT_FRAME];

	/* update cis_data about preproc */
	wdrParam.uiShortExposureCoarse = preprocessor_ae_setting_cur->short_exposure_coarse;
	wdrParam.uiShortExposureFine = 0x100;
	wdrParam.uiShortExposureAnalogGain = preprocessor_ae_setting_cur->short_exposure_analog_gain;
	wdrParam.uiShortExposureDigitalGain = preprocessor_ae_setting_cur->short_exposure_digital_gain;
	wdrParam.uiLongExposureCoarse = preprocessor_ae_setting_cur->long_exposure_coarse;
	wdrParam.uiLongExposureFine = 0x100;
	wdrParam.uiLongExposureAnalogGain = preprocessor_ae_setting_cur->long_exposure_analog_gain;
	wdrParam.uiLongExposureDigitalGain = preprocessor_ae_setting_cur->long_exposure_digital_gain;
	wdrParam.bHwSetEveryFrame = true;

	if (sensor_peri->subdev_preprocessor) {
		ret = CALL_PREPROPOPS(sensor_peri->preprocessor, preprocessor_wdr_ae, sensor_peri->subdev_preprocessor, &wdrParam);

		if (ret) {
			err("preprocessor_wdr_ae fail");
		}
	}
#endif
	return ret;
}
