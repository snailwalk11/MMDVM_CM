/*
 *   Copyright (C) 2010,2014,2016,2018 by Jonathan Naylor G4KLX
 *   Copyright (C) 2018 by Andy Uribe CA6JAU
 *   Copyright (C) 2021 by Doug McLain AD8DP
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "RingBuffer.h"
#include "codec2/codec2.h"

const uint8_t TAG_HEADER = 0x00U;
const uint8_t TAG_DATA   = 0x01U;
const uint8_t TAG_LOST   = 0x02U;
const uint8_t TAG_EOT    = 0x03U;
const uint8_t TAG_NODATA = 0x04U;

const int16_t TAG_USRP_HEADER = 0x0000U;
const int16_t TAG_USRP_DATA   = 0x0001U;
const int16_t TAG_USRP_LOST   = 0x0002U;
const int16_t TAG_USRP_EOT    = 0x0003U;
const int16_t TAG_USRP_NODATA = 0x0004U;

const uint8_t USRP_TYPE_VOICE = 0;
const uint8_t USRP_TYPE_TEXT  = 2;

const uint8_t USRP_KEYUP_FALSE  = 0;
const uint8_t USRP_KEYUP_TRUE   = 1;

const uint8_t TLV_TAG_SET_INFO = 8;

#if !defined(MODECONV_H)
#define MODECONV_H

class CModeConv {
public:
	CModeConv();
	~CModeConv();

	void setUSRPGainAdjDb(std::string dbstring);
	void setM17GainAdjDb(std::string dbstring);
	void putUSRP(int16_t* data);
	void putUSRPHeader();
	void putUSRPEOT();
	void putM17(uint8_t* data);
	void putM17Header();
	void putM17EOT();
	uint32_t getM17(uint8_t* data);
	uint32_t getUSRP(int16_t* data);
private:
	uint32_t m_m17N;
	uint32_t m_usrpN;
	CRingBuffer<uint8_t> m_M17;
	CRingBuffer<int16_t> m_USRP;
	CCodec2 *m_c2;
	uint16_t m_m17GainMultiplier;
	bool m_m17Attenuate;
	uint16_t m_usrpGainMultiplier;
	bool m_usrpAttenuate;
};

#endif
