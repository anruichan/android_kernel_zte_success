/*
 * Copyright (c) 2009-2018, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _ARCH_ARM_MACH_MSM_SOCINFO_H_
#define _ARCH_ARM_MACH_MSM_SOCINFO_H_

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/of_fdt.h>
#include <linux/of.h>

#include <asm/cputype.h>
#ifdef CONFIG_BOARD_ZTE
/* add by ZTE_BOOT */
#ifdef CONFIG_ZTE_BOOT_MODE

#define ANDROID_BOOT_MODE              "androidboot.mode="
#define ANDROID_BOOT_MODE_FTM          "ffbm-99"
#define ANDROID_BOOT_MODE_FFBM         "ffbm-00"
#define ANDROID_BOOT_MODE_RECOVERY	"recovery"
#define ANDROID_BOOT_MODE_CHARGER      "charger"

#define MAGIC_NUM_FFBM_MODE          0x6D6D5446 /* FFBM */
#define MAGIC_NUM_NON_FFBM_MODE      0x4D54464E /* NFFBM */

/*
 * Boot mode definition
 */
enum {
	ENUM_BOOT_MODE_NORMAL            = 0,
	ENUM_BOOT_MODE_FTM               = 1,
	ENUM_BOOT_MODE_RTC_ALARM         = 2,
	ENUM_BOOT_MODE_CHARGER           = 3,
	ENUM_BOOT_MODE_RECOVERY          = 4,
	ENUM_BOOT_MODE_FFBM              = 5,
	ENUM_BOOT_MODE_UNKNOWN,
	ENUM_BOOT_MODE_MAX
};

void socinfo_set_boot_mode(int boot_mode);
int socinfo_get_ftm_flag(void);
int socinfo_get_ffbm_flag(void);
int socinfo_get_charger_flag(void);
#endif
#endif
/*
 * SOC version type with major number in the upper 16 bits and minor
 * number in the lower 16 bits.  For example:
 *   1.0 -> 0x00010000
 *   2.3 -> 0x00020003
 */
#define SOCINFO_VERSION_MAJOR(ver) (((ver) & 0xffff0000) >> 16)
#define SOCINFO_VERSION_MINOR(ver) ((ver) & 0x0000ffff)
#define SOCINFO_VERSION(maj, min)  ((((maj) & 0xffff) << 16)|((min) & 0xffff))

#ifdef CONFIG_OF
#define of_board_is_cdp()	of_machine_is_compatible("qcom,cdp")
#define of_board_is_sim()	of_machine_is_compatible("qcom,sim")
#define of_board_is_rumi()	of_machine_is_compatible("qcom,rumi")
#define of_board_is_fluid()	of_machine_is_compatible("qcom,fluid")
#define of_board_is_liquid()	of_machine_is_compatible("qcom,liquid")
#define of_board_is_dragonboard()	\
	of_machine_is_compatible("qcom,dragonboard")
#define of_board_is_cdp()	of_machine_is_compatible("qcom,cdp")
#define of_board_is_mtp()	of_machine_is_compatible("qcom,mtp")
#define of_board_is_qrd()	of_machine_is_compatible("qcom,qrd")
#define of_board_is_xpm()	of_machine_is_compatible("qcom,xpm")
#define of_board_is_skuf()	of_machine_is_compatible("qcom,skuf")
#define of_board_is_sbc()	of_machine_is_compatible("qcom,sbc")

#define machine_is_msm8974()	of_machine_is_compatible("qcom,msm8974")
#define machine_is_msm9625()	of_machine_is_compatible("qcom,msm9625")
#define machine_is_msm8610()	of_machine_is_compatible("qcom,msm8610")
#define machine_is_msm8226()	of_machine_is_compatible("qcom,msm8226")
#define machine_is_apq8074()	of_machine_is_compatible("qcom,apq8074")
#define machine_is_msm8926()	of_machine_is_compatible("qcom,msm8926")

#define early_machine_is_msm8610()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8610")
#define early_machine_is_msm8909()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8909")
#define early_machine_is_msm8916()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8916")
#define early_machine_is_msm8917()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8917")
#define early_machine_is_msm8920()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8920")
#define early_machine_is_msm8940()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8940")
#define early_machine_is_msm8936()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8936")
#define early_machine_is_msm8939()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8939")
#define early_machine_is_apq8084()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,apq8084")
#define early_machine_is_mdm9630()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,mdm9630")
#define early_machine_is_mdm9640()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,mdm9640")
#define early_machine_is_mdm9650()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,mdm9650")
#define early_machine_is_sdx20()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,sdx20")
#define early_machine_is_fsm9900()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,fsm9900")
#define early_machine_is_msm8994()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8994")
#define early_machine_is_msm8992()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8992")
#define early_machine_is_fsm9010()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,fsm9010")
#define early_machine_is_msm8976()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8976")
#define early_machine_is_msm8952()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8952")
#define early_machine_is_msm8953()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8953")
#define early_machine_is_sdm450()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,sdm450")
#define early_machine_is_msm8937()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8937")
#define early_machine_is_msm8996()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8996")
#define early_machine_is_msm8929()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msm8929")
#define early_machine_is_mdm9607()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,mdm9607")
#define early_machine_is_msmcobalt()	\
	of_flat_dt_is_compatible(of_get_flat_dt_root(), "qcom,msmcobalt")
