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
#include "stream_to_vector_overlap_impl.h"

namespace gr {
  namespace oh2eat {

    stream_to_vector_overlap::sptr
    stream_to_vector_overlap::make(size_t itemsize, size_t vecsize, size_t every_n_items)
    {
      return gnuradio::get_initial_sptr
        (new stream_to_vector_overlap_impl(itemsize, vecsize, every_n_items));
    }

    /*
     * The private constructor
     */
    stream_to_vector_overlap_impl::stream_to_vector_overlap_impl(size_t itemsize, size_t vecsize, size_t every_n_items)
      : gr::sync_decimator("stream_to_vector_overlap",
              gr::io_signature::make(1, 1, itemsize),
              gr::io_signature::make(1, 1, itemsize * vecsize),
              every_n_items)
    {
      d_itemsize = itemsize;
      d_vecsize = vecsize;
      d_every_n_items = every_n_items;
      vecbytes = vecsize * itemsize;
      every_n_bytes = every_n_items * itemsize;
      if(vecsize > every_n_items) {
        // history is needed if windows do overlap
        set_history(vecsize - every_n_items);
      }
    }

    /*
     * Our virtual destructor.
     */
    stream_to_vector_overlap_impl::~stream_to_vector_overlap_impl()
    {
    }

    int
    stream_to_vector_overlap_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const char *in = (const char *) input_items[0];
      char *out = (char *) output_items[0];

      for(int i=0; i < noutput_items; i++) {
        memcpy(out + vecbytes*i, in + every_n_bytes*i, vecbytes);
      }

      return noutput_items;
    }

  } /* namespace oh2eat */
} /* namespace gr */
