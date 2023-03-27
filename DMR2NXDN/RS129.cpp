/*
 *   Copyright (C) 2015 by Jonathan Naylor G4KLX
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

#include "RS129.h"

#include <cstdio>
#include <cassert>
#include <cstring>

const unsigned int NPAR = 3U;

/* Generator Polynomial */
const unsigned char POLY[] = {64U, 56U, 14U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};

const unsigned char EXP_TABLE[] = {
	0x01U, 0x02U, 0x04U, 0x08U, 0x10U, 0x20U, 0x40U, 0x80U, 0x1DU, 0x3AU, 0x74U, 0xE8U, 0xCDU, 0x87U, 0x13U, 0x26U,
	0x4CU, 0x98U, 0x2DU, 0x5AU, 0xB4U, 0x75U, 0xEAU, 0xC9U, 0x8FU, 0x03U, 0x06U, 0x0CU, 0x18U, 0x30U, 0x60U, 0xC0U,
	0x9DU, 0x27U, 0x4EU, 0x9CU, 0x25U, 0x4AU, 0x94U, 0x35U, 0x6AU, 0xD4U, 0xB5U, 0x77U, 0xEEU, 0xC1U, 0x9FU, 0x23U,
	0x46U, 0x8CU, 0x05U, 0x0AU, 0x14U, 0x28U, 0x50U, 0xA0U, 0x5DU, 0xBAU, 0x69U, 0xD2U, 0xB9U, 0x6FU, 0xDEU, 0xA1U,
	0x5FU, 0xBEU, 0x61U, 0xC2U, 0x99U, 0x2FU, 0x5EU, 0xBCU, 0x65U, 0xCAU, 0x89U, 0x0FU, 0x1EU, 0x3CU, 0x78U, 0xF0U,
	0xFDU, 0xE7U, 0xD3U, 0xBBU, 0x6BU, 0xD6U, 0xB1U, 0x7FU, 0xFEU, 0xE1U, 0xDFU, 0xA3U, 0x5BU, 0xB6U, 0x71U, 0xE2U,
	0xD9U, 0xAFU, 0x43U, 0x86U, 0x11U, 0x22U, 0x44U, 0x88U, 0x0DU, 0x1AU, 0x34U, 0x68U, 0xD0U, 0xBDU, 0x67U, 0xCEU,
	0x81U, 0x1FU, 0x3EU, 0x7CU, 0xF8U, 0xEDU, 0xC7U, 0x93U, 0x3BU, 0x76U, 0xECU, 0xC5U, 0x97U, 0x33U, 0x66U, 0xCCU,
	0x85U, 0x17U, 0x2EU, 0x5CU, 0xB8U, 0x6DU, 0xDAU, 0xA9U, 0x4FU, 0x9EU, 0x21U, 0x42U, 0x84U, 0x15U, 0x2AU, 0x54U,
	0xA8U, 0x4DU, 0x9AU, 0x29U, 0x52U, 0xA4U, 0x55U, 0xAAU, 0x49U, 0x92U, 0x39U, 0x72U, 0xE4U, 0xD5U, 0xB7U, 0x73U,
	0xE6U, 0xD1U, 0xBFU, 0x63U, 0xC6U, 0x91U, 0x3FU, 0x7EU, 0xFCU, 0xE5U, 0xD7U, 0xB3U, 0x7BU, 0xF6U, 0xF1U, 0xFFU,
	0xE3U, 0xDBU, 0xABU, 0x4BU, 0x96U, 0x31U, 0x62U, 0xC4U, 0x95U, 0x37U, 0x6EU, 0xDCU, 0xA5U, 0x57U, 0xAEU, 0x41U,
	0x82U, 0x19U, 0x32U, 0x64U, 0xC8U, 0x8DU, 0x07U, 0x0EU, 0x1CU, 0x38U, 0x70U, 0xE0U, 0xDDU, 0xA7U, 0x53U, 0xA6U,
	0x51U, 0xA2U, 0x59U, 0xB2U, 0x79U, 0xF2U, 0xF9U, 0xEFU, 0xC3U, 0x9BU, 0x2BU, 0x56U, 0xACU, 0x45U, 0x8AU, 0x09U,
	0x12U, 0x24U, 0x48U, 0x90U, 0x3DU, 0x7AU, 0xF4U, 0xF5U, 0xF7U, 0xF3U, 0xFBU, 0xEBU, 0xCBU, 0x8BU, 0x0BU, 0x16U,
	0x2CU, 0x58U, 0xB0U, 0x7DU, 0xFAU, 0xE9U, 0xCFU, 0x83U, 0x1BU, 0x36U, 0x6CU, 0xD8U, 0xADU, 0x47U, 0x8EU, 0x01U,
	0x02U, 0x04U, 0x08U, 0x10U, 0x20U, 0x40U, 0x80U, 0x1DU, 0x3AU, 0x74U, 0xE8U, 0xCDU, 0x87U, 0x13U, 0x26U, 0x4CU,
	0x98U, 0x2DU, 0x5AU, 0xB4U, 0x75U, 0xEAU, 0xC9U, 0x8FU, 0x03U, 0x06U, 0x0CU, 0x18U, 0x30U, 0x60U, 0xC0U, 0x9DU,
	0x27U, 0x4EU, 0x9CU, 0x25U, 0x4AU, 0x94U, 0x35U, 0x6AU, 0xD4U, 0xB5U, 0x77U, 0xEEU, 0xC1U, 0x9FU, 0x23U, 0x46U,
	0x8CU, 0x05U, 0x0AU, 0x14U, 0x28U, 0x50U, 0xA0U, 0x5DU, 0xBAU, 0x69U, 0xD2U, 0xB9U, 0x6FU, 0xDEU, 0xA1U, 0x5FU,
	0xBEU, 0x61U, 0xC2U, 0x99U, 0x2FU, 0x5EU, 0xBCU, 0x65U, 0xCAU, 0x89U, 0x0FU, 0x1EU, 0x3CU, 0x78U, 0xF0U, 0xFDU,
	0xE7U, 0xD3U, 0xBBU, 0x6BU, 0xD6U, 0xB1U, 0x7FU, 0xFEU, 0xE1U, 0xDFU, 0xA3U, 0x5BU, 0xB6U, 0x71U, 0xE2U, 0xD9U,
	0xAFU, 0x43U, 0x86U, 0x11U, 0x22U, 0x44U, 0x88U, 0x0DU, 0x1AU, 0x34U, 0x68U, 0xD0U, 0xBDU, 0x67U, 0xCEU, 0x81U,
	0x1FU, 0x3EU, 0x7CU, 0xF8U, 0xEDU, 0xC7U, 0x93U, 0x3BU, 0x76U, 0xECU, 0xC5U, 0x97U, 0x33U, 0x66U, 0xCCU, 0x85U,
	0x17U, 0x2EU, 0x5CU, 0xB8U, 0x6DU, 0xDAU, 0xA9U, 0x4FU, 0x9EU, 0x21U, 0x42U, 0x84U, 0x15U, 0x2AU, 0x54U, 0xA8U,
	0x4DU, 0x9AU, 0x29U, 0x52U, 0xA4U, 0x55U, 0xAAU, 0x49U, 0x92U, 0x39U, 0x72U, 0xE4U, 0xD5U, 0xB7U, 0x73U, 0xE6U,
	0xD1U, 0xBFU, 0x63U, 0xC6U, 0x91U, 0x3FU, 0x7EU, 0xFCU, 0xE5U, 0xD7U, 0xB3U, 0x7BU, 0xF6U, 0xF1U, 0xFFU, 0xE3U,
	0xDBU, 0xABU, 0x4BU, 0x96U, 0x31U, 0x62U, 0xC4U, 0x95U, 0x37U, 0x6EU, 0xDCU, 0xA5U, 0x57U, 0xAEU, 0x41U, 0x82U,
	0x19U, 0x32U, 0x64U, 0xC8U, 0x8DU, 0x07U, 0x0EU, 0x1CU, 0x38U, 0x70U, 0xE0U, 0xDDU, 0xA7U, 0x53U, 0xA6U, 0x51U,
	0xA2U, 0x59U, 0xB2U, 0x79U, 0xF2U, 0xF9U, 0xEFU, 0xC3U, 0x9BU, 0x2BU, 0x56U, 0xACU, 0x45U, 0x8AU, 0x09U, 0x12U,
	0x24U, 0x48U, 0x90U, 0x3DU, 0x7AU, 0xF4U, 0xF5U, 0xF7U, 0xF3U, 0xFBU, 0xEBU, 0xCBU, 0x8BU, 0x0BU, 0x16U, 0x2CU,
	0x58U, 0xB0U, 0x7DU, 0xFAU, 0xE9U, 0xCFU, 0x83U, 0x1BU, 0x36U, 0x6CU, 0xD8U, 0xADU, 0x47U, 0x8EU, 0x01U, 0x00U};