#else
#define of_board_is_sim()		0
#define of_board_is_rumi()		0
#define of_board_is_fluid()		0
#define of_board_is_liquid()		0
#define of_board_is_dragonboard()	0
#define of_board_is_cdp()		0
#define of_board_is_mtp()		0
#define of_board_is_qrd()		0
#define of_board_is_xpm()		0
#define of_board_is_skuf()		0
#define of_board_is_sbc()		0

#define machine_is_msm8974()		0
#define machine_is_msm9625()		0
#define machine_is_msm8610()		0
#define machine_is_msm8226()		0
#define machine_is_apq8074()		0
#define machine_is_msm8926()		0

#define early_machine_is_msm8610()	0
#define early_machine_is_msm8909()	0
#define early_machine_is_msm8916()	0
#define early_machine_is_msm8917()	0
#define early_machine_is_msm8920()	0
#define early_machine_is_msm8940()	0
#define early_machine_is_msm8936()	0
#define early_machine_is_msm8939()	0
#define early_machine_is_apq8084()	0
#define early_machine_is_mdm9630()	0
#define early_machine_is_fsm9900()	0
#define early_machine_is_fsm9010()	0
#define early_machine_is_msm8952()	0
#define early_machine_is_msm8953()	0
#define early_machine_is_sdm450()	0
#define early_machine_is_msm8937()	0
#define early_machine_is_msm8996()	0
#define early_machine_is_msm8976() 0
#define early_machine_is_msm8929()	0
#define early_machine_is_mdm9607()	0
#define early_machine_is_mdm9650()	0
#define early_machine_is_sdx20()	0
#define early_machine_is_msmcobalt()	0
#endif

#define PLATFORM_SUBTYPE_MDM	1
#define PLATFORM_SUBTYPE_INTERPOSERV3 2
#define PLATFORM_SUBTYPE_SGLTE	6

enum msm_cpu {
	MSM_CPU_UNKNOWN = 0,
	MSM_CPU_7X01,
	MSM_CPU_7X25,
	MSM_CPU_7X27,
	MSM_CPU_8X50,
	MSM_CPU_8X50A,
	MSM_CPU_7X30,
	MSM_CPU_8X55,
	MSM_CPU_8X60,
	MSM_CPU_8960,
	MSM_CPU_8960AB,
	MSM_CPU_7X27A,
	FSM_CPU_9XXX,
	MSM_CPU_7X25A,
	MSM_CPU_7X25AA,
	MSM_CPU_7X25AB,
	MSM_CPU_8064,
	MSM_CPU_8064AB,
	MSM_CPU_8064AA,
	MSM_CPU_8930,
	MSM_CPU_8930AA,
	MSM_CPU_8930AB,
	MSM_CPU_7X27AA,
	MSM_CPU_9615,
	MSM_CPU_8974,
	MSM_CPU_8974PRO_AA,
	MSM_CPU_8974PRO_AB,
	MSM_CPU_8974PRO_AC,
	MSM_CPU_8627,
	MSM_CPU_8625,
	MSM_CPU_9625,
	MSM_CPU_8909,
	MSM_CPU_8916,
	MSM_CPU_8917,
	MSM_CPU_8920,
	MSM_CPU_8940,
	MSM_CPU_8936,
	MSM_CPU_8939,
	MSM_CPU_8226,
	MSM_CPU_8610,
	MSM_CPU_8625Q,
	MSM_CPU_8084,
	MSM_CPU_9630,
	FSM_CPU_9900,
	MSM_CPU_ZIRC,
	MSM_CPU_8994,
	MSM_CPU_8992,
	FSM_CPU_9010,
	MSM_CPU_8952,
	MSM_CPU_8953,
	MSM_CPU_SDM450,
	MSM_CPU_8937,
	MSM_CPU_8996,
	MSM_CPU_8976,
	MSM_CPU_8929,
	MSM_CPU_9607,
	MSM_CPU_9650,
	MSM_CPU_COBALT,
	SDX_CPU_20,
	MSM_CPU_9150,
};

struct msm_soc_info {
	enum msm_cpu generic_soc_type;
	char *soc_id_string;
};

enum pmic_model {
	PMIC_MODEL_PM8058	= 13,
	PMIC_MODEL_PM8028	= 14,
	PMIC_MODEL_PM8901	= 15,
	PMIC_MODEL_PM8027	= 16,
	PMIC_MODEL_ISL_9519	= 17,
	PMIC_MODEL_PM8921	= 18,
	PMIC_MODEL_PM8018	= 19,
	PMIC_MODEL_PM8015	= 20,
	PMIC_MODEL_PM8014	= 21,
	PMIC_MODEL_PM8821	= 22,
	PMIC_MODEL_PM8038	= 23,
	PMIC_MODEL_PM8922	= 24,
	PMIC_MODEL_PM8917	= 25,
	PMIC_MODEL_UNKNOWN	= 0xFFFFFFFF
};

enum msm_cpu socinfo_get_msm_cpu(void);
uint32_t socinfo_get_id(void);
uint32_t socinfo_get_version(void);
uint32_t socinfo_get_raw_id(void);
char *socinfo_get_build_id(void);
uint32_t socinfo_get_platform_type(void);
uint32_t socinfo_get_platform_subtype(void);
uint32_t socinfo_get_platform_version(void);
uint32_t socinfo_get_serial_number(void);
enum pmic_model socinfo_get_pmic_model(void);
uint32_t socinfo_get_pmic_die_revision(void);
int __init socinfo_init(void) __must_check;
#ifdef CONFIG_BOARD_ZTE
int zte_get_boot_mode(void);
#endif

#endif
