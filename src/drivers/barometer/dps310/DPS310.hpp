/****************************************************************************
 *
 *   Copyright (c) 2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file dps310.cpp
 *
 * Driver for the DPS310 barometer connected via I2C or SPI.
 */

#pragma once

#include <drivers/device/Device.hpp>
#include <lib/drivers/barometer/PX4Barometer.hpp>
#include <lib/perf/perf_counter.h>
#include <px4_work_queue/ScheduledWorkItem.hpp>

#include "Infineon_DPS310_Registers.hpp"

using Infineon_DPS310::Register;

/*
 * DPS310 internal constants and data structures.
 */

class DPS310 : public px4::ScheduledWorkItem
{
public:
	DPS310(device::Device *interface);
	virtual ~DPS310();

	virtual int		init();

	void			print_info();

private:

	void			Run() override;

	void			start();
	void			stop();
	int			reset();

	uint8_t			RegisterRead(Register reg);
	void			RegisterWrite(Register reg, uint8_t val);
	void			RegisterClearBits(Register reg, uint8_t clearbits);
	void			RegisterSetBits(Register reg, uint8_t setbits);

	void			calculate_PT(int32_t UT, int32_t UP, float &pressure, float &temperature);


	PX4Barometer		_px4_barometer;

	device::Device		*_interface;

	Infineon_DPS310::Calibration	_calibration{};

	unsigned		_measure_interval{0};

	bool			_collect_phase{false};

	perf_counter_t		_sample_perf;
	perf_counter_t		_sample_interval_perf;
	perf_counter_t		_comms_errors;

};