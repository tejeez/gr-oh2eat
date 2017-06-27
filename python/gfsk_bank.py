#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2017 Tatu Peltola.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 
import numpy
from gnuradio import filter

# generate correlator bank for GFSK
def gfsk_bank(samples_per_symbol=2, modulation_index=0.65, bt=0.5, L=0, N=3, M=2):
	if L <= 0:
		L = samples_per_symbol*(N+1)

	tonestep = 2*numpy.pi * modulation_index / samples_per_symbol
	tone0 = -tonestep * (M-1)/2

	filter_taps = filter.firdes.gaussian(
		1.0,		       # gain
		samples_per_symbol,    # symbol_rate
		bt,		       # bandwidth * symbol time
		4*samples_per_symbol+1 # number of taps
		)


	def L_elements_in_center(f, extraoffset):
		l = len(f)
		first_element = int((l-L)/2) + extraoffset
		return f[first_element : first_element+L]

	# offset by -1 from center because the filter taps seem to be asymmetric or something
	def symbols_to_freqs(symbs):
		return L_elements_in_center(numpy.convolve(numpy.repeat(tonestep * numpy.array(symbs) + tone0, samples_per_symbol), filter_taps, 'same'), -1)

	# set phase of middle sample to 0 so we can sum the signals of multiple symbol sequences
	def move_middle_to_0(f):
		l = len(f)
		return f - 0.5*(f[int(l/2)] + f[int((l-1)/2)])

	def fmmodulate(freqs):
		return numpy.exp(1j * move_middle_to_0(numpy.cumsum(freqs)))


	correlators = []
	S = M**N

	for m in range(S):
		symbols = [0]*N
		for n in range(N):
			symbols[n] = (m / (M**(N-n-1))) % M

		# make it average of multiple correlators with the correct middle sequence
		# TODO: make this work correctly for other M
		symbols_1 = [[0,0]+symbols+[0,0], [0,0]+symbols+[1,1], [1,1]+symbols+[0,0], [1,1]+symbols+[1,1]]
		#symbols_1 = [[0]+symbols+[0], [0]+symbols+[1], [1]+symbols+[0], [1]+symbols+[1]]
		c = numpy.zeros(L, dtype=numpy.complex)
		for s in symbols_1:
			c += fmmodulate(symbols_to_freqs(s))
		c *= 1.0 / len(symbols_1)

		correlators.extend(c)
	#print numpy.abs(generate_correlators()) # debug
	return correlators
