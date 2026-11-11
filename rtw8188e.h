/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright(c) 2007-2017 Realtek Corporation
 * Copyright(c) 2018-2019 Realtek Corporation
 * Copyright 2024 Fiona Klute
 * Copyright(c) 2026 Igor Listopad <dev@listopad.email>
 */

#ifndef __RTW8188E_H__
#define __RTW8188E_H__

extern const struct rtw_chip_info rtw8188e_hw_spec;

#define RTW8188E_CHIP_VER_CUT_I		8

enum rtw8188e_flags {
	RTW8188E_FLAG_BEACON_UPDATE,
};

struct rtw8188e_priv {
	unsigned long flags;
	s8 l2h_th_min;
	s8 h2l_th_min;
	u8 adaptivity_igi_upper;
};

struct rtw8188e_efuse {
	u8 res0[4];
	u8 afe;
	u8 res1[11];
	struct rtw_2g_txpwr_idx txpwr_idx;	/* 0x10 */
	u8 res2[0xb8 - 0x10 - sizeof(struct rtw_2g_txpwr_idx)];
	u8 channel_plan;			/* 0xb8 */
	u8 xtal_k;
	u8 thermal_meter;
	u8 res3[6];
	u8 rf_board_option;		/* 0xc1 */
	u8 res4[8];
	u8 rfe_option;			/* 0xca */
	u8 country_code[2];
	u8 res5[0x4d];
	u8 mac_addr[ETH_ALEN];		/* 0x11a */
	u8 res6[0xe0];
} __packed;

static_assert(sizeof(struct rtw8188e_efuse) == 512);

struct phy_status_8188e {
	u8 path_agc[2];
	u8 res0[2];
	u8 cck_sig_qual_ofdm_pwdb_all;
	u8 cck_agc_rpt_ofdm_cfosho_a;
	u8 res1[3];
	s8 path_cfotail[2];
	u8 res2[2];
	s8 stream_rxevm[2];
	u8 path_rxsnr[2];
	u8 res3[11];
} __packed;

#define RTW8188E_RA_RATE_COUNT		(DESC_RATEMCS7 + 1)
#define RTW8188E_RA_RETRY_COUNT		5
#define RTW8188E_RA_RPT_TIME_MAX		0x927c

struct rtw8188e_ra_info {
	u32 rate_mask;
	u32 nsc_up;
	u32 nsc_down;
	u32 total;
	u16 retry[RTW8188E_RA_RETRY_COUNT];
	u16 rpt_time;
	u8 drop;
	u8 decision_rate;
	u8 pre_rate;
	u8 highest_rate;
	u8 lowest_rate;
	u8 rssi;
	u8 pre_rssi;
	u8 bw_mode;
	u8 waiting;
	u8 pending;
	u8 drop_after_down;
	u8 rpt_stable;
	u8 stage;
	u8 pt_stage;
	u8 pt_smooth_factor;
	u8 pt_stop_count;
	u8 pt_pre_rate;
	u8 pt_pre_rssi;
	bool sgi_enable;
	bool rate_sgi;
	bool active;
};

struct rtw8188e_sta {
	/* Serializes rate updates with TX preparation and RX reports. */
	spinlock_t ra_lock;
	struct rtw8188e_ra_info ra;
	bool rsvd_pending;
};

enum rtw8188e_rate_id {
	RTW8188E_RATEID_NGB,
	RTW8188E_RATEID_NG,
	RTW8188E_RATEID_NB,
	RTW8188E_RATEID_N,
	RTW8188E_RATEID_GB,
	RTW8188E_RATEID_G,
	RTW8188E_RATEID_B,
};

#define IQK_COEFF_UNITY			0x100

#define XCAP_MASK			GENMASK(5, 0)
#define CFO_TRK_ENABLE_TH		20
#define CFO_TRK_STOP_TH			10
#define CFO_TH_ATC			80

#define EDCCA_DC_BACKOFF			2

enum rtw8188e_iqk_result {
	IQK_TX_X,
	IQK_TX_Y,
	IQK_RX_X,
	IQK_RX_Y,
	RTW8188E_IQK_NR,
};

#define RTW8188E_IQK_ADDA_REG_NUM		16
#define RTW8188E_IQK_MAC8_REG_NUM		3
#define RTW8188E_IQK_MAC32_REG_NUM		1
#define RTW8188E_IQK_BB_REG_NUM		9

struct rtw8188e_iqk_backup_regs {
	u32 adda[RTW8188E_IQK_ADDA_REG_NUM];
	u8 mac8[RTW8188E_IQK_MAC8_REG_NUM];
	u32 mac32[RTW8188E_IQK_MAC32_REG_NUM];
	u32 bb[RTW8188E_IQK_BB_REG_NUM];
};

#define RTW8188E_RX_DMA_RESERVED_SIZE	0x200

#define RTW8188E_EFUSE_INT_PA		BIT(3)
#define RTW8188E_EFUSE_INT_LNA		BIT(2)
#define RTW8188E_EFUSE_GLNA_TYPE		GENMASK(6, 4)

