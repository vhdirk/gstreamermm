// -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 2 -*-

/* gstreamermm - a C++ wrapper for gstreamer
 *
 * Copyright 2008 The gstreamermm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <gstreamermm.h>
#include <iostream>

// Following code adapted from GStreamer Application Development Manual
// section 8.3.2 (feature negotiation)

int link_elements_with_filter (const Glib::RefPtr<Gst::Element> e1,
  const Glib::RefPtr<Gst::Element> e2)
{
  Gst::Structure structure("video/x-raw-yuv") ;
  structure.set_field("width", 384);
  structure.set_field("height", 288);
  structure.set_field("framerate", Gst::Fraction(25, 1));
  Glib::RefPtr<Gst::Caps> caps = Gst::Caps::create(structure);
    
  Gst::Structure structure2("video/x-raw-rgb");
  structure2.set_field("width", 384);
  structure2.set_field("height", 288);
  structure2.set_field("framerate", Gst::Fraction(25, 1));
  caps->append_structure(structure2);

  return e1->link_filtered(e2, caps);
}

int main (int argc, char* argv[])
{
  Glib::RefPtr<Gst::Pipeline> pipeline;
  Glib::RefPtr<Gst::Element> e1, e2;

  Gst::init(argc, argv);

  pipeline = Gst::Pipeline::create("pipeline");

  e1 = Gst::ElementFactory::create_element("fakesrc", "source");
  e2 = Gst::ElementFactory::create_element("fakesink", "sink");

  pipeline->add(e1)->add(e2);

  if (!link_elements_with_filter(e1, e2))
    std::cerr << "Falied to link e1 and e2." << std::endl;
  else
    std::cerr << "Succeeded linking e1 and e2 with filter." << std::endl;

  return 0;
}
