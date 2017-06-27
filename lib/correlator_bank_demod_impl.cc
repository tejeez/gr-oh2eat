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
#include "correlator_bank_demod_impl.h"

namespace gr {
  namespace oh2eat {

    correlator_bank_demod::sptr
    correlator_bank_demod::make(int sps, int corr_num, int corr_len, const std::vector<gr_complex> &corrs)
    {
      return gnuradio::get_initial_sptr
        (new correlator_bank_demod_impl(sps, corr_num, corr_len, corrs));
    }

    /*
     * The private constructor
     */
    correlator_bank_demod_impl::correlator_bank_demod_impl(int sps, int corr_num, int corr_len, const std::vector<gr_complex> &corrs)
      : gr::sync_decimator("correlator_bank_demod",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)), sps),
        d_sps(sps), d_corr_num(corr_num), d_corr_len(corr_len)
    {
      if(corrs.size() != corr_num * corr_len) {
        throw std::runtime_error("size of corrs vector should be corr_num * corr_len");
        return;
      }

      int alignment = volk_get_alignment();
      int alignment_items = alignment / sizeof(gr_complex);
      if(alignment_items > 1) set_alignment(alignment_items);
      set_history(corr_len);

      // aligned length of correlators
      corr_len_a = (corr_len + alignment_items - 1) / alignment_items * alignment_items;

      // copy taps so that they are aligned for volk
      correlators = (gr_complex*)volk_malloc(sizeof(gr_complex) * corr_num * corr_len_a, alignment);
      memset(correlators, 0, corr_num*corr_len_a * sizeof(gr_complex));

      for(int c = 0; c < corr_num; c++) {
        gr_complex *correlator = correlators + c*corr_len_a;
        for(int i = 0; i < corr_len; i++) {
          correlator[i] = std::conj(corrs[c*corr_len + i]);
        }
      }
    }

    /*
     * Our virtual destructor.
     */
    correlator_bank_demod_impl::~correlator_bank_demod_impl()
    {
    }

    int
    correlator_bank_demod_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      for(int i = 0; i < noutput_items; i++) {
        const gr_complex *in1 = in + i*d_sps;
        float max_mag2 = 0;
        int max_c = 0;
        for(int c = 0; c < d_corr_num; c++) {
          float mag2;
          gr_complex result;
          volk_32fc_x2_dot_prod_32fc(&result, in1, correlators + c*corr_len_a, d_corr_len);
          mag2 = result.real()*result.real() + result.imag()*result.imag(); // squared magnitude of correlation
          if(mag2 > max_mag2) {
            max_mag2 = mag2;
            max_c = c;
          }
        }
        out[i] = max_c;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace oh2eat */
} /* namespace gr */

