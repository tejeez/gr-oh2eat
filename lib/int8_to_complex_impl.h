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

#ifndef INCLUDED_OH2EAT_INT8_TO_COMPLEX_IMPL_H
#define INCLUDED_OH2EAT_INT8_TO_COMPLEX_IMPL_H

#include <oh2eat/int8_to_complex.h>

namespace gr {
  namespace oh2eat {

    class int8_to_complex_impl : public int8_to_complex
    {
     private:
      int d_input_type;

     public:
      int8_to_complex_impl(int input_type);
      ~int8_to_complex_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace oh2eat
} // namespace gr

#endif /* INCLUDED_OH2EAT_INT8_TO_COMPLEX_IMPL_H */

