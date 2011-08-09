// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file.
 */
// ----------------------------------------------------------------------------
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_AT91__SPI_0_HPP
#define XPCC_AT91__SPI_0_HPP

#include <stdint.h>

#include <xpcc/architecture/utils.hpp>

namespace xpcc
{
	namespace at91
	{
		/**
		 * \brief		Serial Peripheral Interface (SPI0)
		 * 
		 * \ingroup	at91
		 */
		class Spi0
		{
		public:
			/*static void
			initialize(SPI_PRESCALER_t prescaler = SPI_PRESCALER_DIV128_gc,
					bool doubleSpeed = false,
					SPI_MODE_t mode = SPI_MODE_0_gc);
			
			/// initiates a SPI transfer and returns the received byte
			static uint8_t
			write(uint8_t data);
			
			inline static SPI_t&
			getModuleBase()
			{
				return SPI0;
			}
			
			/// Enable the SPI module.
			inline static void
			enable(bool enable)
			{
				SPI0_CTRL = (SPI0_CTRL & ~SPI_ENABLE_bm) | (enable ? SPI_ENABLE_bm : 0);
			}
			
			inline static void
			setPrescaler(SPI_PRESCALER_t prescaler, bool doubleSpeed)
			{
				SPI0_CTRL = (SPI0_CTRL & ~(SPI_PRESCALER_gm | SPI_CLK2X_bm)) | prescaler | (doubleSpeed ? SPI_CLK2X_bm : 0);
			}
			
			inline static void
			setDataOrder(bool LsbFirst)
			{
				SPI0_CTRL = (SPI0_CTRL & ~SPI_DORD_bm) | (LsbFirst ? SPI_DORD_bm : 0);
			}
			
			inline static void
			setMode(SPI_MODE_t mode)
			{
				SPI0_CTRL = (SPI0_CTRL & ~SPI_MODE_gm) | mode;
			}*/
		};
	}
}

#endif // XPCC_AT91__SPI_0_HPP
