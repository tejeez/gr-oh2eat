/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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
#include "vec_mov_avg_ff_impl.h"

namespace gr {
  namespace oh2eat {

    vec_mov_avg_ff::sptr
    vec_mov_avg_ff::make(int vecsize, int length)
    {
      return gnuradio::get_initial_sptr
        (new vec_mov_avg_ff_impl(vecsize, length));
    }

    /*
     * The private constructor
     */
    vec_mov_avg_ff_impl::vec_mov_avg_ff_impl(int vecsize, int length)
      : gr::sync_block("vec_mov_avg_ff",
              gr::io_signature::make(1, 1, sizeof(float)*vecsize),
              gr::io_signature::make(1, 1, sizeof(float)*vecsize)),
              d_vecsize(vecsize), d_length(length)
    {}

    /*
     * Our virtual destructor.
     */
    vec_mov_avg_ff_impl::~vec_mov_avg_ff_impl()
    {
    }

    int
    vec_mov_avg_ff_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // could try to optimize this with libvolk later
      
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      if(d_length < 1 || d_length >= d_vecsize) return 0;

      int calcsize, outoffset;
      calcsize = d_vecsize - d_length;
      outoffset = (d_length-1) / 2;

      float mult = 1.0f / d_length;

      memset(out, 0, noutput_items * d_vecsize * sizeof(float));

      float *out1 = out + outoffset;
      for(int n = 0; n < noutput_items; n++) {
        float acc = 0;
        for(int i = 0; i < d_length; i++)
          acc += in[i];

        for(int i = 0; i < calcsize; i++) {
          out1[i] = mult * acc;
          acc += in[i + d_length];
          acc -= in[i];
        }
        out1[calcsize] = mult * acc;

        in += d_vecsize;
        out1 += d_vecsize;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace oh2eat */
} /* namespace gr */