const unsigned char LOG_TABLE[] = {
	0x00U, 0x00U, 0x01U, 0x19U, 0x02U, 0x32U, 0x1AU, 0xC6U, 0x03U, 0xDFU, 0x33U, 0xEEU, 0x1BU, 0x68U, 0xC7U, 0x4BU,
	0x04U, 0x64U, 0xE0U, 0x0EU, 0x34U, 0x8DU, 0xEFU, 0x81U, 0x1CU, 0xC1U, 0x69U, 0xF8U, 0xC8U, 0x08U, 0x4CU, 0x71U,
	0x05U, 0x8AU, 0x65U, 0x2FU, 0xE1U, 0x24U, 0x0FU, 0x21U, 0x35U, 0x93U, 0x8EU, 0xDAU, 0xF0U, 0x12U, 0x82U, 0x45U,
	0x1DU, 0xB5U, 0xC2U, 0x7DU, 0x6AU, 0x27U, 0xF9U, 0xB9U, 0xC9U, 0x9AU, 0x09U, 0x78U, 0x4DU, 0xE4U, 0x72U, 0xA6U,
	0x06U, 0xBFU, 0x8BU, 0x62U, 0x66U, 0xDDU, 0x30U, 0xFDU, 0xE2U, 0x98U, 0x25U, 0xB3U, 0x10U, 0x91U, 0x22U, 0x88U,
	0x36U, 0xD0U, 0x94U, 0xCEU, 0x8FU, 0x96U, 0xDBU, 0xBDU, 0xF1U, 0xD2U, 0x13U, 0x5CU, 0x83U, 0x38U, 0x46U, 0x40U,
	0x1EU, 0x42U, 0xB6U, 0xA3U, 0xC3U, 0x48U, 0x7EU, 0x6EU, 0x6BU, 0x3AU, 0x28U, 0x54U, 0xFAU, 0x85U, 0xBAU, 0x3DU,
	0xCAU, 0x5EU, 0x9BU, 0x9FU, 0x0AU, 0x15U, 0x79U, 0x2BU, 0x4EU, 0xD4U, 0xE5U, 0xACU, 0x73U, 0xF3U, 0xA7U, 0x57U,
	0x07U, 0x70U, 0xC0U, 0xF7U, 0x8CU, 0x80U, 0x63U, 0x0DU, 0x67U, 0x4AU, 0xDEU, 0xEDU, 0x31U, 0xC5U, 0xFEU, 0x18U,
	0xE3U, 0xA5U, 0x99U, 0x77U, 0x26U, 0xB8U, 0xB4U, 0x7CU, 0x11U, 0x44U, 0x92U, 0xD9U, 0x23U, 0x20U, 0x89U, 0x2EU,
	0x37U, 0x3FU, 0xD1U, 0x5BU, 0x95U, 0xBCU, 0xCFU, 0xCDU, 0x90U, 0x87U, 0x97U, 0xB2U, 0xDCU, 0xFCU, 0xBEU, 0x61U,
	0xF2U, 0x56U, 0xD3U, 0xABU, 0x14U, 0x2AU, 0x5DU, 0x9EU, 0x84U, 0x3CU, 0x39U, 0x53U, 0x47U, 0x6DU, 0x41U, 0xA2U,
	0x1FU, 0x2DU, 0x43U, 0xD8U, 0xB7U, 0x7BU, 0xA4U, 0x76U, 0xC4U, 0x17U, 0x49U, 0xECU, 0x7FU, 0x0CU, 0x6FU, 0xF6U,
	0x6CU, 0xA1U, 0x3BU, 0x52U, 0x29U, 0x9DU, 0x55U, 0xAAU, 0xFBU, 0x60U, 0x86U, 0xB1U, 0xBBU, 0xCCU, 0x3EU, 0x5AU,
	0xCBU, 0x59U, 0x5FU, 0xB0U, 0x9CU, 0xA9U, 0xA0U, 0x51U, 0x0BU, 0xF5U, 0x16U, 0xEBU, 0x7AU, 0x75U, 0x2CU, 0xD7U,
	0x4FU, 0xAEU, 0xD5U, 0xE9U, 0xE6U, 0xE7U, 0xADU, 0xE8U, 0x74U, 0xD6U, 0xF4U, 0xEAU, 0xA8U, 0x50U, 0x58U, 0xAFU};

