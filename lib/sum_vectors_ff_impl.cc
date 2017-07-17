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
#include <volk/volk.h>
#include "sum_vectors_ff_impl.h"

namespace gr {
  namespace oh2eat {

    sum_vectors_ff::sptr
    sum_vectors_ff::make(size_t vecsize, int decimation, float scale)
    {
      return gnuradio::get_initial_sptr
        (new sum_vectors_ff_impl(vecsize, decimation, scale));
    }

    /*
     * The private constructor
     */
    sum_vectors_ff_impl::sum_vectors_ff_impl(size_t vecsize, int decimation, float scale)
      : gr::sync_decimator("sum_vectors_ff",
              gr::io_signature::make(1, 1, sizeof(float)*vecsize),
              gr::io_signature::make(1, 1, sizeof(float)*vecsize), decimation)
    {
      d_vecsize = vecsize;
      d_decimation = decimation;
      d_scale = scale;
      d_vecbytes = sizeof(float)*vecsize;

      int alignment = volk_get_alignment();
      int alignment_items = alignment / (sizeof(float)*vecsize);
      if(alignment_items > 1) set_alignment(alignment_items);
      //accumulator = (float*)volk_malloc(sizeof(float) * vecsize, alignment);
    }

    /*
     * Our virtual destructor.
     */
    sum_vectors_ff_impl::~sum_vectors_ff_impl()
    {
      //volk_free(accumulator);
    }

    int
    sum_vectors_ff_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      for(int n = 0; n < noutput_items; n++) {
        const float *in1 = in + d_decimation * d_vecsize * n;
        float *accumulator = out + d_vecsize * n;

        memcpy(accumulator, in1, d_vecbytes);

        // can the input and output be the same array in libvolk?
        for(int i = 1; i < d_decimation; i++)
          volk_32f_x2_add_32f(accumulator, accumulator, in1 + d_vecsize * i, d_vecsize);

        if(d_scale != 1)
          volk_32f_s32f_multiply_32f(accumulator, accumulator, d_scale, d_vecsize);
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace oh2eat */
} /* namespace gr */

