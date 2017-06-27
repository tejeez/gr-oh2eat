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


#ifndef INCLUDED_OH2EAT_CORRELATOR_BANK_DEMOD_H
#define INCLUDED_OH2EAT_CORRELATOR_BANK_DEMOD_H

#include <oh2eat/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
  namespace oh2eat {

    /*!
     * \brief <+description of block+>
     * \ingroup oh2eat
     *
     */
    class OH2EAT_API correlator_bank_demod : virtual public gr::sync_decimator
    {
     public:
      typedef boost::shared_ptr<correlator_bank_demod> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of oh2eat::correlator_bank_demod.
       *
       * To avoid accidental use of raw pointers, oh2eat::correlator_bank_demod's
       * constructor is in a private implementation
       * class. oh2eat::correlator_bank_demod::make is the public interface for
       * creating new instances.
       */
      static sptr make(int sps, int corr_num, int corr_len, const std::vector<gr_complex> &corrs);
    };

  } // namespace oh2eat
} // namespace gr

#endif /* INCLUDED_OH2EAT_CORRELATOR_BANK_DEMOD_H */