/* multiplication using logarithms */
static unsigned char gmult(unsigned char a, unsigned char b)
{
  if (a == 0U || b == 0U)
	  return 0U;

  unsigned int i = LOG_TABLE[a];
  unsigned int j = LOG_TABLE[b];

  return EXP_TABLE[i + j];
}

/* Simulate a LFSR with generator polynomial for n byte RS code. 
 * Pass in a pointer to the data array, and amount of data. 
 *
 * The parity bytes are deposited into parity.
 */
void CRS129::encode(const unsigned char* msg, unsigned int nbytes, unsigned char* parity)
{
  assert(msg != NULL);
  assert(parity != NULL);

  for (unsigned int i = 0U; i < NPAR + 1U; i++)
	  parity[i] = 0x00U;

  for (unsigned int i = 0U; i < nbytes; i++) {
    unsigned char dbyte = msg[i] ^ parity[NPAR - 1U];

	for (int j = NPAR - 1; j > 0; j--)
		parity[j] = parity[j - 1] ^ ::gmult(POLY[j], dbyte);

	parity[0] = ::gmult(POLY[0], dbyte);
  }
}

// Reed-Solomon (12,9) check
bool CRS129::check(const unsigned char* in)
{
	assert(in != NULL);

	unsigned char parity[4U];
	encode(in, 9U, parity);

	return in[9U] == parity[2U] && in[10U] == parity[1U] && in[11U] == parity[0U];
}