#define REG_AGGLEN_LMT			0x0458
#define REG_MACID_NO_LINK_0		0x0484
#define REG_MACID_NO_LINK_1		0x0488
#define REG_TSFTR_SYN_OFFSET		0x0518
#define REG_MBID_NUM			0x0552
#define REG_DUAL_TSF_RST			0x0553
#define REG_MBSSID_BCN_SPACE		0x0554
#define REG_TSFTR			0x0560
#define REG_TSFTR1			0x0568
#define BIT_DIS_ATIM			BIT(0)
#define BIT_DIS_BCNQ_SUB			BIT(1)
#define REG_BWOPMODE			0x0603
#define REG_FPGA0_XA_HSSI_PARM1		0x0820
#define REG_FPGA0_XB_HSSI_PARM1		0x0828
#define REG_DBGPORT_SEL_11N		0x0908
#define REG_CONFIG_ANT_A_11N		0x0b68
#define BIT_EN_ATC			BIT(11)
#define REG_DBGPORT_RPT_11N		0x0df4
#define BIT_MASK_IQK_MODE		GENMASK(31, 8)
#define BIT_MASK_IQK_RESULT		GENMASK(25, 16)

#define RTW8188E_TX_DESC_W1_MACID			GENMASK(5, 0)
#define RTW8188E_TX_DESC_W1_RATE_ID		GENMASK(19, 16)
#define RTW8188E_TX_DESC_W1_NAVUSEHDR		BIT(20)
#define RTW8188E_TX_DESC_W1_PKT_OFFSET		GENMASK(30, 26)
#define RTW8188E_TX_DESC_W2_AGG_BREAK		BIT(16)
#define RTW8188E_TX_DESC_W3_SEQ			GENMASK(27, 16)
#define RTW8188E_TX_DESC_W3_EN_HWSEQ		BIT(31)
#define RTW8188E_TX_DESC_W4_RTS_RATE		GENMASK(4, 0)
#define RTW8188E_RSVD_HW_SSN_SEL			2
#define RTW8188E_TX_DESC_W4_HW_SSN_SEL		GENMASK(7, 6)
#define RTW8188E_TX_DESC_W4_USE_RATE		BIT(8)
#define RTW8188E_TX_DESC_W4_DISDATAFB		BIT(10)
#define RTW8188E_TX_DESC_W4_RTS_EN			BIT(12)
#define RTW8188E_TX_DESC_W4_HW_RTS_EN		BIT(13)
#define RTW8188E_TX_DESC_W4_PORT_ID		BIT(14)
#define RTW8188E_TX_DESC_W4_PWR_STATUS		GENMASK(17, 15)
#define RTW8188E_TX_DESC_W4_DATA_SC		GENMASK(21, 20)
#define RTW8188E_TX_DESC_W4_DATA_BW		BIT(25)
#define RTW8188E_TX_DESC_W4_RTS_BW			BIT(27)
#define RTW8188E_TX_DESC_W4_RTS_SC			GENMASK(29, 28)
#define RTW8188E_TX_DESC_W5_DATARATE		GENMASK(5, 0)
#define RTW8188E_TX_DESC_W5_SHORT_GI		BIT(6)
#define RTW8188E_TX_DESC_W5_DATA_FB_LIMIT		GENMASK(12, 8)
#define RTW8188E_TX_DESC_W5_RTS_FB_LIMIT		GENMASK(16, 13)
#define RTW8188E_TX_DESC_W6_MAX_AGG_NUM		GENMASK(15, 11)
#define RTW8188E_TX_DESC_W7_SW_DEFINE		GENMASK(27, 16)

enum rtw8188e_primary_channel {
	RTW8188E_SC_DONT_CARE,
	RTW8188E_SC_LOWER,
	RTW8188E_SC_UPPER,
};

#define RTW8188E_RX_DESC_W1_MACID			GENMASK(4, 0)
#define RTW8188E_RX_DESC_W3_RX_RATE		GENMASK(5, 0)
#define RTW8188E_RX_DESC_W3_BW			BIT(9)
#define RTW8188E_RX_DESC_W3_RPT_SEL		GENMASK(15, 14)

enum rtw8188e_rx_report_type {
	RTW8188E_RX_NORMAL,
	RTW8188E_RX_TX_REPORT1,
	RTW8188E_RX_TX_REPORT2,
};

struct rtw8188e_tx_report {
	__le32 w0;
	__le32 w1;
} __packed;

struct rtw8188e_tx_report2 {
	__le16 retry0;
	u8 retry[RTW8188E_RA_RETRY_COUNT - 1];
	u8 drop;
	u8 res;
} __packed;

static_assert(sizeof(struct rtw8188e_tx_report2) == 8);

#define RTW8188E_TX_RPT_CTRL_REPORT_EN		BIT(0)
#define RTW8188E_TX_RPT_CTRL_TIMER_EN		BIT(1)

#define RTW8188E_TX_REPORT_W0_INT_CCX		BIT(7)
#define RTW8188E_TX_REPORT_W0_PKT_OK		BIT(14)
#define RTW8188E_TX_REPORT_W1_SW_HIGH		GENMASK(19, 16)
#define RTW8188E_TX_REPORT_W1_SW_LOW		GENMASK(31, 24)

#endif
