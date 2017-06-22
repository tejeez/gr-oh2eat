/* -*- c++ -*- */
/* 
 * Copyright 2017 Tatu Peltola.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "int8_to_complex_impl.h"

namespace gr {
  namespace oh2eat {

    int8_to_complex::sptr
    int8_to_complex::make(int input_type)
    {
      return gnuradio::get_initial_sptr
        (new int8_to_complex_impl(input_type));
    }

    /*
     * The private constructor
     */
    int8_to_complex_impl::int8_to_complex_impl(int input_type)
      : gr::sync_decimator("int8_to_complex",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), 2)
    {
      d_input_type = input_type;
    }

    /*
     * Our virtual destructor.
     */
    int8_to_complex_impl::~int8_to_complex_impl()
    {
    }

    int
    int8_to_complex_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];
      if(d_input_type) {
        // signed
        const int8_t *in = (const int8_t *) input_items[0];
        for(int i=0; i<noutput_items; i++) {
          out[i] = 0.0078125f * gr_complex((float)in[2*i], (float)in[2*i+1]);
        }
      } else {
        // unsigned with 127.4 offset for RTL-SDR
        const uint8_t *in = (const uint8_t *) input_items[0];
        for(int i=0; i<noutput_items; i++) {
          out[i] = 0.00783699f * gr_complex((float)in[2*i] - 127.4f, (float)in[2*i+1] - 127.4f);
        }
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace oh2eat */
} /* namespace gr */

