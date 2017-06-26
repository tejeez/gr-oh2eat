/* -*- c++ -*- */

#define OH2EAT_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "oh2eat_swig_doc.i"

%{
#include "oh2eat/int8_to_complex.h"
#include "oh2eat/stream_to_vector_overlap.h"
#include "oh2eat/sum_vectors_ff.h"
#include "oh2eat/vec_mov_avg_ff.h"
%}


%include "oh2eat/int8_to_complex.h"
GR_SWIG_BLOCK_MAGIC2(oh2eat, int8_to_complex);
%include "oh2eat/stream_to_vector_overlap.h"
GR_SWIG_BLOCK_MAGIC2(oh2eat, stream_to_vector_overlap);
%include "oh2eat/sum_vectors_ff.h"
GR_SWIG_BLOCK_MAGIC2(oh2eat, sum_vectors_ff);

%include "oh2eat/vec_mov_avg_ff.h"
GR_SWIG_BLOCK_MAGIC2(oh2eat, vec_mov_avg_ff);
