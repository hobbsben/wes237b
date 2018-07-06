/* -*- c++ -*- */
/*
 * Copyright 2004,2010,2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "iir_filter_ffd_impl.h"
#include <gnuradio/io_signature.h>
#include <stdexcept>
#include <iostream>
#include <fstream>

namespace gr {
  namespace filter {
    
    iir_filter_ffd::sptr
    iir_filter_ffd::make(const std::vector<double> &fftaps,
			 const std::vector<double> &fbtaps,
			 bool oldstyle)
    {
      return gnuradio::get_initial_sptr
	(new iir_filter_ffd_impl(fftaps, fbtaps, oldstyle));
    }
 
    iir_filter_ffd_impl::iir_filter_ffd_impl(const std::vector<double> &fftaps,
					     const std::vector<double> &fbtaps,
					     bool oldstyle)

      : sync_block("iir_filter_ffd",
		      io_signature::make(1, 1, sizeof (float)),
		      io_signature::make(1, 1, sizeof (float))),
	d_updated(false)
    {
      d_iir = new kernel::iir_filter<float,float,double>(fftaps, fbtaps, oldstyle);
    }

    iir_filter_ffd_impl::~iir_filter_ffd_impl()
    {
      delete d_iir;
    }

    void
    iir_filter_ffd_impl::set_taps(const std::vector<double> &fftaps,
				  const std::vector<double> &fbtaps)
    {
      d_new_fftaps = fftaps;
      d_new_fbtaps = fbtaps;
      d_updated = true;
    }

    int
    iir_filter_ffd_impl::work(int noutput_items,
			      gr_vector_const_void_star &input_items,
			      gr_vector_void_star &output_items)
    {
      const float *in = (const float*)input_items[0];
      float *out = (float*)output_items[0];

      if(d_updated) {
	d_iir->set_taps(d_new_fftaps, d_new_fbtaps);
	d_updated = false;
      }1

      d_iir->filter_n(out, in, noutput_items);

     // print to file:
     ofstream output_file;
     output_file.open ("irr_output.txt");
     output_file.sekp(0,std::ios::end);   // make sure pointer is at end
     output_file << "\t\t\t" << out;      // print output to text file

      return noutput_items;


      //*********Added*****************//
     // do_update();                    // update d_fp is reqd

      //if(!d_fp)
      //  return noutput_items;         // drop output on the floor

      //while(100 < noutput_items) {
        //int count = fwrite(inbuf, d_itemsize, noutput_items - 100, d_fp);
        //if(count == 0) {
        //  if(ferror(d_fp)) {
        //    std::stringstream s;
        //  if(count == 0) {
        //   if(ferror(d_fp)) {
        //     std::stringstream s;
        //  s << "file_sink write failed with error " << fileno(d_fp) << std::endl;
        //  throw std::runtime_error(s.str());
        // }
        // else { // is EOF
        //   break;
        //  }
        //}
        //nwritten += count;
        //inbuf += count * d_itemsize;
      //}
      //*********Added*****************//


    };

  } /* namespace filter */
} /* namespace gr */

// test
